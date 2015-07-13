#define BLOCK_SIZE 512

__kernel void main(
                   __global unsigned int* input,
                   __global unsigned int* output
                  ) {

  const unsigned int local_id = get_local_id(0);
  const unsigned int group_id = get_group_id(0);

  __local unsigned int shared_memory[BLOCK_SIZE]; 

  const unsigned int offset = group_id * BLOCK_SIZE;
  input += offset; 
  output += offset;

  shared_memory[local_id] = input[local_id];

  barrier(CLK_LOCAL_MEM_FENCE); 

  for(unsigned int k=2; k<=BLOCK_SIZE; k<<=1) {
    for(unsigned int j=k>>1; j>0; j=j>>1) {

      const unsigned int ixj = local_id ^ j;

      if( ixj > local_id ) {
        const unsigned int val = local_id & k;

        const bool vz = val == 0;
        const bool vnz = val != 0;

        const bool vzorvnz = vz || vnz;

        const unsigned int sml = vzorvnz ? shared_memory[local_id] : 0;
        const unsigned int smx = vzorvnz ? shared_memory[ixj] : 0;

        const bool a = (vz && (sml > smx));
        
        const unsigned int aal = a ? smx : sml;
        
        shared_memory[local_id] = aal;

        const unsigned int aax = a ? sml : smx;

        shared_memory[ixj] = aax;
        const bool b = (vnz && (sml < smx));

        shared_memory[local_id] = b ? smx : aal;
        shared_memory[ixj] = b ? sml : aax;
      }


      /*if( (ixj) > local_id ) {  
        if ((local_id&k)==0 && shared_memory[local_id]>shared_memory[ixj]) {
          const unsigned int temp = shared_memory[local_id];
          shared_memory[local_id] = shared_memory[ixj];
          shared_memory[ixj] = temp;
        }
        if ((local_id&k)!=0 && shared_memory[local_id]<shared_memory[ixj])  {
          const unsigned int temp = shared_memory[local_id];
          shared_memory[local_id] = shared_memory[ixj];
          shared_memory[ixj] = temp;
        }
      }*/

      barrier(CLK_LOCAL_MEM_FENCE);
      
    }
  }

  output[local_id] = shared_memory[local_id];

}
