#ifndef LAMBDABATCHCALLER_H
#define LAMBDABATCHCALLER_H

#include <functional>

#include "test/Batch.h"
#include "test/TestMacro.h"

class LambdaBatchCaller {

public:

  void setBatch(Batch& batch);
  LambdaBatchCaller& operator, (const std::function<void(void)>&& lambda);

private:
  Batch* batch_;

};

#endif