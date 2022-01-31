#include "unity.h"
#include "http_header_parser.h"
#include <string.h>


void setUp(void) {}
void tearDown(void) {}
void suiteSetUp(void) {}
int suiteTearDown(int num_failures) {}


void test_content_length() {
    char *headers = "Connection: keep-alive\r\nContent-Length: \t254\r\n";
    struct buffer buf = {
        .data = headers,
        .len = strlen(headers)
    };
    struct headers const hdrs = headerParser(&buf);
    TEST_ASSERT_EQUAL_UINT16(254, hdrs.content_lenght);
}
