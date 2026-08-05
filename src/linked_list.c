#include <stdlib.h>
#include <stdbool.h>
#include "linked_list.h"
#include "utils.h"

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
static void nodeFree(struct Node *node, bool freeValue)
{
    if (node == NULL)
        return;

    if (freeValue)
        free(node->value);
    free(node);
}

struct List *linkedListCreate(void)
{
    struct List *list = malloc(sizeof(List));
    CHECK(list != NULL);

    list->first = NULL;
    list->last = NULL;
    return list;

cleanup:
    free(list);
    return NULL;
}

static struct Node *linkedListCreateNode(NodeType value)
{
    struct Node *node = malloc(sizeof(struct Node));
    CHECK(node != NULL);
    node->value = value;
    node->next = NULL;
    return node;

cleanup:
    free(node);
    return NULL;
}

struct NodeType *linkedListAddToEnd(struct List *list, NodeType value)
{
    if (list == NULL)
        return NULL;

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

void linkedListRemoveFromEnd(struct List *list, bool freeValue)
{
    if (list == NULL)
        return;

    struct Node *node = list->first;
    while (true)
    {
        if (node->next == list->last)
        {
            node->next = NULL;
            break;
        }
        node = node->next;
    }
    nodeFree(list->last, freeValue);
};

static struct Node *linkedListGetNode(struct List *list, int pos)
{
    if (list == NULL)
        return NULL;

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
    if (list == NULL)
        return false;

    struct Node *node = linkedListGetNode(list, pos);
    if (node == NULL)
        return false;
    *out = node->value;

    return true;
}

int linkedListLength(struct List *list)
{
    if (list == NULL)
        return -1;

    struct Node *node = list->first;
    for (int i = 0;; i++)
    {
        if (node == NULL)
            return i;
        node = node->next;
    }
}

void linkedListFree(struct List *list, bool freeValues)
{
    if (list == NULL)
        return;

    struct Node *node = list->first;
    while (true)
    {
        if (node == NULL)
            return;
        struct Node *nextNode = node->next;
        nodeFree(node, freeValues);
        node = nextNode;
    }
}
