#include "unity.h"
#include "http_cgi_request.h"
#include "mock_http_cgi_function_list.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}
void suiteSetUp(void) {}
int suiteTearDown(int num_failures) {}


void test_get_version() {
    char *ver_out_buf = {"test_version"};
    get_version_ExpectAnyArgs();
    get_version_ReturnArrayThruPtr_buf(ver_out_buf, strlen(ver_out_buf));

    char *uri = "/get_version.cgi";
    char out_buf[2048] = {0};
    cgiHandler(uri, out_buf, 0);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(ver_out_buf, out_buf, strlen(ver_out_buf));
}


void test_uncorrect_query() {
    char *ver_out_buf = {"404 Not Found"};
    send404_ExpectAnyArgs();
    send404_ReturnArrayThruPtr_buf(ver_out_buf, strlen(ver_out_buf));

    char *uri = "/get_versions.cgi";
    char out_buf[2048] = {0};
    cgiHandler(uri, out_buf, 0);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(ver_out_buf, out_buf, strlen(ver_out_buf));
}
