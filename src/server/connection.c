#include "connection.h"
#include "../utils/http_parser.h"
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
#include "../utils/dbg.h"

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

    write_to_log("Connection Finsihed Thread Exiting", SRV_LOG_LVL);
    pthread_exit(NULL);
}

void handle_http_request(char *buffer, int fd){
    struct http_buffer* http = parse_http(buffer);
    toString(http);
    if (http->error == NULL) {
        write_to_log("%s %s %s", ACCESS_LOG_LVL, http->method, http->host, http->route);

        handle_response_sucess(http->route, http->method, fd);
    }else {
        handle_response_error(http->error, fd);
    }

    freehttp(http);
}

void handle_response_sucess(char* requested_route, char* request_method,  int fd){
    write_to_log("Handling Good Request", SRV_LOG_LVL);
    
    if (strcmp(requested_route, "/") == 0) {
        requested_route = getdefaultlocation();
    }

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
    write_to_log("Return Header Created", SRV_LOG_LVL);

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
    write_to_log("Responce rejected with code %s", SRV_LOG_LVL, code);
    char* response = (char *)malloc(BUFFERSIZE * 2 * sizeof(char)); 
    snprintf(response, BUFFERSIZE, "HTTP/1.1 %s\r\n Content-type: text/html\r\n\r\n<div align='center'><h1>%s</h1></div>", code, code);
    write_to_log("Error Responce Created", SRV_LOG_LVL);
    send_response(fd, response, strlen(response));
     
    pthread_exit(NULL);
    write_to_log("listening for connection on localhost:9999", SRV_LOG_LVL);
}

void send_response(int fd, char* response, int response_len){
    write_to_log("Sending Responce", SRV_LOG_LVL);
    long sent = send(fd, response, response_len, 0);
    write_to_log("sending code: %d", SRV_LOG_LVL, sent);
    if (sent < 0) {
        pthread_exit(NULL);
    }
    free(response);
    return;
}
