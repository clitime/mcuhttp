#include "unity.h"
#include "base64.h"


void setUp(void) {}
void tearDown(void) {}
void suiteSetUp(void) {}
int suiteTearDown(int num_failures) {}


void test_encode(void) {
    const char input[] = "Hello, world";
    char output[16] = {0};
    encode(input, 12, output, 16);
    const char expected[] = "SGVsbG8sIHdvcmxk";
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, output, 16);
}


void test_encode1(void) {
    const char input[] = "H";
    char output[4] = {0};
    encode(input, 1, output, 4);
    const char expected[] = "SA==";
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, output, 4);
}


void test_encode_err_out_len(void) {
    const char input[] = "H";
    char output[3] = {0};
    TEST_ASSERT(encode(input, 1, output, 3) == -1);
}


void test_encode_err_inlen_more_outlen(void) {
    const char input[] = "H";
    char output[4] = {0};
    TEST_ASSERT(encode(input, 4, output, 4) == -2);
}


void test_decode_1(void) {
    const char input[] = "SA";
    char output[3] = {0};
    decode(input, 4, output, 3);
    const char expected[] = "H\0\0";
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, output, 3);
}


void test_decode_hw(void) {
    const char input[] = "SGVsbG8sIHdvcmxk";
    char output[12] = {0};
    decode(input, 16, output, 12);
    const char expected[] = "Hello, world";
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, output, 12);
}
