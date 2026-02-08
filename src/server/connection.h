#ifndef _CONNECTION_
#define _CONNECTION_

void connection(int fd);
void handle_http_request(char* buffer, int fd);
void handle_response_success(char* request_route,char* request_method, int fd);
void handle_response_error(char* code, int fd);
void send_response(int fd, char* response, int response_len);

#endif // !DEBUG
