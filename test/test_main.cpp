#include "unity.h"

extern void test_circular_buffer();

void setUp(void) {}

void tearDown(void) {}

void test_hello_world(void) {
  TEST_ASSERT_EQUAL(1 + 1, 2);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_hello_world);
  test_circular_buffer();

  return UNITY_END();
}