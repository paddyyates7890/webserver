#include "pstring.h"
#include "linkedList.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/** 
 * split a char array by <split> and return a linkedList of the result
 * @param str char* the string to split
 * @param split char the character to split on
 * @return splitStr LinkedList* the split string as a linked list
*/
linkedList* splitstr(char* str, char split){
    linkedList* splitStr = create_list();
    int strLen = strlen(str);
    char tmpStr[strLen];

    for (int i=0; i < strLen; i++){
        if (str[i] != split){
            char ch = str[i];
            strncat(tmpStr, &ch, 1);
            printf("%s", tmpStr);
        }else {
            LLpush(splitStr, tmpStr);
            char tmpStr[strLen];

            printf("%s", tmpStr);
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
