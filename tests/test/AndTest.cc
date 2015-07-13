#include "test/TestClass.h"

BATCH_CLASS(AndBatch) {



  TEST(AndBatchTest0) {
    return AND(true, true);
  };

  TEST(AndBatchTest1) {
    return AND(true, true, true, true, true, true, true, true, true, true);
  };



  BATCH(AndThrowBatch) {

    TEST(AndBatchTest2) {
      return EXPECT_ANY_THROW( AND(true, false) );
    };

    TEST(AndBatchTest3) {
      return EXPECT_ANY_THROW( AND(false, true) );
    };

    TEST(AndBatchTest4) {
      return EXPECT_ANY_THROW( AND(true, true, true, true, false) );
    };

    TEST(AndBatchTest5) {
      return EXPECT_ANY_THROW( AND(true, true, false, true, true) );
    };

    TEST(AndBatchTest6) {
      return EXPECT_ANY_THROW( AND(false, true, true, true, true) );
    };


    TEST(AndBatchTest11) {
      return EXPECT_THROW( AND(true, false, true, true, true, true), std::invalid_argument );
    };

  };
  


  BATCH(AndWarningBatch) {

    TEST(AndBatchTest7) {
      LOG_EXPECT_WARNING(log_);
      return AND_WARNING(true, false) == false;
    };

    TEST(AndBatchTest8) {
      LOG_EXPECT_WARNING(log_);
      return AND_WARNING(true, true, false, true, true) == false;
    };

    TEST(AndBatchTest9) {
      LOG_EXPECT_WARNING(log_);
      return AND_WARNING(false, true, true, true, true) == false;
    };

    TEST(AndBatchTest10) {
      LOG_EXPECT_WARNING(log_);
      return AND_WARNING(true, true, true, true, false) == false;
    };

  };



};