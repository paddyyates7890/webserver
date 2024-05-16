#include "webserver.h"
#include "log.h"
#include "connection.h"
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* handle_connect(void* arg){
    int client_fd = *((int *)arg);
    write_to_log("connection accepted", SRV_LOG_LVL);
    connection(client_fd);
    free(arg);
    return NULL;
}

int main(void){
    clear_log_files();
    write_to_log("starting setup", SRV_LOG_LVL);    
    write_to_log("access log setup", ACCESS_LOG_LVL);
    write_to_log("error log setup", ERROR_LOG_LVL);

    int server_fd;
    struct sockaddr_in server_addr;

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        write_to_log("socket failed", SRV_LOG_LVL);
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        write_to_log("failed to bind to socket", SRV_LOG_LVL);
        write_to_log("SYSTEM HAS FAILED TO START SEE webserver.log", 9);
        exit(1);
    }

    if (listen(server_fd, 10) < 0) {
        write_to_log("failed to start socket listener", SRV_LOG_LVL);
        write_to_log("SYSTEM HAS FAILED TO START SEE webserver.log", 9);
        exit(1);
    }
    
    write_to_log("listening for connections on http://localhost:9999", 9);
    while(1){
        struct sockaddr_in connect_addr;
        socklen_t connect_addr_len = sizeof(connect_addr);
        int *connect_fd = malloc(sizeof(int));

        if ((*connect_fd= accept(server_fd, (struct sockaddr *)&connect_addr, &connect_addr_len)) < 0) {
            write_to_log("failed to accept connection", SRV_LOG_LVL); 
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_connect, connect_fd);
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}
