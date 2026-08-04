#pragma once

typedef struct List List;
typedef void *NodeType;

struct List *linkedListCreate(void);

struct NodeType *linkedListAddToEnd(struct List *list, NodeType value);

bool linkedListGetValue(struct List *list, int pos, NodeType *out);

int linkedListLength(struct List *list);

void linkedListFree(struct List *list, bool freeValues);

void linkedListRemoveFromEnd(struct List *list, bool freeValue);
