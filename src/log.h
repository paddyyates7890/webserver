#ifndef _LOGGER_
#define _LOGGER_

#define ERROR_LOG_LVL -1
#define ACCESS_LOG_LVL 1
#define SRV_LOG_LVL 0

void write_to_log(char* line, int log_level);
void write_to_error_log(char* line);
void write_to_access_log(char* line);
void write_to_srv_log(char* line);
void write_to_console(char * line);
void clear_log_files();
void build_log_text(char* log, char* line, char* logHeader);

#endif // _LOGGER_
