#include <stdlib.h>
#ifndef _HTTP_PARSER_
#define _HTTP_PARSER_

/**
 * Handles different parts of the HTTP buffer
 */
struct http_buffer {
    /** Any errors from regex compilation */
    char* error;
    /** Request Method GET POST PUT DELETE */
    char* method;
    /** The Host the request has been sent to e.g. localhost */
    char* host;
    /** The route that has been requested e.g. /index.html */
    char* route;
    /** The http version being used */
    char* http;
};


#define METHOD_PATTERN "(.*) \\/"
#define HOST_PATTERN "Host: (.*) [A-z]"
#define ROUTE_PATTERN ".* (.*) H"
#define HTTP_PATTERN ".* (HTTP.*).* [A-z]"

struct http_buffer* parse_http(char* buffer);
char* do_regex(char* buffer, char* pattern);
int validate_result(char* result);
void freehttp(struct http_buffer* http);

#define toString(A) printf("method: %s host: %s route: %s http: %s", A->method, A->host, A->route, A->http); 

#endif
