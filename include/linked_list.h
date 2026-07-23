#pragma once

typedef struct List List;
typedef void *NodeType;

struct List *linkedListCreate(void);

struct Node *linkedListAddToEnd(struct List *list, NodeType value);

bool linkedListGetValue(struct List *list, int pos, NodeType *out);
