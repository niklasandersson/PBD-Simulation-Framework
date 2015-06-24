#include "test/TestClass.h"

BATCH_CLASS(TestMacroBatch) {

  BATCH(ThrowMessageBatch) {

    class ThrowingClassMock {

    public:
      void throwingFunction() {
        throw std::invalid_argument{"This is a usual throw message."};
      }

    };

    TEST(UsualThrowTest) {
      ThrowingClassMock tcm;
      return EXPECT_THROW(tcm.throwingFunction(), std::invalid_argument);
    };

  };

};