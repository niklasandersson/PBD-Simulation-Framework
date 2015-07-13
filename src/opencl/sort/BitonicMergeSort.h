#ifndef BITONICMERGESORT_H
#define BITONICMERGESORT_H


#include <algorithm> // swap

#include <CL/cl.h> 

#include "opencl/OpenCL_Utilities.h" // CL_CALL
#include "calculus/calculus.h" // isOdd


namespace ocl {


  struct BitonicMergeSortInfo {

    BitonicMergeSortInfo(
                         cl_command_queue command_queue,
                         cl_kernel sortingInKernel,
                         cl_kernel firstInterleavedMerging,
                         cl_kernel interleavedMerging,
                         cl_kernel mergingInKernel,
                         cl_mem input, 
                         cl_mem scratch, 
                         const unsigned int numberOfElements, 
                         const unsigned int blockSize
                        )
    : command_queue_{command_queue}
    , sortingInKernel_{sortingInKernel}
    , firstInterleavedMerging_{firstInterleavedMerging}
    , interleavedMerging_{interleavedMerging}
    , mergingInKernel_{mergingInKernel}
    , input_{input}
    , scratch_{scratch}
    , numberOfElements_{numberOfElements}
    , blockSize_{blockSize}
    {
    }

    cl_command_queue command_queue_;
    cl_kernel sortingInKernel_;
    cl_kernel firstInterleavedMerging_;
    cl_kernel interleavedMerging_;
    cl_kernel mergingInKernel_;
    cl_mem input_;
    cl_mem scratch_;
    const unsigned int numberOfElements_;
    const unsigned int blockSize_;

  };


  namespace bms {


    inline void interleavedMerging(
                                   const BitonicMergeSortInfo& info,
                                   cl_mem input, 
                                   cl_mem output, 
                                   const unsigned int iteration, 
                                   const unsigned int step
                                  ) {

      const size_t global[3] = { info.numberOfElements_, 1, 1 };
      const size_t local[3] = { info.blockSize_ / 2, 1, 1 };

      cl_kernel merging = iteration == 0 ? info.firstInterleavedMerging_ : info.interleavedMerging_;

      CL_CALL(clSetKernelArg, merging, 0, sizeof(cl_mem), &input);
      CL_CALL(clSetKernelArg, merging, 1, sizeof(cl_mem), &output);
      CL_CALL(clSetKernelArg, merging, 2, sizeof(unsigned int), &step);
      CL_CALL(clSetKernelArg, merging, 3, sizeof(unsigned int), &info.numberOfElements_);

      CL_CALL(
              clEnqueueNDRangeKernel, 
              info.command_queue_,
              merging, 
              1, 
              nullptr, 
              global, 
              local, 
              0, 
              nullptr, 
              nullptr
             );

    }


    inline void inKernelMerging(
                                const BitonicMergeSortInfo& info,
                                cl_mem input, 
                                cl_mem output
                               ) {

      const size_t global[3] = { info.numberOfElements_, 1, 1 };
      const size_t local[3] = { info.blockSize_ / 2, 1, 1 };

      CL_CALL(clSetKernelArg, info.mergingInKernel_, 0, sizeof(cl_mem), &input);
      CL_CALL(clSetKernelArg, info.mergingInKernel_, 1, sizeof(cl_mem), &output);
      CL_CALL(clSetKernelArg, info.mergingInKernel_, 2, sizeof(unsigned int), &info.numberOfElements_);

      CL_CALL(
              clEnqueueNDRangeKernel, 
              info.command_queue_,
              info.mergingInKernel_, 
              1, 
              nullptr, 
              global, 
              local, 
              0, 
              nullptr, 
              nullptr
             );
      
    }


    inline void inKernelSort(
                             const BitonicMergeSortInfo& info,
                             cl_mem input, 
                             cl_mem output
                            ) {

      const size_t global[3] = { info.numberOfElements_, 1, 1 };
      const size_t local[3] = { info.blockSize_, 1, 1 };

      CL_CALL(clSetKernelArg, info.sortingInKernel_, 0, sizeof(cl_mem), &input);
      CL_CALL(clSetKernelArg, info.sortingInKernel_, 1, sizeof(cl_mem), &output);

      CL_CALL(
              clEnqueueNDRangeKernel, 
              info.command_queue_,
              info.sortingInKernel_, 
              1, 
              nullptr, 
              global, 
              local, 
              0, 
              nullptr, 
              nullptr
             );

    }


  }; // namespace bms


  inline cl_mem bitonicMergeSort(const BitonicMergeSortInfo& info) {

    bms::inKernelSort(info, info.input_, info.scratch_);

    cl_mem in = info.scratch_;
    cl_mem out = info.input_;

    unsigned int numberOfSwaps = 0;
    unsigned int halfBlockSize = info.blockSize_ / 2;
    unsigned int numberOfChunks = info.numberOfElements_ / info.blockSize_;

    while( numberOfChunks > 1 ) {

      unsigned int step = halfBlockSize / (info.blockSize_ / 4); 
      unsigned int iteration = 0;

      while( step > 1 ) {
        
        bms::interleavedMerging(info, in, out, iteration, step);

        std::swap(in, out);
        numberOfSwaps++;

        iteration++;
        step /= 2;
      }

      bms::inKernelMerging(info, in, out);

      std::swap(in, out);
      numberOfSwaps++;

      halfBlockSize *= 2;
      numberOfChunks /= 2; 
    }
    
    return cal::isOdd(numberOfSwaps) ? info.input_ : info.scratch_; 
  }


}; // namespace ocl


#endif // BITONICMERGESORT_H