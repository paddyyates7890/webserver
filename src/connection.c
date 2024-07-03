#include "connection.h"
#include "http_parser.h"
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
    
    write_to_log(buffer, ACCESS_LOG_LVL);

    if (bytes_received > 0) {
        handle_http_request(buffer, fd);
    }else {
        handle_response_error("400 Bad Request", fd);
    }

    close(fd);
    free(buffer);
}

void handle_http_request(char *buffer, int fd){
    http_buffer* http = parse_http(buffer);
    
    if (http->error == NULL) {
        handle_response_sucess(http->route, http->method, fd);
    }else {
        handle_response_error(http->error, fd);
    }
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
