#ifndef _LINKED_LIST
#define _LINKED_LIST

struct listNode;
typedef struct listNode{
    int indx;
    struct listNode *next;
    struct listNode *prev;
    void *value;
}listNode;

typedef struct linkedList{
    int count;
    listNode *first;
    listNode *last;
}linkedList;

linkedList* create_list();

void LLdestroy(linkedList *list);
void LLpush(linkedList *list, void *value);
void *LLpop(linkedList *list);
void *LLremove(linkedList *list, listNode* node);
void *LLindex(linkedList *list, int index);

#define LLcount(A) ((A)->count);

#define LLFOREACH(L, S, M, V) listNode *_node = NULL; listNode *V = NULL; for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
