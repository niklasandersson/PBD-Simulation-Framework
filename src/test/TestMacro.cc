#include "test/TestMacro.h"

Batch* TestMacro::activeBatch_;
bool TestMacro::batchIsActive_ = false;
std::vector<Batch*> TestMacro::activeBatches_;


TestMacro::TestMacro(const std::string& name) : Test{name} {
  func_ = std::shared_ptr<std::function<bool(TestLog&)> >{new std::function<bool(TestLog&)>};
}


TestMacro::TestMacro(const std::string& name, const std::string& fileName, const unsigned int line) : Test{name, fileName, line} {
  func_ = std::shared_ptr<std::function<bool(TestLog&)> >{new std::function<bool(TestLog&)>};
}


bool TestMacro::test() {
  log_.testBegin(getName());

  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

  const bool testResult = (*func_)(log_);

  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  unsigned int duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

  setDuration(duration);

  log_.testEnd(testResult);

  return testResult;
}


std::shared_ptr<std::function<bool(TestLog&)> > TestMacro::getFunc() const {
  return func_;
}


void TestMacro::setActiveBatch(Batch& batch) {
  activeBatch_ = &batch;
  batchIsActive_ = true;
  activeBatches_.push_back(&batch);
};


void TestMacro::setActiveBatch(Batch* batch) {
  activeBatch_ = batch;
  batchIsActive_ = true;
  activeBatches_.push_back(batch);
};


void TestMacro::removeActiveBatch(Batch& batch) {
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


void TestMacro::removeActiveBatch(Batch* batch) {
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


void TestMacro::addTestToActiveBatch(Test& test) {
  if( batchIsActive_ && activeBatch_ ) {
    activeBatch_->add(&test);
  }
}


void TestMacro::addTestToActiveBatch(Test* test) {
  if( batchIsActive_ && activeBatch_ ) {
    activeBatch_->add(test);
  }
}