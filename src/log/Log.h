#ifndef LOG_H
#define LOG_H


#include <iostream>
#include <ctime> 
#include <iomanip>
#include <sstream>
#include <fstream>
#include <type_traits>


class Log {

  typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
  typedef CoutType& (*StandardEndLine)(CoutType&);
  typedef Log& (*LogManipulator)(Log&);

public:
  Log();
  ~Log() = default;

  std::string getDateAndTime() const;
  std::string getStartDateAndTime() const;
  
  template <typename T>
  Log& operator<<(const T& x) {
    if( enabled_ ) {
      log_ << x;
      if( printToCout_ ) {
        std::cout << x;
      }
    }
    return *this;
  }
 
  Log& operator<<(StandardEndLine manip);
  Log& operator<<(LogManipulator manip);

  static Log& line(Log& log);

  void setEnabled(const bool enabled);
  void setName(const std::string name);
  void setPath(const std::string path);
  void setFileTag(const std::string fileTag);
  void setFileName(const std::string fileName);
  void enablePrintToCout();
  void disablePrintToCout();

  std::string toString() const;

  void putLine(const char c = '-');

  void clear();
  void begin();
  void end();

  void write(std::string fileName);
  void write();

protected:

private:
  void constructFileName();
  bool printToCout_;
  unsigned int tabCounter_;

  bool enabled_;
  std::string startDateAndTime_;
  std::ostringstream log_;
  std::string name_;
  std::string path_;
  std::string fileTag_;
  std::string fileName_;

};


#endif // LOG_H