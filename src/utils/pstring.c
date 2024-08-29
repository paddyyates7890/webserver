#include "pstring.h"
#include "linkedList.h"
#include <string.h>
#include <stdlib.h>

linkedList* splitstr(char* str, char split){
    linkedList* splitStr = create_list();
    int strLen = strlen(str);
    int i = 0;
    int lastSplit = 0;
    for (i = 0; i < strLen; i++) {
        if (str[i] == split || str[i] == '\0') {
            char* substring = substr(lastSplit, i, str);
            LLpush(splitStr, substring);
            lastSplit = i;
        }
    }

    return splitStr;
}

char* substr(int pos,int len, char* str){
    int count = 0;
    char substring[len];
    char* result = malloc(sizeof(substring));

    while(count < len){
        int position = pos + count - 1;
        strncat(substring, &str[position], sizeof(str[position]));
        count++;
    }
    
    memcpy(result, substring, sizeof(substring));
    return result;
}
