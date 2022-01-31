#ifndef http_search_buffer_h_
#define http_search_buffer_h_

#include "http_base.h"
#include <stdint.h>


enum what_search{where_method, where_uri, where_version, where_header, where_body};

struct buffer searchInBuffer(char **buf, enum what_search);

#endif
