#define BLOCK_SIZE 512

__kernel void main(
                   __global unsigned int* input,
                   __global unsigned int* output,
                   const unsigned int step,
                   const unsigned int numberOfElements
                  ) {

  const unsigned int local_id = get_local_id(0);
  const unsigned int group_id = get_group_id(0);

  const unsigned int read_left = (group_id/step)*2*step + group_id%step; 
  const unsigned int read_right = read_left + step; 

  const unsigned int write_left = read_left;  
  const unsigned int write_right = read_right; 
  
  if( (read_left*(BLOCK_SIZE/2) + local_id < numberOfElements) &&
      (read_right*(BLOCK_SIZE/2) + local_id < numberOfElements) ) {

    const unsigned int sml = input[read_left*(BLOCK_SIZE/2) + local_id];
    const unsigned int smb = input[read_right*(BLOCK_SIZE/2) + local_id];

    const bool shouldSwap = sml > smb;

    output[write_left*(BLOCK_SIZE/2) + local_id] = shouldSwap ? smb : sml;
    output[write_right*(BLOCK_SIZE/2) + local_id] = shouldSwap ? sml : smb;

  }

}