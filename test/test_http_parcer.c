#include "unity.h"
#include "http_parser.h"
#include "http_search_buffer.h"


void setUp(void) {}
void tearDown(void) {}
void suiteSetUp(void) {}
int suiteTearDown(int num_failures) {}



void test_get(void) {
    char *m = {"GET"};
    TEST_ASSERT_EQUAL_UINT8(M_GET, getMethod(&m));
}


void test_post(void) {
    char *m = {"POST v1/1"};
    TEST_ASSERT_EQUAL_UINT8(M_POST, getMethod(&m));
}


void test_uncknown(void) {
    char *m = {"DELETE"};
    TEST_ASSERT_EQUAL_UINT8(M_NOT_SUP, getMethod(&m));
}


void test_ext_cgi(void) {
    TEST_ASSERT_EQUAL_UINT8(E_CGI, getDataExtensionRequest((char []){"get_version.cgi?param"}, 22));
}


void test_ext_html(void) {
    TEST_ASSERT_EQUAL_UINT8(E_HTML, getDataExtensionRequest((char []){"index.html?param"}, 17));
}


void test_ext_jpg(void) {
    TEST_ASSERT_EQUAL_UINT8(E_JPG, getDataExtensionRequest((char []){"index.jpg?param"}, 16));
}


void test_ext_png(void) {
    TEST_ASSERT_EQUAL_UINT8(E_PNG, getDataExtensionRequest((char []){"index.png?param"}, 16));
}


void test_ext_gif(void) {
    TEST_ASSERT_EQUAL_UINT8(E_GIF, getDataExtensionRequest((char []){"index.gif?param"}, 16));
}


void test_ext_css(void) {
    TEST_ASSERT_EQUAL_UINT8(E_CSS, getDataExtensionRequest((char []){"index.css?param"}, 16));
}


void test_ext_js(void) {
    TEST_ASSERT_EQUAL_UINT8(E_JS, getDataExtensionRequest((char []){"index.js?param"}, 15));
}


void test_ext_ico(void) {
    TEST_ASSERT_EQUAL_UINT8(E_ICO, getDataExtensionRequest((char []){"index.ico?param"}, 16));
}


void test_ext_txt(void) {
    TEST_ASSERT_EQUAL_UINT8(E_TXT, getDataExtensionRequest((char []){"index.txt?param"}, 16));
}


void test_ext_not_sup(void) {
    TEST_ASSERT_EQUAL_UINT8(E_NOT_SUP, getDataExtensionRequest((char []){"index.mkv?param"}, 16));
}
