#include "http_parser.h"
#include "dbg.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

struct http_buffer* create_http_buffer(){
    struct http_buffer* buffer = calloc(1, sizeof(struct http_buffer));
    buffer->error = NULL;
    buffer->method = NULL;
    buffer->http = NULL;
    buffer->host = NULL;
    buffer->route = NULL;
    return buffer;
}

struct http_buffer* parse_http(char* buffer){
    struct http_buffer* parsed = create_http_buffer();
    char* method = NULL;
    char* host = NULL;
    char* route = NULL;
    char* http = NULL;
    
    method = do_regex(buffer, METHOD_PATTERN);
    host = do_regex(buffer, HOST_PATTERN); 
    route = do_regex(buffer, ROUTE_PATTERN); 
    http = do_regex(buffer, HTTP_PATTERN); 
    
    if (validate_result(method)) {
        parsed->method = malloc(sizeof(method));
        parsed->method = method;
    }else {
        parsed->error= malloc(sizeof(method));
        parsed->error = method;
    }

    if (validate_result(host)) {
        parsed->host = malloc(sizeof(host));
        parsed->host = host;
    }else if(parsed->error == NULL){
        parsed->error = malloc(sizeof(host));
        parsed->error = host;
    }

    if (validate_result(route)) {
        parsed->route = malloc(sizeof(route));
        parsed->route = route;
    }else if(parsed->error == NULL){
        parsed->error = malloc(sizeof(route));
        parsed->error = route;
    }

    if (validate_result(http)) {
        parsed->http = malloc(sizeof(http));
        parsed->http = http;
    }else if(parsed->error == NULL){
        parsed->error = malloc(sizeof(http));
        parsed->error = http;
    }
    write_to_log("Sucessfully Parsed HTTP Buffer", SRV_LOG_LVL);
    return parsed;
}

char* do_regex(char* buffer, char* pattern){
    char* result = NULL;
    regex_t re;
    regmatch_t match[2];
    int retval = 0;

    if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
        write_to_log("Failed To Compile Regex", SRV_LOG_LVL);
        result = "500 Internal Server Error";
        goto failed;
    }
    
    if ((retval = regexec(&re, buffer, 2, match, 0)) == 0){
        result = malloc(match[1].rm_eo - match[1].rm_so + 1);
        memcpy(result, buffer + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
        result[match[1].rm_eo - match[1].rm_so] = '\0';
    }/*else {
        char* buf = malloc(512);
        regerror(retval, &re, buf, 512);
        debug("%s", buf);
    }*/

    regfree(&re);

failed:
    if (result == NULL) {
        result = "";
    }
    return result;
}

int validate_result(char* result){
    int valid = 1;
    
    if (strcmp(result, "500 Internal Server Error") == 0 || strcmp(result, "") == 0) {
        debug("%s", result);
        write_to_log("An Error Occured While Parsing HTTP Buffer", SRV_LOG_LVL);
        valid = 0;
    }

    return valid;
}


void freehttp(struct http_buffer* http){
    free(http->error);
    free(http->method);
    free(http->host);
    free(http->route);
    free(http->http);

}
