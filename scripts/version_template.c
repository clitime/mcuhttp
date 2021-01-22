// file created automaticallysemver.py
// do not change the data, because they may be lost

#include "version.h"


const char device_version_full[] = "v{{SEMVER}} ({{HASH_DATA}})";
const char device_version_core[] = "v{{SEMVER}}";


const char* get_device_version_full(void) {
    return device_version_full;
}


const char *get_device_version_core(void) {
    return device_version_core;
}
