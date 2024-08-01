#include "pstring.h"
#include "linkedList.h"
#include <string.h>

linkedList* split(char* str, char split){
    linkedList* splitStr;
    int strLen = strlen(str);
    int i = 0;
    int lastSplit = 0;
    for (i = 0; i < strLen; i++) {
        if (str[i] == split || str[i] == '\0') {
            LLpush(splitStr, substr(lastSplit, i, str));
            lastSplit = i;
        }
    }

    return splitStr;
}

char* substr(int pos,int len, char* str){
    int i = 0;
    char substring[len];
    

    while(i < len){
        substring[i] = str[pos+i-1];
        i++;
    }

    substring[i] = '\0';
    
    char* result = substring;

    return result;
}
