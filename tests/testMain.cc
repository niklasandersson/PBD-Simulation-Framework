#include <iostream>

#include "test/TestManager.h"

int main(int argc, const char* argv[]) {

  const bool testResult = TestManager::parseCommandLine(argc, argv);

  return testResult ? EXIT_SUCCESS : EXIT_FAILURE;

}
