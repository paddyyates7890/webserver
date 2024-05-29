#include "connection.h"
#include "dbg.h"
#include "log.h"
#include "sysglobal.h"
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

void connection(int fd){
    char* buffer = (char *)malloc(BUFFERSIZE * sizeof(char));
    ssize_t bytes_received = recv(fd, buffer, BUFFERSIZE, 0);
    
    if (bytes_received > 0) {
        handle_http_request(buffer, fd);
    }else {
        handle_response_error("400 Bad Request", fd);
    }

    close(fd);
    free(buffer);
}

void handle_http_request(char *buffer, int fd){
    regex_t regex;
    int value;
    write_to_log(buffer, ACCESS_LOG_LVL); 
    //char* pattern = "^[A-z]+ /([^ ]*) HTTP/1";
    char* pattern = "([A-Z])[[:space:]]/(.+)HTTP/1.1[[:space:]]Host:(.*)[[:space:]][A-z !-@\n]+(User-Agent.+)";
    regcomp(&regex, pattern, REG_EXTENDED);
    regmatch_t matches[5];
    value = regexec(&regex, buffer, 5, matches, 0);  
    
    if (value == 0) {
        char* request_method = buffer + matches[1].rm_so;
        char* file_requested = buffer + matches[2].rm_so;
        char* host = buffer + matches[3].rm_so;
        char* user_agent = buffer + matches[4].rm_so;

        debug("%s", request_method);
        debug("%s", file_requested);
        debug("%s", host);
        debug("%s", user_agent);

        if (strcmp(file_requested, " ")) {
            file_requested = "index.html";
        }

        handle_response_sucess(file_requested, request_method, fd);
    }
    else if (value == REG_NOMATCH) {
        write_to_log("Malformed HTTP header no match", SRV_LOG_LVL);
        handle_response_error("400 Bad Request", fd);
    }else {
        write_to_log("HTTP header match failed with error", SRV_LOG_LVL);
        handle_response_error("500 Internal Server Error", fd);
    }

    regfree(&regex);
}

void handle_response_sucess(char* requested_route, char* request_method,  int fd){
    
    int file_fd = open(requested_route, O_RDONLY);
    if (file_fd == -1) {
        handle_response_error("404 Not Found", fd);
    }

    char* header = (char *)malloc(BUFFERSIZE * sizeof(char));
    char* response = (char *)malloc(BUFFERSIZE * 2 * sizeof(char));
    
    struct stat file_stat;
    fstat(file_fd, &file_stat);
    off_t file_size = file_stat.st_size;
    
    char* server = "patrick/0.01";
    time_t t;time(&t);
    char* date = strtok(ctime(&t), "\n");

    snprintf(header, BUFFERSIZE, "HTTP/1.1 200 OK\r\nServer: %s\r\nDate: %s\r\nContent-size: %ld\r\nContent-Type: text/html\r\n\r\n", server, date, file_size);
    
    int res_len = 0;
    memcpy(response, header, strlen(header));
    res_len = strlen(header);

    ssize_t bytes_read;
    while ((bytes_read = read(file_fd, response + res_len, BUFFERSIZE - res_len)) > 0) {
        res_len += bytes_read;
    } 

    send_response(fd, response, res_len);
    free(header);
}

void handle_response_error(char* code, int fd){
    char* response = (char *)malloc(BUFFERSIZE * 2 * sizeof(char)); 
    snprintf(response, BUFFERSIZE, "HTTP/1.1 %s\r\n Content-type: text/html\r\n\r\n<div align='center'><h1>%s</h1></div>", code, code);
    send_response(fd, response, strlen(response));

    pthread_exit(NULL);
}

void send_response(int fd, char* response, int response_len){
    send(fd, response, response_len, 0);
    free(response);
    
    return;
}
