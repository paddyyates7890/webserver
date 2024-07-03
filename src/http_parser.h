#ifndef _HTTP_PARSER_
#define _HTTP_PARSER_

struct http_buffer;
typedef struct http_buffer {
    char* error;
    char* method;
    char* host;
    char* route;
    char* http;
}http_buffer;


http_buffer* parse_http(char* buffer);

#endif
