#ifndef TESTABLECLASS_H
#define TESTABLECLASS_H

#include <string>

class TestAbleClass {

public:

  TestAbleClass(const std::string& name) : name_{name} {}

  virtual ~TestAbleClass() = default;
  virtual void registerTests() const = 0;

  std::string getName() const { return name_; }

private:
  const std::string name_;

};

#endif 