#include "unity.h"
#include "http_parser.h"
#include "http_search_buffer.h"


void setUp(void) {}
void tearDown(void) {}
void suiteSetUp(void) {}
int suiteTearDown(int num_failures) {}



void test_get(void) {
    char *m = {"GET"};
    TEST_ASSERT_EQUAL_UINT8(M_GET, get_method(&m));
}


void test_post(void) {
    char *m = {"POST v1/1"};
    TEST_ASSERT_EQUAL_UINT8(M_POST, get_method(&m));
}


void test_uncknown(void) {
    char *m = {"DELETE"};
    TEST_ASSERT_EQUAL_UINT8(M_NOT_SUP, get_method(&m));
}


void test_ext_cgi(void) {
    TEST_ASSERT_EQUAL_UINT8(E_CGI, get_data_extension_request((char []){"get_version.cgi?param"}, 22));
}


void test_ext_html(void) {
    TEST_ASSERT_EQUAL_UINT8(E_HTML, get_data_extension_request((char []){"index.html?param"}, 17));
}


void test_ext_jpg(void) {
    TEST_ASSERT_EQUAL_UINT8(E_JPG, get_data_extension_request((char []){"index.jpg?param"}, 16));
}


void test_ext_png(void) {
    TEST_ASSERT_EQUAL_UINT8(E_PNG, get_data_extension_request((char []){"index.png?param"}, 16));
}


void test_ext_gif(void) {
    TEST_ASSERT_EQUAL_UINT8(E_GIF, get_data_extension_request((char []){"index.gif?param"}, 16));
}


void test_ext_css(void) {
    TEST_ASSERT_EQUAL_UINT8(E_CSS, get_data_extension_request((char []){"index.css?param"}, 16));
}


void test_ext_js(void) {
    TEST_ASSERT_EQUAL_UINT8(E_JS, get_data_extension_request((char []){"index.js?param"}, 15));
}


void test_ext_ico(void) {
    TEST_ASSERT_EQUAL_UINT8(E_ICO, get_data_extension_request((char []){"index.ico?param"}, 16));
}


void test_ext_txt(void) {
    TEST_ASSERT_EQUAL_UINT8(E_TXT, get_data_extension_request((char []){"index.txt?param"}, 16));
}


void test_ext_not_sup(void) {
    TEST_ASSERT_EQUAL_UINT8(E_NOT_SUP, get_data_extension_request((char []){"index.mkv?param"}, 16));
}
