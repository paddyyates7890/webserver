#include <stdio.h>
#include "linkedList.h"

#ifndef _FILELIB_ACTIVE
#define _FILELIB_ACTIVE

int fileExists(char* file);
int filesz(FILE *f);
FILE *open_file_read(char *file);
FILE *open_file_write(char *file);
char* read_line(int line, char* file);
int rLines(char * file);
char* rFileConcat(char *file);
linkedList* rFileList(char *file);
void close_file(FILE *file);
int write_line(char *line, char *file);
void printListFile(char* file, int addNo);
char* getFile(char* file);
void clearFile(char* file);
#endif 
