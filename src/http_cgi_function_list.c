#include "http_cgi_function_list.h"
#include "version.h"
#include <stdio.h>


__attribute__((weak)) void get_version(char *buf, char **param, uint8_t len_param) {
    (void)len_param;
    (void)param;
    sprintf(buf, "%s\n", get_device_version_full());
    return;
}

__attribute__((weak)) void send404(char *buf, char **param, uint8_t len_param) {
    (void)len_param;
    (void)param;
    sprintf(buf, "ok");
    return;
}

__attribute__((weak)) void check_authorization(char *buf, char **param, uint8_t len_param) {
    (void)len_param;
    (void)param;
    sprintf(buf, "ok");
    return;
}
