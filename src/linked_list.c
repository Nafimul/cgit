#include <stdlib.h>
#include <stdbool.h>
#include "linked_list.h"

struct Node
{
    NodeType value;
    struct Node *next;
};

struct List
{
    struct Node *first;
    struct Node *last;
};

struct List *linkedListCreate(void)
{
    struct List *list = malloc(sizeof(List));
    list->first = NULL;
    list->last = NULL;
    return list;
}

static struct Node *linkedListCreateNode(NodeType value)
{
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL)
        return NULL;
    node->value = value;
    node->next = NULL;
    return node;
}

struct NodeType *linkedListAddToEnd(struct List *list, NodeType value)
{
    struct Node *node = linkedListCreateNode(value);
    if (node == NULL)
        return NULL;
    if (list->last == NULL)
    {
        list->first = node;
        list->last = node;
    }
    else
    {
        list->last->next = node;
        list->last = node;
    }
    return node->value;
}

static struct Node *linkedListGetNode(struct List *list, int pos)
{
    struct Node *node = list->first;
    if (list->first == NULL)
        return NULL;
    for (int i = 0; i < pos; i++)
    {
        if (node->next == NULL)
            return NULL;
        node = node->next;
    }
    return node;
}

bool linkedListGetValue(struct List *list, int pos, NodeType *out)
{
    struct Node *node = linkedListGetNode(list, pos);
    if (node == NULL)
        return false;
    *out = node->value;

    return true;
}

int linkedListLength(struct List *list)
{
    struct Node *node = list->first;
    for (int i = 0;; i++)
    {
        if (node == NULL)
            return i;
        node = node->next;
    }
}
