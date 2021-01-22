#include "unity.h"
#include "http_search_buffer.h"

void setUp(void) {}
void tearDown(void) {}
void suiteSetUp(void) {}
int suiteTearDown(int num_failures) {}


void test_search_all() {
    char *glob_uri = "GET cgi_request HTTP/1.1\r\n";
    struct buffer buf = search_in_buffer(&glob_uri, where_method);
    TEST_ASSERT_EQUAL_UINT16(sizeof("GET") - 1, buf.len);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("GET", buf.data, buf.len);

    buf = search_in_buffer(&glob_uri, where_uri);
    TEST_ASSERT_EQUAL_UINT16(sizeof("cgi_request") - 1, buf.len);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("cgi_request", buf.data, buf.len);

    buf = search_in_buffer(&glob_uri, where_version);
    TEST_ASSERT_EQUAL_UINT16(sizeof("HTTP/1.1") - 1, buf.len);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("HTTP/1.1", buf.data, buf.len);
}


void test_search_method() {
    char *uri = "METHOD blabla HTTP1.1";
    struct buffer buf = search_in_buffer(&uri, where_method);
    TEST_ASSERT_EQUAL_UINT16(sizeof("METHOD") - 1, buf.len);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("METHOD", buf.data, buf.len);
}


void test_search_uri() {
    char *uri = "blabla HTTP1.1";
    struct buffer buf = search_in_buffer(&uri, where_uri);
    TEST_ASSERT_EQUAL_UINT16(sizeof("blabla") - 1, buf.len);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("blabla", buf.data, buf.len);
}


void test_search_version() {
    char *uri = "HTTP1.1\r\n";
    struct buffer buf = search_in_buffer(&uri, where_version);
    TEST_ASSERT_EQUAL_UINT16(sizeof("HTTP1.1") - 1, buf.len);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("HTTP1.1", buf.data, buf.len);
}


void test_search_version_err() {
    char *uri = "HTTP1.1";
    struct buffer buf = search_in_buffer(&uri, where_version);
    TEST_ASSERT_EQUAL_UINT16(0, buf.len);
}
