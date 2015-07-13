// #include "test/TestClass.h"

// BATCH_CLASS(batch3) {

//   ADD_BATCH_CLASS_FROM_OTHER_FILE(batch1);

//   TEST(successTest) {
//     return true; 
//   };

//   BATCH(randomBatch1) {

//     BATCH(randomBatch2) {

//       TEST(successTest2) {
//         return true; 
//       };

//       TEST(warningTest) {
//         return EQUAL_WARNING(1, 2) == false; 
//       };

//       TEST(failingTest) {
//         return false; 
//       };

//     };

//   };

//   ADD_BATCH_CLASS_FROM_OTHER_FILE(batch2);

// };
