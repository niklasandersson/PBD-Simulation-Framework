#include "log/TestLog.h"


std::string TestLog::getFileTag() const {
  return "ansi";
}

// #ifdef __linux__ 
//     //linux code goes here
// #elif _WIN32
//     // windows code goes here
// #else

std::string TestLog::getBatchName(const std::string& name) {
  std::ostringstream os;
  os << "\033[1;36m" << name << "\033[0m";
  return os.str();
}


std::string TestLog::getDateAndTime(const std::string& dateAndTime) {
  std::ostringstream os;
  os << "\033[35m" << dateAndTime << "\033[0m";
  return os.str();
}


std::string TestLog::getTestName(const std::string& name) {
  std::ostringstream os;
  os << "\033[1;34m" << name << "\033[0m";
  return os.str();
}


void TestLog::testBegin(const std::string& name) {
  *this << getTestName(name) << ": " << std::endl;
  // *this << name << ": " << std::endl;
}


std::string TestLog::getResult(const bool testResult, const bool hasWarning) const {
  if( testResult ) {
    if( hasWarning ) {
      return "\033[1;33mPASS\033[0m";
    } else {
      return "\033[1;32mPASS\033[0m";
    }
  } else {
    return "\033[1;31mFAIL\033[0m";
  }
}


void TestLog::testEnd(const bool testResult) {
  // *this << getResult(testResult) << std::endl << std::endl;
  *this << (testResult ? "PASS" : "FAIL") << std::endl << std::endl;
}

void TestLog::testEnd(const bool testResult, const unsigned int duration) {
  std::ostringstream os;
  os << duration << " ms";
  *this << getResult(testResult) << ", " << getDateAndTime(os.str()) << std::endl << std::endl;
  // *this << (testResult ? "PASS" : "FAIL") << std::endl << std::endl;
}






