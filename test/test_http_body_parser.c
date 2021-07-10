#include "unity.h"
#include "http_body_parser.h"


void setUp(void) {}
void tearDown(void) {}
void suiteSetUp(void) {}
int suiteTearDown(int num_failures) {}


void test_null_size() {
    TEST_ASSERT_EQUAL_UINT8(0, getBodySize(0));
    TEST_ASSERT_EQUAL_UINT8(0, getBodySize(""));
}


void test_any_size() {
    TEST_ASSERT_EQUAL_UINT8(5, getBodySize("abcsd"));
    TEST_ASSERT_EQUAL_UINT8(15, getBodySize("abcsd1234567890"));
}
