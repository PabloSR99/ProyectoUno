#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef _list_h
#define _list_h

typedef struct List List;
typedef List Stack;
typedef struct Node Node;
/* list operations */
Node * createNode(void * data);
List * createList();
bool is_empty(List* list);
void * firstList(List * list);
void * nextList(List * list);
void * lastList(List * list);
void * prevList(List * list);
void pushFront(List * list, void * data);
void pushBack(List * list, void * data);
void pushCurrent(List * list, void * data);
void pushFrontCircular(List * list, void * data);
void *popFront(List *list);
void * popBack(List * list);
void *popCurrent(List * list);
List *copyList(List *src);
void cleanList(List * list);

/* stack operations */
Stack* createStack();
void pop(Stack* s);
void* top(Stack* s);
void push(Stack* s, void* data);

#endif /* _list_h */
