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

#include "test/Test.h"
#include "test/Batch.h"

#include "test/TestAbleClass.h"

class TestManager {

public:
  static TestManager& getInstance() {
    static TestManager manager;
    return manager;
  }

  ~TestManager() = default;


  static bool parseCommandLine(const int argc, const char* argv[]) {
    bool result = false;

    if( argc > 1 ) {

      std::vector<std::string> argvToLower;
      for(unsigned int i=0; i<argc; i++) {
        std::string temp = argv[i];
        std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        argvToLower.push_back(temp);
      }

      if( argc == 2 ) {
        if( argvToLower[1] == "help") {
          TestManager::getInstance().printHelp();
          return true;
        } if( (argvToLower[1] == "ls") || (argvToLower[1] == "list") ) {
          TestManager::getInstance().listAllBatches();
          TestManager::getInstance().listAllTests();
          return true;
        }
      } else if( argc == 3 ) {
        if( ((argvToLower[1] == "ls") || (argvToLower[1] == "list")) && argvToLower[2] == "batches" ) {
          TestManager::getInstance().listAllBatches();
          return true;
        } else if( ((argvToLower[1] == "ls") || (argvToLower[1] == "list")) && argvToLower[2] == "tests" ) {
          TestManager::getInstance().listAllTests();
          return true;
        }
      }

      std::vector<std::string> tests; 
      for(unsigned int i=1; i<argc; i++) {
        std::ostringstream os{};
        os << argv[i];
        tests.push_back(os.str());
      }

      result = TestManager::getInstance().runTests(tests);

    } else {
      result = TestManager::getInstance().runAllTests();
    }

    return result;
  }


  void add(TestAbleClass* testClass) {
    testClasses_.push_back(testClass);
  }


  void sortTests() {
    std::sort(tests_.begin(), tests_.end(), [](const Test* t1, Test* t2) -> bool {
      std::string str1 = t1->getName();
      std::string str2 = t2->getName();
      std::transform(str1.begin(), str1.end(),str1.begin(), ::toupper);
      std::transform(str2.begin(), str2.end(),str2.begin(), ::toupper);
      return str1 < str2;
    });
  }


  bool runAllTests() {
    Batch testBatch{"TestBatch", false};

    registerTests();
    sortTests();
    
    for(auto& test : tests_) {
      if( typeid(*test) != typeid(Batch) ) {
        testBatch.add(test);
      }
    }

    return testBatch.run();
  }


  void countTestsAndBatches() {
    numberOfBatches_ = 0;
    numberOfTests_ = 0;
    for(auto& test : tests_) {
      if( typeid(*test) == typeid(Batch) ) {
        numberOfBatches_++;
      } else {
        numberOfTests_++;
      }
    }
  }


  void listAllTests() {
    registerTests();
    sortTests();
    countTestsAndBatches();
    std::cout << "All " << numberOfTests_ << " tests:" << std::endl;
    for(auto& test : tests_) {
      if( typeid(*test) != typeid(Batch) ) {
        std::cout << "  " << test->getName() << std::endl;
      }
    }
    std::cout << std::endl;
  }


  void listAllBatches() {
    registerTests();
    sortTests();
    countTestsAndBatches();
    std::cout << "All " << numberOfBatches_ << " batches:" << std::endl;
    for(auto& test : tests_) {
      if( typeid(*test) == typeid(Batch) ) {
        std::cout << "  " << test->getName() << std::endl;
      }
    }
    std::cout << std::endl;
  }


  void addTest(Test* test) {
    tests_.push_back(test);
  }


  bool runTests(const std::vector<std::string>& testNames) {
    Batch testBatch{"TestBatch", false};

    registerTests();

    Test* oneBatch;
    Test* oneTest;
    unsigned int matchingBatchesCounter = 0;
    unsigned int matchingTestsCounter = 0;

    for(auto& testName : testNames) {
      bool added = false;
      for(auto& test : tests_) {
        if( test->getName() == testName ) {
          testBatch.add(test);
          added = true;
          if( typeid(*test) == typeid(Batch) ) {
            oneBatch = test;
            matchingBatchesCounter++;
          } else if( typeid(*test) != typeid(Batch) ) {
            oneTest = test;
            matchingTestsCounter++;
          }
        }
      }

      if( !added ) {
        std::ostringstream os{};
        os << "Error: The Test '" << testName << "' was not found."; 
        throw std::invalid_argument{os.str()};
      }
    }

    return (matchingBatchesCounter == 1) && (matchingTestsCounter == 0) ? oneBatch->run() : testBatch.run();
  }

protected:

private:
  TestManager() {
    numberOfTests_ = 0;
    numberOfBatches_ = 0;
  }

  void registerTests() const {
    for(auto& testClass : testClasses_) {
        testClass->registerTests();
    }
  }

  void printHelp() {
    std::cout << "Testing help:" << std::endl;

    std::cout << std::endl;

    std::cout << "./make.sh" << std::endl;
    std::cout << "  makes main runnable and test runnable" << std::endl;

    std::cout << std::endl;

    std::cout << "./make.sh clean" << std::endl;
    std::cout << "  cleans main runnable and test runnable" << std::endl;

    std::cout << std::endl;

    std::cout << "./make.sh all" << std::endl;
    std::cout << "  makes main runnable and test runnable" << std::endl;

    std::cout << std::endl;

    std::cout << "./make.sh all clean" << std::endl;
    std::cout << "  cleans main runnable and test runnable" << std::endl;

    std::cout << std::endl;

    std::cout << "./make.sh main" << std::endl;
    std::cout << "  makes main runnable" << std::endl;

    std::cout << std::endl;

    std::cout << "./make.sh main clean" << std::endl;
    std::cout << "  cleans main runnable" << std::endl;

    std::cout << std::endl;

    std::cout << "./make.sh test" << std::endl;
    std::cout << "  makes test runnable" << std::endl;

    std::cout << std::endl;

    std::cout << "./make.sh test clean" << std::endl;
    std::cout << "  cleans test runnable" << std::endl;

    std::cout << std::endl;

    std::cout << "./test" << std::endl;
    std::cout << "  runs all tests" << std::endl; 

    std::cout << std::endl;

    std::cout << "./test TestName" << std::endl;
    std::cout << "  runs the test TestName" << std::endl;

    std::cout << std::endl;

    std::cout << "./test TestName1 TestName2 ..." << std::endl;
    std::cout << "  runs the tests TestName1 TestName2 ..." << std::endl;

    std::cout << std::endl;

    std::cout << "./test BatchName" << std::endl;
    std::cout << "  runs the batch BatchName" << std::endl;

    std::cout << std::endl;

    std::cout << "./test BatchName1 BatchName2 ..." << std::endl;
    std::cout << "  runs the batches BatchName1 BatchName2 ..." << std::endl;

    std::cout << std::endl;

    std::cout << "./test TestName1 BatchName2 TestName3 ..." << std::endl;
    std::cout << "  runs the TestName1 BatchName2 TestName3 ..." << std::endl;

    std::cout << std::endl;

    std::cout << "./test ls" << std::endl;
    std::cout << "  lists all batches and tests" << std::endl;

    std::cout << std::endl;

    std::cout << "./test ls batches" << std::endl;
    std::cout << "  lists all batches" << std::endl;

    std::cout << std::endl;

    std::cout << "./test ls batches" << std::endl;
    std::cout << "  lists all tests" << std::endl;
    
    std::cout << std::endl;

    std::cout << "You can be using list instead of ls" << std::endl;

    std::cout << std::endl;

    std::cout << "Logs will for now be placed in /logs/test and are also for now Ansi color based." << std::endl;
    std::cout << "To get Ansi coloring in Sublime open package install by ctr-shift-p and install Ansible." << std::endl;

    std::cout << std::endl;
  }

  std::vector<TestAbleClass*> testClasses_;
  std::vector<Test*> tests_;
  unsigned int numberOfBatches_;
  unsigned int numberOfTests_;

};

#endif