#ifndef TESTMANAGER_H
#define TESTMANAGER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <typeinfo>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <string>

#include "log/Log.h"

#include "test/Test.h"
#include "test/Batch.h"

#include "test/TestAbleClass.h"

class TestManager {

public:
  static TestManager& getInstance();
  ~TestManager() = default;

  static bool parseCommandLine(const int argc, const char* argv[]);

  void add(TestAbleClass* testClass);
  void sortTests();
  bool runAllTests();
  void countTestsAndBatches();
  void listAllTests();
  void listAllBatches();
  void saveAllTestsToFile();
  void addTest(Test* test);
  bool runTests(const std::vector<std::string>& testNames);

protected:

private:
  TestManager();
  void registerTests() const;
  void printHelp();

  std::vector<TestAbleClass*> testClasses_;
  std::vector<Test*> tests_;
  unsigned int numberOfBatches_;
  unsigned int numberOfTests_;

};

#endif