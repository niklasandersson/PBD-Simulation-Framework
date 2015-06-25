#ifndef TESTLOG_H
#define TESTLOG_H

#include "log/Log.h" 

class TestLog : public Log {

public:
  TestLog();
  ~TestLog() = default;

  std::string getBatchName(const std::string& name);
  std::string getDateAndTime(const std::string& dateAndTime);
  std::string getTestName(const std::string& name);

  void testBegin(const std::string& name);
  void testEnd(const bool testResult);
  void testEnd(const bool testResult, const unsigned int duration);

  std::string getResult(const bool testResult, const bool hasWarning = false) const;

protected:

private:

};

#endif