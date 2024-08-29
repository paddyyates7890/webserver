#include "linkedList.h"
#include <stdlib.h>
#include <stdio.h>

linkedList* create_list(){
    return calloc(1, sizeof(linkedList));
}

void LLdestroy(linkedList *list){
    LLFOREACH(list, first, next, cur){
        if (cur->prev){
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}

void LLpush(linkedList *list, void *value){
    int nodeSize = sizeof(listNode);
    listNode *node = calloc(1, nodeSize);
    node->value = value;

    if (list->last == NULL) {
        node->indx = 0;
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
        node->indx = node->prev->indx + 1;
    }

    list->count++;
}

void *LLpop(linkedList *list){
    listNode *node = list->last;
    return (node != NULL ? LLremove(list, node) : NULL);
}

void *LLremove(linkedList *list, listNode* node){
    void *result = NULL;

    if (node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if (node == list->first) {
        list->first = node->next;
        list->first->prev = NULL;
    } else if (node == list->last) {
        list->last = node->prev;
        list->last->next = NULL;
    } else {
        listNode *after = node->next;
        listNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }
    
    list->count--;
    result = node->value;
    free(node);
    
    return result;
}

void *LLindex(linkedList *list, int index){
    listNode* found = NULL;
    int count = LLcount(list);

    if (index == 0) {
        found = list->first; 
    }else if (index == count){
        found = list->last;
    }

    if (found == NULL) {
        LLFOREACH(list, first, next, cur){
            if (cur->indx == index) {
                found = cur;
            }
        }
    }

    return found->value;

}

void LLtoString(linkedList *list){
    int count = list->count;

    for (int i = 0; i < count; i++) {
        char* value = LLindex(list, i);
        printf("%d: %s ", i, value);
    }

    printf("\n");
    fflush(stdout);
}


