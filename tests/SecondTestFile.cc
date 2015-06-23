#include "test/TestClass.h"

BATCH_CLASS(batch2) {

  class ShouldBeTested {
    
  public:

    int add(int a, int b) { return a + b; }

    int sub(int a, int b) { return a - b; }

    int mult(int a, int b) { return a * b; }

    float div(int a, int b) { return a / (float)b; }

    void count() {
      throw std::out_of_range("My error message.");
    }

  };


  TEST(sbtTest1) {
    ShouldBeTested sbt;
    return EQUAL(sbt.add(3, 3), 6);
  };

  TEST(sbtTest2) {
    ShouldBeTested sbt;
    return NOT_EQUAL(sbt.add(3, 4), 6);
  };

  TEST(sbtTest3) {
    ShouldBeTested sbt;
    return GREATER(sbt.mult(3, 4), 1);
  };

  TEST(sbtTest4) {
    ShouldBeTested sbt;
    return LESS(sbt.mult(2, 2), 5);
  };

  TEST(sbtTest5) {
    ShouldBeTested sbt;
    return EQUAL_WARNING(sbt.mult(2, 2), 1) == false;
  };

  TEST(sbtTest6) {
    ShouldBeTested sbt;
    return EXPECT_ANY_THROW(EQUAL(sbt.add(3, 5), 6));
  };

  TEST(sbtTest7) {
    ShouldBeTested sbt;
    return EXPECT_THROW(EQUAL(sbt.add(3, 5), 6), std::invalid_argument);
  };


  BATCH(includeOtherBatchClass) {

    TEST(includeTest) {
      log_ << "Nice include test" << std::endl;
      return true;
    };

    ADD_BATCH_CLASS_FROM_OTHER_FILE(batch1);

  };


};