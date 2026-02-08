#include "webserver.h"
#include "log.h"
#include "connection.h"
#include "sysglobal.h"
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

void* handle_connect(void* arg){
    int client_fd = *((int *)arg);
    write_to_log("Connection received", SRV_LOG_LVL);
    connection(client_fd);
    free(arg);
    return NULL;
}

void term_handle(int sig){
    write_to_log("server shutdown init", SRV_LOG_LVL);
    freeconfig();
    pthread_exit(NULL);
    exit(1);
}

int main(void){
    signal(SIGINT, term_handle);
    clear_log_files();
    write_to_log("starting server configuration", SRV_LOG_LVL);    
    setup_sysglobals(); 

    int server_fd;
    int opt = 1;
    struct sockaddr_in server_addr;

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        write_to_log("socket failed", SRV_LOG_LVL);
        exit(1);
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        write_to_log("failed setting stocket options", SRV_LOG_LVL);
        write_to_log("[WARNING] SYSTEM FAILED TO SET STOCKET OPTIONS", SRV_LOG_LVL);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(getport());
    
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        write_to_log("failed to bind to socket", SRV_LOG_LVL);
        write_to_log("[ERROR] SYSTEM HAS FAILED TO START EXITING", SRV_LOG_LVL);
        exit(1);
    }

    if (listen(server_fd, 10) < 0) {
        write_to_log("failed to start socket listener", SRV_LOG_LVL);
        write_to_log("[ERROR] SYSTEM HAS FAILED TO START EXITING", SRV_LOG_LVL);
        exit(1);
    }
    

    write_to_log("Waiting For Connection on http://localhost:%d", SRV_LOG_LVL, getport());
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
