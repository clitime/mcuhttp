#include "unity.h"
#include "http_body_parser.h"


void setUp(void) {}
void tearDown(void) {}
void suiteSetUp(void) {}
int suiteTearDown(int num_failures) {}


void test_null_size() {
    TEST_ASSERT_EQUAL_UINT8(0, get_body_size(0));
    TEST_ASSERT_EQUAL_UINT8(0, get_body_size(""));
}


void test_any_size() {
    TEST_ASSERT_EQUAL_UINT8(5, get_body_size("abcsd"));
    TEST_ASSERT_EQUAL_UINT8(15, get_body_size("abcsd1234567890"));
}
