#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>
#include <vector>
#include <typeinfo>
#include <utility> 
#include <limits>

#include "log/TestLog.h"


class Test {

public:
  Test(const std::string& name, const bool shouldAddTest = true);
  Test(const std::string& name, const std::string& fileName, const unsigned int line, const bool shouldAddTest = true);
  virtual ~Test() = default;

  std::string getName() const;
  void clearLog();
  std::string getLog() const;

  void enablePrintOut();
  void disablePrintOut();

  bool shouldPrintOut() const;

  virtual bool test() = 0;
  virtual bool run();

  void setIndent(const unsigned int indent);
  unsigned int getIndent() const;

  void setDuration(const unsigned int durationInMs);
  unsigned int getDuration() const;
  void resetDuration();
  bool hasSetDuration() const;

  void setFileName(const std::string& fileName);
  std::string getFileName() const;
  bool hasFileName() const;
  void setLine(const unsigned int line);
  unsigned int getLine() const;
  bool hasLine() const;
  void log(const std::string& message);

  bool hasWarning() const;
  void setHasWarning(const bool hasWarning);

protected:
  TestLog log_;
  unsigned int durationInMs_;
  bool hasSetDuration_;
  std::string fileName_;
  bool hasFileName_;
  unsigned int line_;
  bool hasLine_;

private:
  unsigned int indent_;
  const std::string name_;
  bool printOut_;
  bool hasWarning_;

};


#endif // TEST_H