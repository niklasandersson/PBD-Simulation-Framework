#include "test/TestClass.h"

BATCH_CLASS(TestMacroBatch) {

  BATCH(ThrowMessageBatch) {

    class ThrowingClassMock {

    public:
      void throwingFunction() {
        throw std::domain_error{"This is a usual throw message."};
      }

    };

    TEST(UsualThrowTest) {
      ThrowingClassMock tcm;
      return EXPECT_THROW(tcm.throwingFunction(), std::domain_error);
    };

    TEST(MessageThrowTest1) {
      ThrowingClassMock tcm;
      return EXPECT_THROW_MESSAGE_WARNING(tcm.throwingFunction(), std::domain_error, "This is a usual throw message.");
    };

    TEST(MessageThrowTest2) {
      ThrowingClassMock tcm;
      log_ << "There should be a warning here:" << std::endl;
      return EXPECT_THROW_MESSAGE_WARNING(tcm.throwingFunction(), std::domain_error, "This is not a usual throw message.") == false;
    };

    TEST(MessageThrowTest3) {
      ThrowingClassMock tcm;
      return EXPECT_THROW(EXPECT_THROW_MESSAGE(tcm.throwingFunction(), std::domain_error, "This is not a usual throw message."), std::invalid_argument);
    };

  };

};