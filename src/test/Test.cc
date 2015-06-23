#include "test/Test.h"
#include "test/TestManager.h"


Test::Test(const std::string& name, const bool shouldAddTest) : name_{name} {
  if( shouldAddTest ) {
    TestManager::getInstance().addTest(this);
  }  
  log_.setPath("logs/test/");
  log_.setName(name_);
  printOut_ = false;
  indent_ = 0;
  hasFileName_ = false;
  hasLine_ = false;
  hasWarning_ = false;
  resetDuration();
}

Test::Test(const std::string& name, const std::string& fileName, const unsigned int line, const bool shouldAddTest) : name_{name} {
  if( shouldAddTest ) {
    TestManager::getInstance().addTest(this);
  }  
  log_.setPath("logs/test/");
  log_.setName(name_);
  printOut_ = false;
  indent_ = 0;
  fileName_ = fileName;
  hasFileName_ = true;
  line_ = line;
  hasLine_ = true;
  hasWarning_ = false;
  resetDuration();
}



void Test::enablePrintOut() {
  printOut_ = true;
  log_.enablePrintToCout();
}


void Test::disablePrintOut() {
  printOut_ = false;
  log_.disablePrintToCout();
}


bool Test::shouldPrintOut() const {
  return printOut_;
}


void Test::setIndent(const unsigned int indent) {
  indent_ = indent;
}


unsigned int Test::getIndent() const {
  return indent_;
}


std::string Test::getName() const {
  return name_;
}


bool Test::hasWarning() const {
  return hasWarning_;
}


void Test::setHasWarning(const bool hasWarning) {
  hasWarning_ = hasWarning;
}


void Test::clearLog() {
  log_.clear();
}


std::string Test::getLog() const {
  return log_.toString();
}


void Test::setDuration(const unsigned int durationInMs) {
  durationInMs_ = durationInMs;
  hasSetDuration_ = true;
}


unsigned int Test::getDuration() const {
  return durationInMs_;
}


void Test::resetDuration() {
  durationInMs_ = std::numeric_limits<unsigned int>::max();
  hasSetDuration_ = false;
}


bool Test::hasSetDuration() const {
  return hasSetDuration_;
}


void Test::setFileName(const std::string& fileName) {
  fileName_ = fileName;
}


std::string Test::getFileName() const {
  return fileName_;
}


bool Test::hasFileName() const {
  return hasFileName_;
}


void Test::setLine(const unsigned int line) {
  line_ = line;
}


unsigned int Test::getLine() const {
  return line_;
}


bool Test::hasLine() const {
  return hasLine_;
}


void Test::log(const std::string& message) {
  log_ << message;
}


bool Test::run() {
  resetDuration();

  log_.begin();

  log_.putLine();

  log_ << name_ << ": " << log_.getStartDateAndTime() << std::endl;

  log_.putLine();

  const auto start = std::chrono::high_resolution_clock::now();


  const bool testResult = test();

  const auto end = std::chrono::high_resolution_clock::now();
  const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();

  log_.putLine();

  log_ << "Time elapsed: " << duration << " milliseconds" << std::endl;
  log_ << name_ << ": " << (testResult ? "PASS" : "FAIL") << std::endl;

  log_.putLine();

  log_.end();

  return testResult;
}
