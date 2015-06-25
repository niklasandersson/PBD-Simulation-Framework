#include "test/TestClass.h"

BATCH_CLASS(TestMacroBatch) {



  BATCH(ThrowMessageBatch) {

    class ThrowingClassMock {

    public:
      void throwingFunction() {
        throw std::domain_error{"This is a usual throw message."};
      }
      void intThrowingFunction() {
        throw 1337;
      }

      void cStringThrowingFunction() {
        throw "throwing a cstring exception";
      }

      void stdStringThrowingFunction() {
        throw std::string{"throwing a std::string exception"};
      }

      void charThrowingFunction() {
        throw 'a';
      }

      void nonThrowingFunction() {
        
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
      LOG_EXPECT_WARNING(log_);
      return EXPECT_THROW_MESSAGE_WARNING(tcm.throwingFunction(), std::domain_error, "This is not a usual throw message.") == false;
    };

    TEST(MessageThrowTest3) {
      ThrowingClassMock tcm;
      return EXPECT_THROW(EXPECT_THROW_MESSAGE(tcm.throwingFunction(), std::domain_error, "This is not a usual throw message."), std::invalid_argument);
    };

    TEST(MessageThrowTest4) {
      ThrowingClassMock tcm;
      return EXPECT_THROW_MESSAGE(tcm.throwingFunction(), std::invalid_argument, "This is not a usual throw message.");
    };

    TEST(MessageThrowTest5) {
      ThrowingClassMock tcm;
      return EXPECT_ANY_THROW(tcm.intThrowingFunction());
    };

    TEST(MessageThrowTest6) {
      ThrowingClassMock tcm;
      return EXPECT_THROW_MESSAGE(tcm.intThrowingFunction(), std::invalid_argument, "This is not a usual throw message.");
    };

    TEST(MessageThrowTest7) {
      ThrowingClassMock tcm;
      tcm.cStringThrowingFunction();
      return true;
    };

    TEST(MessageThrowTest8) {
      ThrowingClassMock tcm;
      return EXPECT_THROW_MESSAGE(tcm.cStringThrowingFunction(), std::invalid_argument, "This is not a usual throw message.");
    };

    TEST(MessageThrowTest9) {
      ThrowingClassMock tcm;
      return EXPECT_ANY_THROW(tcm.stdStringThrowingFunction());
    };

    TEST(MessageThrowTest10) {
      ThrowingClassMock tcm;
      return EXPECT_THROW_MESSAGE(tcm.stdStringThrowingFunction(), std::invalid_argument, "This is not a usual throw message.");
    };

    TEST(MessageThrowTest11) {
      ThrowingClassMock tcm;
      return EXPECT_THROW_MESSAGE(tcm.charThrowingFunction(), std::invalid_argument, "This is not a usual throw message.");
    };

    TEST(MessageThrowTest12) {
      ThrowingClassMock tcm;
      return EXPECT_ANY_THROW(tcm.nonThrowingFunction());
    };

  };



  BATCH(XorBatch) {

    TEST(XorTest1) {
      const bool a = true;
      const bool b = false;
      return XOR(a, b) == (a ^ b);
    };

    TEST(XorTest2) {
      const bool a = false;
      const bool b = true;
      return XOR(a, b) == (a ^ b);
    };

    TEST(XorTest3) {
      const bool a = false;
      const bool b = false;
      LOG_EXPECT_WARNING(log_);
      return XOR_WARNING(a, b) == (a ^ b);
    };

    TEST(XorTest4) {
      const bool a = true;
      const bool b = true;
      LOG_EXPECT_WARNING(log_);
      return XOR_WARNING(a, b) == (a ^ b);
    };

  };



};