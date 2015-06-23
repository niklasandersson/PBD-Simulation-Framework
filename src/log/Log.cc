#include "log/Log.h"


Log::Log() {
  enabled_ = true;
  printToCout_ = true;
  path_ = "logs/";
  name_ = "Log";
  tabCounter_ = 0;
}


std::string Log::getDateAndTime() const {
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", timeinfo);
  std::string dateAndTime(buffer);

  return dateAndTime;
}


std::string Log::getStartDateAndTime() const {
  return startDateAndTime_;
}


std::string Log::getFileTag() const {
  return "txt";
}


Log& Log::operator<<(StandardEndLine manip) {
  if( enabled_ ) {
    log_ << std::endl;
    if( printToCout_ ) {
      manip(std::cout); 
    }
  }
  return *this;
}


Log& Log::operator<<(LogManipulator manip) {
  return manip(*this);
}


Log& Log::line(Log& log) {
  log.putLine();
  return log;
}


void Log::setEnabled(const bool enabled) {
  enabled_ = enabled;
}


void Log::setName(const std::string name) {
  name_ = name;
}


void Log::setPath(const std::string path) {
  path_ = path;
}


void Log::putLine(const char c) {
  *this << std::setfill(c) << std::setw(80) << c << std::endl;
}


void Log::clear() {
  log_.str("");
  log_.clear();
}


void Log::begin() {
  clear();
  startDateAndTime_ = getDateAndTime();
}


std::string Log::constructFileName() {
  std::string fileName = path_;
  fileName.append(name_);
  fileName.append("_");
  fileName.append(startDateAndTime_);
  fileName.append(std::string{"."} + getFileTag());
  return fileName;
}


void Log::end() {
  std::string fileName = constructFileName();
  std::ofstream output;
  try {
    output.open(fileName.c_str());
    output << log_.str();
    output.close();
  } catch (std::ifstream::failure e) {
    std::cerr << "Exception opening/writing/closing log file: " << fileName << std::endl;
  }
}


std::string Log::toString() const {
  return log_.str();
}


void Log::enablePrintToCout() {
  printToCout_ = true;
}


void Log::disablePrintToCout() {
  printToCout_ = false;
}