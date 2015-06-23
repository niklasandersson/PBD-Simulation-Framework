#include "test/LambdaBatchCaller.h"

void LambdaBatchCaller::setBatch(Batch& batch)  {
  batch_ = &batch;
}

LambdaBatchCaller& LambdaBatchCaller::operator, (const std::function<void(void)>&& lambda) {
  lambda();
  TestMacro::removeActiveBatch(batch_);
  return *this;
}