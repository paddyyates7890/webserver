#ifndef _CONNECTION_
#define _CONNECTION_

void connection(int fd);
void log_buffer(char* buffer);
void handle_http_request(char* buffer, int fd);
void handle_response_sucess(char* request_route,char* request_method, int fd);
void handle_response_error(char* code, int fd);
void send_response(int fd, char* response, int response_len);

#endif // !DEBUG
