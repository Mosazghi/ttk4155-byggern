#include <stdint.h>

#include "unity.h"
#include "utility.h"

// Unity setup and teardown
void setUp(void) {
  // Set up before each test
}

void tearDown(void) {
  // Clean up after each test
}

// ============================================================================
// CLAMP Tests
// ============================================================================

void test_clamp_value_below_min(void) {
  int result = CLAMP(-5, 0, 10);
  TEST_ASSERT_EQUAL_INT(0, result);
}

void test_clamp_value_above_max(void) {
  int result = CLAMP(15, 0, 10);
  TEST_ASSERT_EQUAL_INT(10, result);
}

void test_clamp_value_within_range(void) {
  int result = CLAMP(5, 0, 10);
  TEST_ASSERT_EQUAL_INT(5, result);
}

void test_clamp_value_at_min(void) {
  int result = CLAMP(0, 0, 10);
  TEST_ASSERT_EQUAL_INT(0, result);
}

void test_clamp_value_at_max(void) {
  int result = CLAMP(10, 0, 10);
  TEST_ASSERT_EQUAL_INT(10, result);
}

void test_clamp_negative_range(void) {
  int result = CLAMP(-5, -10, -1);
  TEST_ASSERT_EQUAL_INT(-5, result);
}

void test_clamp_equal_min_max(void) {
  int result = CLAMP(5, 10, 10);
  TEST_ASSERT_EQUAL_INT(10, result);
}

void test_clamp_uint8_overflow(void) {
  uint8_t result = CLAMP(300, 0, 255);
  TEST_ASSERT_TRUE(result <= 255);
}

// ============================================================================
// ABS Tests
// ============================================================================

void test_abs_positive_number(void) {
  int result = ABS(5);
  TEST_ASSERT_EQUAL_INT(5, result);
}

void test_abs_negative_number(void) {
  int result = ABS(-5);
  TEST_ASSERT_EQUAL_INT(5, result);
}

void test_abs_zero(void) {
  int result = ABS(0);
  TEST_ASSERT_EQUAL_INT(0, result);
}

void test_abs_large_negative(void) {
  int result = ABS(-1000);
  TEST_ASSERT_EQUAL_INT(1000, result);
}

// ============================================================================
// ARR_LEN Tests
// ============================================================================

void test_arr_len_int_array(void) {
  int arr[] = {1, 2, 3, 4, 5};
  size_t len = ARR_LEN(arr);
  TEST_ASSERT_EQUAL_size_t(5, len);
}

void test_arr_len_char_array(void) {
  char arr[] = {'a', 'b', 'c'};
  size_t len = ARR_LEN(arr);
  TEST_ASSERT_EQUAL_size_t(3, len);
}

void test_arr_len_uint8_array(void) {
  uint8_t arr[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
  size_t len = ARR_LEN(arr);
  TEST_ASSERT_EQUAL_size_t(7, len);
}

void test_arr_len_single_element(void) {
  int arr[] = {42};
  size_t len = ARR_LEN(arr);
  TEST_ASSERT_EQUAL_size_t(1, len);
}

// ============================================================================
// Main test runner
// ============================================================================

int main(void) {
  UNITY_BEGIN();

  // CLAMP tests
  RUN_TEST(test_clamp_value_below_min);
  RUN_TEST(test_clamp_value_above_max);
  RUN_TEST(test_clamp_value_within_range);
  RUN_TEST(test_clamp_value_at_min);
  RUN_TEST(test_clamp_value_at_max);
  RUN_TEST(test_clamp_negative_range);
  RUN_TEST(test_clamp_equal_min_max);
  RUN_TEST(test_clamp_uint8_overflow);

  // ABS tests
  RUN_TEST(test_abs_positive_number);
  RUN_TEST(test_abs_negative_number);
  RUN_TEST(test_abs_zero);
  RUN_TEST(test_abs_large_negative);

  // ARR_LEN tests
  RUN_TEST(test_arr_len_int_array);
  RUN_TEST(test_arr_len_char_array);
  RUN_TEST(test_arr_len_uint8_array);
  RUN_TEST(test_arr_len_single_element);

  return UNITY_END();
}
