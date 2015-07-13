#define BLOCK_SIZE 512

__kernel void main(
                   __global unsigned int* input,
                   __global unsigned int* output,
                   const unsigned int numberOfElements
                  ) {

  const unsigned int local_id = get_local_id(0);
  const unsigned int group_id = get_group_id(0);

  __local unsigned int shared_memory[BLOCK_SIZE]; 

  const unsigned int read_left = group_id*BLOCK_SIZE;
  const unsigned int read_right = group_id*BLOCK_SIZE + BLOCK_SIZE/2;

  const unsigned int write_left = read_left;
  const unsigned int write_right = read_right;

  if( (read_left + local_id < numberOfElements) &&
      (read_right + local_id < numberOfElements) ) {

    shared_memory[local_id] = input[read_left + local_id];
    shared_memory[(BLOCK_SIZE/2) + local_id] = input[read_right + local_id];

    unsigned int jump = BLOCK_SIZE/2;
    while( jump > 0 ) { 
      const unsigned int offset = (local_id / jump) * (jump << 1) + local_id % jump;

      barrier(CLK_LOCAL_MEM_FENCE);

      const unsigned int smo = shared_memory[offset];
      const unsigned int smj = shared_memory[jump + offset];

      if( smo > smj ) {
        // SWAP
        shared_memory[offset] = smj;
        shared_memory[jump + offset] = smo;
      }

      jump >>= 1;
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    output[write_left + local_id] = shared_memory[local_id];
    output[write_right + local_id] = shared_memory[(BLOCK_SIZE/2) + local_id];
  
  }

}