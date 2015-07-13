#include "sequence.h"


namespace cal {


  unsigned int* allocateSequence(const unsigned int length) {
    unsigned int* sequence;

    try {
      sequence = new unsigned int[length];
    } catch(std::bad_alloc& exception) {
      std::cout << "Failed to allocate sequence of " << length << " length." << std::endl;
      throw;
    }

    return sequence;
  }


  unsigned int* copySequence(const unsigned int* sequence, const unsigned int length) {
    unsigned int* copy = allocateSequence(length);
    memcpy(copy, sequence, length * sizeof(unsigned int));
    return copy;
  }


  unsigned int* getRandomSequence(const unsigned int length, const unsigned int max) {
    unsigned int* sequence = allocateSequence(length);

    srand(time(NULL));
    for(unsigned int i=0; i<length; i++) {
      sequence[i] = rand() % max;
    }

    return sequence;
  }


  unsigned int* getAscendingSequence(const unsigned int length) {
    unsigned int* sequence = allocateSequence(length);

    for(unsigned int i=0; i<length; i++) {
      sequence[i] = i;
    }

    return sequence;
  }


  unsigned int* getDescendingSequence(const unsigned int length) {
    unsigned int* sequence = allocateSequence(length);

    for(unsigned int i=0; i<length; i++) {
      sequence[i] = length-1-i;
    }

    return sequence;
  }


};

