#include "unity.h"
#include "http_cgi_request.h"
#include "http_base.h"
#include "mock_http_cgi_function_list.h"

#include <string.h>

bool isAuthorization = false;

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
    cgiHandler(uri, out_buf, 0, 0, M_GET);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(ver_out_buf, out_buf, strlen(ver_out_buf));
}


void test_post_query() {
    char buf[2048] = {0};
    char uri[] = "/check_authorizations.cgi";
    char out_buf[2048] = {0};
    char body[] = "login=true&password=false";
    char *param[] = {&body[0], &body[11]};
    char *auth_return = {"OK!"};

    check_authorization_Expect(buf, param, 2);
    check_authorization_IgnoreArg_param();
    check_authorization_ReturnArrayThruPtr_buf(auth_return, strlen(auth_return));

    cgiHandler(uri, out_buf, body, strlen(body), M_POST);
}


void test_uncorrect_query() {
    char *ver_out_buf = {"404 Not Found"};
    send404_ExpectAnyArgs();
    send404_ReturnArrayThruPtr_buf(ver_out_buf, strlen(ver_out_buf));

    char *uri = "/get_versions.cgi";
    char out_buf[2048] = {0};
    cgiHandler(uri, out_buf, 0, 0, M_GET);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(ver_out_buf, out_buf, strlen(ver_out_buf));
}
