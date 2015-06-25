#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>

#include "log/TestLog.h"

#include "test/TestManager.h"
#include "test/TestAbleClass.h"

#include "test/Batch.h"
#include "test/Test.h"
#include "test/TestMacro.h"
#include "test/LambdaBatchCaller.h"



#define REGISTER_CLASS(name) \
  class name : public TestAbleClass { \
 \
  public: \
 \  
    template<typename T> \
    name(const T&& lambda) : TestAbleClass{#name} { \
      if( !once_ ) { \
        TestManager::getInstance().add(this); \
        once_ = true; \
        func_ = std::move(lambda); \
      } \
    } \
 \
    void registerTests() const { \
      func_(); \
    } \
 \
  protected: \
 \
  private: \
    bool once_{false}; \
    std::function<void(void)> func_; \
 \
  }; \
  static name name##Instance = [](void) -> void



#define BATCH_CLASS(name) \
  Batch name(#name, __FILE__, __LINE__); \
  class name##ClassType : public TestAbleClass { \
 \
  public: \
 \
    template<typename T> \
    name##ClassType(const T&& lambda) : TestAbleClass{#name} { \
      if( !once_ ) { \
        TestManager::getInstance().add(this); \
        once_ = true; \
        func_ = std::move(lambda); \
      } \
    } \
 \
    void registerTests() const { \
      TestMacro::addTestToActiveBatch(name); \
      TestMacro::setActiveBatch(name); \
      func_(); \
      TestMacro::removeActiveBatch(name); \
    } \
 \
  protected: \
 \
  private: \
    bool once_{false}; \
    std::function<void(void)> func_; \
 \
  }; \
  static name##ClassType name##ClassType##Instance = [](void) -> void



#define TEST_CLASS(name) \
  TestMacro name{#name, __FILE__, __LINE__}; \
  class name##ClassType { \
 \
  public: \
 \
    template<typename T> \
    name##ClassType(const T&& lambda) { \
      if( !once_ ) { \
        once_ = true; \
        func_ = std::move(lambda); \
        *name.getFunc() = func_; \
      } \
    } \
 \
  protected: \
 \
  private: \
    bool once_{false}; \
    std::function<bool(TestLog&)> func_; \
 \
  }; \
  static name##ClassType name##ClassType##Instance = [&](TestLog& log_) -> bool




#endif


