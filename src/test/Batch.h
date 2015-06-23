#ifndef BATCH_H
#define BATCH_H

#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>
#include <vector>
#include <typeinfo>
#include <utility> 

#include "test/Ansi.h"
#include "test/Test.h"

class Batch : public Test {

public:
  using Test::Test;
  virtual ~Batch() = default;

  void add(Test* test);
  void remove(Test* test);

  void clear();

  std::vector<std::pair<Test*, bool> > getTestResults() const;
  bool containsTest(Test* test) const;
  unsigned int getNumberOfTests() const;
  void setDynamicIndent(const unsigned int dynamicIndent);
  unsigned int getDynamicIndent() const;

  bool test() override;
  bool execute();
  bool run() override;

protected:

private:
  std::string colorTestLine(const std::string& line, Test* test) const;

  const std::string name_;
  bool includedBatch_;
  std::vector<Test*> tests_;
  std::vector<std::pair<Test*, bool> > allTestResults_;
  std::vector<std::pair<Test*, bool> > myTestResults_;
  unsigned int dynamicIndent_;

};

#endif // BATCH_H