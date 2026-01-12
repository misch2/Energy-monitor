#include <unity.h>
#include "circular_buffer.h"

void test_circular_buffer_init(void) {
  CircularBuffer<int, 5> buffer;
  TEST_ASSERT_EQUAL(0, buffer.size());
  TEST_ASSERT_TRUE(buffer.isEmpty());
  TEST_ASSERT_FALSE(buffer.isFull());
}

void test_circular_buffer_push_single(void) {
  CircularBuffer<int, 5> buffer;
  buffer.push(10);
  TEST_ASSERT_EQUAL(1, buffer.size());
  TEST_ASSERT_FALSE(buffer.isEmpty());
}

void test_circular_buffer_push_until_full(void) {
  CircularBuffer<int, 5> buffer;
  for (int i = 0; i < 5; i++) {
    buffer.push(i);
  }
  TEST_ASSERT_EQUAL(5, buffer.size());
  TEST_ASSERT_TRUE(buffer.isFull());
}

void test_circular_buffer_overwrite_when_full(void) {
  CircularBuffer<int, 3> buffer;
  buffer.push(1);
  buffer.push(2);
  buffer.push(3);
  buffer.push(4);  // should overwrite 1
  TEST_ASSERT_EQUAL(3, buffer.size());
  TEST_ASSERT_EQUAL(2, buffer[0]);
  TEST_ASSERT_EQUAL(4, buffer[2]);
}

void test_circular_buffer_access_by_index(void) {
  CircularBuffer<int, 5> buffer;
  buffer.push(10);
  buffer.push(20);
  buffer.push(30);
  TEST_ASSERT_EQUAL(10, buffer[0]);
  TEST_ASSERT_EQUAL(20, buffer[1]);
  TEST_ASSERT_EQUAL(30, buffer[2]);
}

void test_circular_buffer_clear(void) {
  CircularBuffer<int, 5> buffer;
  buffer.push(10);
  buffer.push(20);
  buffer.clear();
  TEST_ASSERT_EQUAL(0, buffer.size());
  TEST_ASSERT_TRUE(buffer.isEmpty());
}

void test_circular_buffer_average(void) {
  SmartCircularBuffer<float, 5> buffer;
  buffer.push(10.0f);
  buffer.push(20.0f);
  buffer.push(30.0f);
  buffer.push(40.0f);
  TEST_ASSERT_FLOAT_WITHIN(0.01f, 40.0f, buffer.averageValue(1));
  TEST_ASSERT_FLOAT_WITHIN(0.01f, 30.0f, buffer.averageValue(3));
  TEST_ASSERT_FLOAT_WITHIN(0.01f, 25.0f, buffer.averageValue(4));
  TEST_ASSERT_FLOAT_WITHIN(
      0.01f, 25.0f,
      buffer.averageValue(
          5));  // the same as for 4, since only 4 elements are present

  buffer.push(0.0f);
  TEST_ASSERT_FLOAT_WITHIN(
      0.01f, 20.0f, buffer.averageValue(5));  // now 5 elements are present

  buffer.push(1.0f);
  buffer.push(2.0f);
  TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, buffer.averageValue(3));
}

void test_circular_buffer_max(void) {
  SmartCircularBuffer<int, 4> buffer;
  buffer.push(10);
  buffer.push(20);
  buffer.push(30);
  TEST_ASSERT_EQUAL(30, buffer.maxValue(3));

  buffer.push(5);
  TEST_ASSERT_EQUAL(30, buffer.maxValue(3));
  TEST_ASSERT_EQUAL(5, buffer.maxValue(1));

  buffer.push(4);
  buffer.push(3);
  TEST_ASSERT_EQUAL(5, buffer.maxValue(3));
  TEST_ASSERT_EQUAL(30, buffer.maxValue(5));
}

void test_circular_buffer() {
  UNITY_BEGIN();
  RUN_TEST(test_circular_buffer_init);
  RUN_TEST(test_circular_buffer_push_single);
  RUN_TEST(test_circular_buffer_push_until_full);
  RUN_TEST(test_circular_buffer_overwrite_when_full);
  RUN_TEST(test_circular_buffer_access_by_index);
  RUN_TEST(test_circular_buffer_clear);
  RUN_TEST(test_circular_buffer_average);
  RUN_TEST(test_circular_buffer_max);
  UNITY_END();
}
