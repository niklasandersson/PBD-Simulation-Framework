#ifndef TESTMACRO_H
#define TESTMACRO_H

#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
#include <typeinfo>

#include "test/Test.h"
#include "test/Batch.h"
#include "test/LambdaBatchCaller.h"
#include "test/Ansi.h"


class TestMacro : public Test {

public:
  TestMacro(const std::string& name) : Test{name} {
    func_ = std::shared_ptr<std::function<bool(TestLog&)> >{new std::function<bool(TestLog&)>};
  }

  TestMacro(const std::string& name, const std::string& fileName, const unsigned int line) : Test{name, fileName, line} {
    func_ = std::shared_ptr<std::function<bool(TestLog&)> >{new std::function<bool(TestLog&)>};
  }

  virtual ~TestMacro() = default;

  virtual bool test() override {
    log_.testBegin(getName());

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    const bool testResult = (*func_)(log_);

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    unsigned int duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    setDuration(duration);

    log_.testEnd(testResult);

    return testResult;
  }

  std::shared_ptr<std::function<bool(TestLog&)> > getFunc() const {
    return func_;
  }

  static void setActiveBatch(Batch& batch) {
    activeBatch_ = &batch;
    batchIsActive_ = true;
    activeBatches_.push_back(&batch);
  };

  static void setActiveBatch(Batch* batch) {
    activeBatch_ = batch;
    batchIsActive_ = true;
    activeBatches_.push_back(batch);
  };

  static void removeActiveBatch(Batch& batch) {
    if( &batch == activeBatch_ ) {
      activeBatches_.pop_back();
      if( !activeBatches_.empty() ) {
        activeBatch_ = activeBatches_.back();
      } else {
        batchIsActive_ = false;
        activeBatch_ = nullptr;
      }
    } 
  };

  static void removeActiveBatch(Batch* batch) {
    if( batch == activeBatch_ ) {
      activeBatches_.pop_back();
      if( !activeBatches_.empty() ) {
        activeBatch_ = activeBatches_.back();
      } else {
        batchIsActive_ = false;
        activeBatch_ = nullptr;
      }
    } 
  };

  static void addTestToActiveBatch(Test& test) {
    if( batchIsActive_ && activeBatch_ ) {
      activeBatch_->add(&test);
    }
  }

  static void addTestToActiveBatch(Test* test) {
    if( batchIsActive_ && activeBatch_ ) {
      activeBatch_->add(test);
    }
  }

protected:
  std::shared_ptr<std::function<bool(TestLog&)> > func_;

private:
  static Batch* activeBatch_;
  static bool batchIsActive_;
  static std::vector<Batch*> activeBatches_;

};



#define TEST(name) \
  static TestMacro name{#name, __FILE__, __LINE__}; \
  TestMacro::addTestToActiveBatch(name); \
  *name.getFunc() = [&](TestLog& log_) -> bool



#define INLINE_BATCH(name) \
  static Batch name(#name, __FILE__, __LINE__); \
  TestMacro::addTestToActiveBatch(name); \
  TestMacro::setActiveBatch(name); \
  LambdaBatchCaller name##LambdaBatchCaller; \
  name##LambdaBatchCaller.setBatch(name); \
  name##LambdaBatchCaller, [] () -> void 

#define BATCH(name) \
  INLINE_BATCH(name)



#define BEGIN_BATCH(name) \
  static Batch name(#name, __FILE__, __LINE__); \
  TestMacro::addTestToActiveBatch(name); \
  TestMacro::setActiveBatch(name)

#define END_BATCH(name) \
  TestMacro::removeActiveBatch(name)



#define ADD_BATCH(name) \
  TestMacro::addTestToActiveBatch(name)

#define ADD_BATCH_CLASS_FROM_OTHER_FILE(name) \
  extern Batch name; \
  ADD_BATCH(name)



#define RUN_BATCH(name) \
  name.run()

#define RUN_TEST(name) \
  name.run()


#define ERROR_PREFIX "Error: "
#define WARNING_PREFIX "Warning: "



#define LOG_EXPECT_WARNING(log) \
  log << "Expect a warning here:" << std::endl



#define LOG_FILE_LINE_IMPL(output) \
  output << "@ " << __FILE__ << " " << __LINE__



#define LOG_ERROR_FOR_ONE_ARG_IMPL(macroStr, result, output, argStr, a, descriptionStr) \
  if( !result ) { \
    std::ostringstream os; \
    os << macroStr << "(" << argStr << ")" << " (" << a << ") " << descriptionStr << std::endl; \
    LOG_FILE_LINE_IMPL(os); \
    throw std::invalid_argument(os.str()); \
  } 

#define LOG_WARNING_FOR_ONE_ARG_IMPL(macroStr, result, output, argStr, a, descriptionStr) \
  if( !result ) { \
    output << WARNING_PREFIX << macroStr << "_WARNING(" << argStr << ")" << " (" << a << ") " << descriptionStr << std::endl; \
    LOG_FILE_LINE_IMPL(output); \
    output << std::endl; \
  } 

#define LOG_ERROR_FOR_TWO_ARGS_IMPL(macroStr, result, output, arg1Str, a1, descriptionStr, arg2Str, a2) \
  if( !result ) { \
    std::ostringstream os; \
    os << macroStr << "(" << arg1Str << ", " << arg2Str << ") is not evaluated to true, since "; \
    os << "'" << arg1Str << "' (" << a1 << ") " << descriptionStr << " '" << arg2Str << "' (" << a2 << ")" << std::endl; \
    LOG_FILE_LINE_IMPL(os); \
    throw std::invalid_argument(os.str()); \
  }

#define LOG_WARNING_FOR_TWO_ARGS_IMPL(macroStr, result, output, arg1Str, a1, descriptionStr, arg2Str, a2) \
  if( !result ) { \
    output << WARNING_PREFIX << macroStr << "_WARNING(" << arg1Str << ", " << arg2Str << ") is not evaluated to true, since "; \
    output << "'" << arg1Str << "' (" << a1 << ") " << descriptionStr << " '" << arg2Str << "' (" << a2 << ")" << std::endl; \
    LOG_FILE_LINE_IMPL(output); \
    output << std::endl; \
  }



#define ASSERT_IMPL(expression, log) \
  [&]() -> bool { \
    const auto a = (expression); \
    const bool result = a; \
    log("ASSERT", result, log_, #expression, a, "is not evaluated to true"); \
    return result; \
  }() 

#define ASSERT(expression) \
  ASSERT_IMPL(expression, LOG_ERROR_FOR_ONE_ARG_IMPL)

#define ASSERT_WARNING(expression) \ 
  ASSERT_IMPL(expression, LOG_WARNING_FOR_ONE_ARG_IMPL)


#define EXPECT_ANY_THROW_IMPL(macroStr, expression, prefixStr) \
  [&]() -> bool { \
    bool result = false; \
    try { \
      expression; \
    } catch(...) { \
      result = true; \
    } \
    if( !result ) { \
      std::ostringstream os; \
      os << macroStr << "(" << #expression << ") did not throw any exception" << std::endl; \
      LOG_FILE_LINE_IMPL(os); \
      if( prefixStr == WARNING_PREFIX ) { \
        log_ << prefixStr << os.str() << std::endl; \
      } else { \
        throw std::invalid_argument{os.str()}; \
      } \
    } \
    return result; \
  }()

#define EXPECT_THROW_IMPL(macroStr, expression, expectedException, prefixStr) \
  [&]() -> bool { \
    bool result = false; \
    try { \
      expression; \
    } catch(const expectedException& e) { \
      result = true; \
    } catch(const std::exception& e) { \
      std::ostringstream os; \
      os << macroStr << "(" << #expression << ", " << #expectedException << ") did not even throw the expected exception"; \
      os << ", another faulty exception was thrown ("  << typeid(e).name() << ")" << std::endl; \
      LOG_FILE_LINE_IMPL(os); \
      log_ << prefixStr << os.str() << std::endl; \
      throw; \
    } \
    if( !result ) { \
      std::ostringstream os; \
      os << macroStr << "(" << #expression << ", " << #expectedException << ") did not throw the expected exception" << std::endl; \
      LOG_FILE_LINE_IMPL(os); \
      if( prefixStr == WARNING_PREFIX ) { \
        log_ << prefixStr << os.str() << std::endl; \
      } else { \
        throw std::invalid_argument{os.str()}; \
      } \
    } \
    return result; \
  }()

#define EXPECT_THROW_MESSAGE_IMPL(macroStr, expression, expectedException, prefixStr, messageStr) \
  [&]() -> bool { \
    bool rightExceptionThrown = false; \
    bool rightMessage = false; \
    std::string thrownMessage = ""; \
    try { \
      expression; \
    } catch(const expectedException& e) { \
      rightExceptionThrown = true; \
      thrownMessage = e.what(); \
      if( messageStr == thrownMessage ) { \
        rightMessage = true; \
      } \
    } catch(const std::exception& e) { \
      std::ostringstream os; \
      os << macroStr << "(" << #expression << ", " << #expectedException << ", " << #messageStr << ") did not even throw the expected exception so that the throw messages could not even be compared"; \
      os << ", another faulty exception was thrown ("  << typeid(e).name() << ")" << std::endl; \
      LOG_FILE_LINE_IMPL(os); \
      log_ << prefixStr << os.str() << std::endl; \
      throw; \
    } \
    const bool result = rightExceptionThrown && rightMessage; \
    if( !result ) { \
      std::ostringstream os; \
      if( rightExceptionThrown ) { \
        os << macroStr << "(" << #expression << ", " << #expectedException << ", " << #messageStr << ") did throw the expected exception but with the wrong message '"; \
        os << thrownMessage << "'" << std::endl; \
      } else { \
        os << macroStr << "(" << #expression << ", " << #expectedException << ", " << #messageStr << ") did not even throw the expected exception so that the throw messages could not even be compared"; \
        os << std::endl; \
      } \
      LOG_FILE_LINE_IMPL(os); \
      if( prefixStr == WARNING_PREFIX ) { \
        log_ << prefixStr << os.str() << std::endl; \
      } else { \
        throw std::invalid_argument{os.str()}; \
      } \
    } \
    return result; \
  }()

#define EXPECT_ANY_THROW(expression) \
  EXPECT_ANY_THROW_IMPL("EXPECT_ANY_THROW", expression, ERROR_PREFIX)

#define EXPECT_THROW(expression, stdException) \
  EXPECT_THROW_IMPL("EXPECT_THROW", expression, stdException, ERROR_PREFIX)

#define EXPECT_THROW_MESSAGE(expression, stdException, messageStr) \
  EXPECT_THROW_MESSAGE_IMPL("EXPECT_THROW_MESSAGE", expression, stdException, ERROR_PREFIX, messageStr)  

#define EXPECT_ANY_THROW_WARNING(expression) \
  EXPECT_ANY_THROW_IMPL("EXPECT_ANY_THROW_WARNING", expression, WARNING_PREFIX)

#define EXPECT_THROW_WARNING(expression, stdException) \
  EXPECT_THROW_IMPL("EXPECT_THROW_WARNING", expression, stdException, WARNING_PREFIX)

#define EXPECT_THROW_MESSAGE_WARNING(expression, stdException, messageStr) \
  EXPECT_THROW_MESSAGE_IMPL("EXPECT_THROW_MESSAGE_WARNING", expression, stdException, WARNING_PREFIX, messageStr)  



#define EQUAL_IMPL(arg1, arg2, log) \
  [&]() -> bool { \
    const auto a1 = arg1; \
    const auto a2 = arg2; \
    const bool result = a1 == a2; \
    log("EQUAL", result, log_, #arg1, a1, "does not equal", #arg2, a2) \
    return result; \
  }() 

#define EQUAL(arg1, arg2) \
  EQUAL_IMPL(arg1, arg2, LOG_ERROR_FOR_TWO_ARGS_IMPL)

#define EQUAL_WARNING(arg1, arg2) \
  EQUAL_IMPL(arg1, arg2, LOG_WARNING_FOR_TWO_ARGS_IMPL)



#define NOT_EQUAL_IMPL(arg1, arg2, log) \
  [&]() -> bool { \
    const auto a1 = arg1; \
    const auto a2 = arg2; \
    const bool result = a1 != a2; \
    log("NOT_EQUAL", result, log_, #arg1, a1, "equals", #arg2, a2) \
    return result; \
  }()

#define NOT_EQUAL(arg1, arg2) \
  NOT_EQUAL_IMPL(arg1, arg2, LOG_ERROR_FOR_TWO_ARGS_IMPL)

#define NOT_EQUAL_WARNING(arg1, arg2) \
  NOT_EQUAL_IMPL(arg1, arg2, LOG_WARNING_FOR_TWO_ARGS_IMPL)



#define GREATER_IMPL(arg1, arg2, log) \
  [&]() -> bool { \
    const auto a1 = arg1; \
    const auto a2 = arg2; \
    const bool result = a1 > a2; \
    log("GREATER", result, log_, #arg1, a1, "is not greater than", #arg2, a2) \
    return result; \
  }() 

#define GREATER(arg1, arg2) \
  GREATER_IMPL(arg1, arg2, LOG_ERROR_FOR_TWO_ARGS_IMPL)

#define GREATER_WARNING(arg1, arg2) \
  GREATER_IMPL(arg1, arg2, LOG_WARNING_FOR_TWO_ARGS_IMPL)



#define LESS_IMPL(arg1, arg2, log) \
  [&]() -> bool { \
    const auto a1 = arg1; \
    const auto a2 = arg2; \
    const bool result = a1 < a2; \
    log("LESS", result, log_, #arg1, a1, "is not less than", #arg2, a2) \
    return result; \
  }()

#define LESS(arg1, arg2) \
  LESS_IMPL(arg1, arg2, LOG_ERROR_FOR_TWO_ARGS_IMPL)

#define LESS_WARNING(arg1, arg2) \
  LESS_IMPL(arg1, arg2, LOG_WARNING_FOR_TWO_ARGS_IMPL)



template<typename T>
T ander(T t) {
  return t;
}

template<typename T, typename... Args>
T ander(T first, Args... args) {
  return first &= ander(args...);
}

#define AND_IMPL(log, ...) \
  [&]() -> bool { \
    const auto a = ander(__VA_ARGS__); \
    const bool result = a; \
    log("AND", result, log_, #__VA_ARGS__, a, "is not evaluated to true"); \
    return result; \
  }()

#define AND(...) \
  AND_IMPL(LOG_ERROR_FOR_ONE_ARG_IMPL, __VA_ARGS__)
  
#define AND_WARNING(...) \
  AND_IMPL(LOG_WARNING_FOR_ONE_ARG_IMPL, __VA_ARGS__)
  


template<typename T>
T orer(T t) {
  return t;
}

template<typename T, typename... Args>
T orer(T first, Args... args) {
  return first |= ander(args...);
}

#define OR_IMPL(log, ...) \
  [&]() -> bool { \
    const auto a = orer(__VA_ARGS__); \
    const bool result = a; \
    log("OR", result, log_, #__VA_ARGS__, a, "is not evaluated to true"); \
    return result; \
  }()

#define OR(...) \
  OR_IMPL(LOG_ERROR_FOR_ONE_ARG_IMPL, __VA_ARGS__)

#define OR_WARNING(...) \
  OR_IMPL(LOG_WARNING_FOR_ONE_ARG_IMPL, __VA_ARGS__)



#define XOR_IMPL(arg1, arg2, log) \
  [&]() -> bool { \
    const auto a1 = arg1; \
    const auto a2 = arg2; \
    const bool result = a1 ^ a2; \
    log("XOR", result, log_, #arg1, a1, "is not the opposite to", #arg2, a2) \
    return result; \
  }()

#define XOR(arg1, arg2) \
  XOR_IMPL(arg1, arg2, LOG_ERROR_FOR_TWO_ARGS_IMPL)

#define XOR_WARNING(arg1, arg2) \
  XOR_IMPL(arg1, arg2, LOG_WARNING_FOR_TWO_ARGS_IMPL)


#endif