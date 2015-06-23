#include "test/TestMacro.h"

Batch* TestMacro::activeBatch_;
bool TestMacro::batchIsActive_ = false;
std::vector<Batch*> TestMacro::activeBatches_;