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
#define HOST_PATTERN "Host: (.*:\\d*) "
#define ROUTE_PATTERN ".* (.*) H"
#define HTTP_PATTERN ".* (HTTP\\/\\d*)"

struct http_buffer* parse_http(char* buffer);
char* do_regex(char* buffer, char* pattern);
int validate_result(char* result);
void freehttp(struct http_buffer* http);
char* http_to_json(struct http_buffer* buffer);

#define toString(A) printf("METHOD: %s \n\nHOST: %s \n\nROUTE: %s \n\nHTTP: %s", A->method, A->host, A->route, A->http); 

#endif
