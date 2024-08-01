#include "log.h"
#include "../utils/fileLib.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include "sysglobal.h"

void write_to_log(char *line, int log_level, ...){
    int sysstatus = 0;
    int debug = 0;
    char logStr[1024];

    va_list arrgs;
    va_start(arrgs, log_level);
    vsnprintf(logStr, 1024, line, arrgs); 

    if (sysstatus) {
        write_to_console(logStr);
    }else {
        switch (log_level) {
            case ACCESS_LOG_LVL:
                write_to_access_log(logStr);
                break;
            case SRV_LOG_LVL:
                if (debug) {
                    write_to_console(logStr);
                }else {
                    write_to_srv_log(logStr);
                }
                break;
            default:
                write_to_console(logStr);
        }
    }
}

void write_to_access_log(char *line){
    char* logHeader = " ACCESS LOG: ";
    char log[512];
    build_log_text(log, line, logHeader);
    write_line(log, ACCESS_LOG);
}


void write_to_srv_log(char *line){
    char* logHeader = " SERVER LOG: ";
    char log[512];
    build_log_text(log, line, logHeader);
    write_line(log, SRV_LOG);
}

void write_to_console(char* line){
    printf("%s\n", line);
}

void clear_log_files(){
   clearFile(ACCESS_LOG);
   clearFile(SRV_LOG);
}

void build_log_text(char* log, char* line, char* logHeader){
    time_t t; time(&t);
    char* ct = strtok(ctime(&t), "\n");
    snprintf(log, 512, "%s%s: %s", ct, logHeader, line);
}

