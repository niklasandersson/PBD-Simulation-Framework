#include "test/TestClass.h"

#include <CL/cl.h>
#include <CL/cl_gl.h>
#ifdef __linux__
  #include <GL/glx.h>
#endif

#include "opencl/OpenCLStub.h"
#include "opencl/OpenCL_Utilities.h"

#include "opencl/sort/BitonicMergeSort.h"

#include "calculus/sequence.h"


BATCH_CLASS(BitonicMergeSortBatch) {


  TEST(SortTest) {

    cl_context context_                = OpenCLStub::getInstance().getContext();
    cl_command_queue command_queue_    = OpenCLStub::getInstance().getCommandQueue();

    cl_kernel sortingInKernel_         = OpenCLStub::getInstance().getKernel("in_kernel_sort.cl");
    cl_kernel firstInterleavedMerging_ = OpenCLStub::getInstance().getKernel("bitonic_interleaved_first.cl");
    cl_kernel interleavedMerging_      = OpenCLStub::getInstance().getKernel("bitonic_interleaved.cl");
    cl_kernel mergingInKernel_         = OpenCLStub::getInstance().getKernel("bitonic_self.cl");

    const unsigned int numberOfElements = 65536;
    // const unsigned int numberOfElements = 131072;
    // const unsigned int numberOfElements = 262144;
    // const unsigned int numberOfElements = 16777216;
    // const unsigned int numberOfElements = 33554432;
    const unsigned int blockSize = 512;

    log_ << "numberOfElements: " << numberOfElements << std::endl;
    log_ << "blockSize: " << blockSize << std::endl;

    unsigned int* inputSequence = cal::getRandomSequence(numberOfElements);
    unsigned int* reference = cal::copySequence(inputSequence, numberOfElements);

    cl_mem input = CL_CALL(
                           clCreateBuffer,
                           context_,
                           CL_MEM_READ_WRITE,
                           numberOfElements * sizeof(unsigned int),
                           nullptr
                          );

    cl_mem scratch = CL_CALL(
                             clCreateBuffer,
                             context_,
                             CL_MEM_READ_WRITE,
                             numberOfElements * sizeof(unsigned int),
                             nullptr
                            );

    // Upload data
    {
      CL_CALL(clFinish, command_queue_);
      cl_event event;
      CL_CALL(
              clEnqueueWriteBuffer, 
              command_queue_, 
              input, 
              CL_TRUE, 
              0, 
              numberOfElements * sizeof(unsigned int), 
              &inputSequence[0], 
              0, 
              nullptr, 
              &event
             );
      clWaitForEvents(1, &event);
      CL_CALL(clFinish, command_queue_); 
    }


    ocl::BitonicMergeSortInfo info{
      command_queue_,
      sortingInKernel_,
      firstInterleavedMerging_,
      interleavedMerging_,
      mergingInKernel_,
      input,
      scratch,
      numberOfElements,
      blockSize
    };

    CL_CALL(clFinish, command_queue_); 
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    // Sort data
    cl_mem sorted_sequence = ocl::bitonicMergeSort(info);

    CL_CALL(clFinish, command_queue_);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    auto durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    log_ << "OpenCL BitonicMergeSort: " << durationTotal << " ms" << std::endl;

    unsigned int* sortedSequenceReceived = cal::allocateSequence(numberOfElements);

    // Download data
    {
      CL_CALL(clFinish, command_queue_);
      cl_event event;
      CL_CALL(
              clEnqueueReadBuffer, 
              command_queue_, 
              sorted_sequence, 
              CL_TRUE, 
              0, 
              numberOfElements * sizeof(unsigned int), 
              &sortedSequenceReceived[0], 
              0, 
              nullptr, 
              &event
             );
      clWaitForEvents(1, &event);
      CL_CALL(clFinish, command_queue_); 
    }

    // Sort reference
    std::chrono::high_resolution_clock::time_point stdSortT1 = std::chrono::high_resolution_clock::now();
    std::sort(reference, reference + numberOfElements);
    std::chrono::high_resolution_clock::time_point stdSortT2 = std::chrono::high_resolution_clock::now();
    auto stdSortDuration = std::chrono::duration_cast<std::chrono::milliseconds>( stdSortT2 - stdSortT1 ).count();
    log_ << "std::sort " << stdSortDuration << " ms" << std::endl;

    // Compare results
    bool testResult = true;
    for(unsigned int i=0; i<numberOfElements; i++) {
      if( sortedSequenceReceived[i] != reference[i] ) {
        testResult = false;
        break;
      }
    }

    delete [] inputSequence;
    delete [] reference;
    delete [] sortedSequenceReceived;

    return testResult;
  };

};