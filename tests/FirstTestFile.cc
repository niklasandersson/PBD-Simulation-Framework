#include "test/TestClass.h"

BATCH_CLASS(batch1) {

  TEST(test1) {
    log_ << "SUCCESS!!!" << std::endl;
    return true; 
  };

  TEST(test2) {
    log_ << "Hej!" << std::endl;
    return true; 
  };

  TEST(test3) {
    log_ << "Ett test!!!" << std::endl;
    return true; 
  };
  
  TEST(test4) {
    log_ << "Min test log detta." << std::endl;
    return true; 
  };

};
