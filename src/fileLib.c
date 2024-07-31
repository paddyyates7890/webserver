#include "dbg.h"
#include "fileLib.h"
#include "linkedList.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int fileExists(char *file){
    !fopen(file, "r");
}

int filesz(FILE *f){
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);

	return size;
}

FILE *open_file_read(char *file) {
    FILE *f = fopen(file, "r");
    check(f, "Unable to Open File: open_file_read(): %s", file);
    
    return f;
error:
    close_file(f);
    exit(0);
}

FILE *open_file_write(char *file) {
    FILE *f = fopen(file, "a");
    check(f, "Unable to Open File: open_file_write(): %s", file);    

    return f;
error:
    printf("FILE NOT OPERNED");
    close_file(f);
    exit(0);
}

void close_file(FILE *file){
	fclose(file);
}

void fix_word(char word[512]){
	word[0] = toupper(word[0]);
	word[strcspn(word, "\n")] = '\0';
}

char* read_line(int line, char* file){
	FILE *f = open_file_read(file);
	int count = 0;
	char* pass = malloc(512);

    while (fgets(pass, sizeof(pass), f) != NULL) {
		if (count == line) {
            printf("%s", pass);
            return pass;
        }
		count ++;
	}
	
    close_file(f);
	
    return pass;
}


int rLines(char *file){
    FILE *f = open_file_read(file);

    int count = 0;
    char c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            count ++;
        }
    }

    close_file(f);
    return count;
}

char* rFileConcat(char *file){
	FILE *f = open_file_read(file);
	char word[512];
	int size = filesz(f);
	char *fileContents = malloc(size);
	while (fgets(word, sizeof(word), f) != NULL) {
		strcat(fileContents, word);
	}
	close_file(f);
	return fileContents;
}

linkedList* rFileList(char *file){
    linkedList *list = create_list();
    int noLines = rLines(file);
    
    if (noLines >= 1) {
        FILE *f = open_file_read(file);
        char lines[noLines][512];
        
        int i = 0;
        while(fgets(lines[i], sizeof(lines[i]), f)) {
            LLpush(list, lines[i]);
            i = i + 1;
        }
        close_file(f);
    }

    return list;
}

int write_line(char *line, char *file){
	FILE *f = open_file_write(file);
    fprintf(f, "%s\n",line);    
    close_file(f);

	return 1;
}

void printListFile(char* file, int addNo){
    linkedList *lines = rFileList(file);
    printf("\n\n");
    
    int fileLen = LLcount(lines);

    int i;
    char* line = LLindex(lines, 0);
    for (i = 1; i <= fileLen; i++){
        if (addNo) {
            printf("%d: %s",i, line);
        }else {
            printf("%s", line);
        }
        line = LLindex(lines, i);
    }

    LLdestroy(lines);
}

char* getFile(char* file){

    char* full_file = malloc(strlen(getenv("HOME")) + strlen(file) + 1);
    strcpy(full_file, getenv("HOME"));
    strcat(full_file, file);


    return full_file;
}

void clearFile(char* file){
    fclose(fopen(file, "w"));
}

