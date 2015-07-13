#ifndef SEQUENCE_H
#define SEQUENCE_H


#include <iostream>
#include <limits>
#include <cstdlib> // rand
#include <time.h> // time
#include <new> // bad_alloc
#include <string.h> // memcpy


namespace cal {


  unsigned int* allocateSequence(const unsigned int length);
  unsigned int* copySequence(const unsigned int* sequence, const unsigned int length);
  unsigned int* getRandomSequence(const unsigned int length, const unsigned int max = std::numeric_limits<unsigned int>::max());
  unsigned int* getAscendingSequence(const unsigned int length);
  unsigned int* getDescendingSequence(const unsigned int length);


};


#endif // SEQUENCE_H