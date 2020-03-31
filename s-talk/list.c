/*
*
* list.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define MAX_LIST_HEAD_NUMBER 3
#define MAX_LIST_NODE_NUMBER 20

typedef struct list_node_s LIST_NODE;

#define VALID_BEFORE_START -1
#define VALID_BEYOND_END -2

struct list_node_s
{
    void *data;
    LIST_NODE *prev;
    LIST_NODE *next;
};

typedef struct list_head_s
{
    int length;
    LIST_NODE *first;
    LIST_NODE *last;
    LIST_NODE *current;
    int status;
} LIST_HEAD;

#define LIST_HEAD_SIZE sizeof(struct list_head_s)
#define LIST_NODE_SIZE sizeof(struct list_node_s)
#define POINTER_SIZE sizeof(void *)

LIST_HEAD list_head_array[MAX_LIST_HEAD_NUMBER] = {0};
LIST_NODE list_node_array[MAX_LIST_NODE_NUMBER] = {0};

int free_head_array[MAX_LIST_HEAD_NUMBER] = {0};
int free_node_array[MAX_LIST_NODE_NUMBER] = {0};

int isfirst = 0;
int free_head_array_index = 0;
int free_node_array_index = 0;

// makes a new, empty list, and returns its reference on success. Returns a NULL pointer on failure.
LIST *ListCreate()
{
    if (isfirst == 0)
    {
        int i;
        for ( i= 0; i < MAX_LIST_HEAD_NUMBER; i++)
        {
            free_head_array[i] = i;
        }
        free_head_array_index = 0;
        for ( i = 0; i < MAX_LIST_NODE_NUMBER; i++)
        {
            free_node_array[i] = i;
        }
        free_node_array_index = 0;
        isfirst = 1;
    }

    if (free_head_array_index >= MAX_LIST_HEAD_NUMBER)
        return NULL;

    int list_head_index = free_head_array[free_head_array_index];

    LIST_HEAD *list_head = list_head_array + list_head_index;
    list_head->current = NULL;
    list_head->first = NULL;
    list_head->last = NULL;
    list_head->length = 0;
    list_head->status = 0;

    free_head_array_index++;

    return list_head;
}

// int ListCount(list) returns the number of items in list.
int ListCount(LIST *list)
{
    LIST_HEAD *list_head = (LIST_HEAD *)list;
    return list_head->length;
}

//returns a pointer to the first item in list and makes the first item the current item
void *ListFirst(LIST *list)
{
    LIST_HEAD *list_head = (LIST_HEAD *)list;
    list_head->current = list_head->first;
    if (list_head->first == NULL)
        return NULL;
    return list_head->first->data;
}

// returns a pointer to the last item in list and makes the last item the current one.
void *ListLast(LIST *list)
{
    LIST_HEAD *list_head = (LIST_HEAD *)list;
    list_head->current = list_head->last;

    if (list_head->last == NULL)
        return NULL;
    return list_head->last->data;
}

// advances list's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the list, a NULL pointer is returned
void *ListNext(LIST *list)
{
    LIST_HEAD *list_head = (LIST_HEAD *)list;
    if (list_head->current == NULL)
        return NULL;

    LIST_NODE *next_node = list_head->current->next;
    list_head->current = next_node;
    if (next_node == NULL)
    {
        list_head->status = VALID_BEYOND_END;
        return NULL;
    }
    return next_node->data;
}

// backs up list's current item by one, and returns a pointer to the new current item.
// If this operation backs up the current item beyondthe start of the list, a NULL pointer is returned.
void *ListPrev(LIST *list)
{
    LIST_HEAD *list_head = (LIST_HEAD *)list;
    if (list_head->current == NULL)
        return NULL;

    LIST_NODE *prev_node = list_head->current->prev;
    list_head->current = prev_node;

    if (prev_node == NULL)
    {
        list_head->status = VALID_BEFORE_START;
        return NULL;
    }

    return prev_node->data;
}

// returns a pointer to the current item in list.
void *ListCurr(LIST *list)
{
    LIST_HEAD *list_head = (LIST_HEAD *)list;
    if (list_head->current == NULL)
        return NULL;
    return list_head->current->data;
}

/*
adds the new item to list directly after the
current item, and makes item the current item. If the current pointer is before the
start of the list, the item is added at the start. If the current pointer is beyond the
end of the list, the item is added at the end. Returns 0 on success, -1 on failure
*/
int ListAdd(LIST *list, void *item)
{
    if (free_node_array_index >= MAX_LIST_NODE_NUMBER)
        return -1;
    LIST_HEAD *list_head = (LIST_HEAD *)list;

    if (list_head->current == NULL && list_head->status == VALID_BEFORE_START)
    {
        return ListPrepend(list, item);
    }

    if (list_head->current == NULL && list_head->status == VALID_BEYOND_END)
    {
        return ListAppend(list, item);
    }

    int list_node_index = free_node_array[free_node_array_index];

    LIST_NODE *list_node = list_node_array + list_node_index;
    list_node->data = item;
    list_node->prev = NULL;
    list_node->next = NULL;
    free_node_array_index++;

    if (list_head->current->next != NULL)
        list_head->current->next->prev = list_node;

    list_node->prev = list_head->current;
    list_node->next = list_head->current->next;
    list_head->current->next = list_node;

    if (list_head->current == list_head->last)
        list_head->last = list_node;
    if (list_head->current == list_head->first)
        list_head->first = list_node;

    list_head->current = list_node;

    list_head->length++;

    return 0;
}

/* adds item to list directly before the current
item, and makes the new item the current one. If the current pointer is before the
start of the list, the item is added at the start. If the current pointer is beyond the
end of the list, the item is added at the end. Returns 0 on success, -1 on failure.
*/
int ListInsert(LIST *list, void *item)
{
    if (free_node_array_index >= MAX_LIST_NODE_NUMBER)
        return -1;
    LIST_HEAD *list_head = (LIST_HEAD *)list;
    if (list_head->current == NULL && list_head->status == VALID_BEFORE_START)
    {
        return ListPrepend(list, item);
    }

    if (list_head->current == NULL && list_head->status == VALID_BEYOND_END)
    {
        return ListAppend(list, item);
    }

    int list_node_index = free_node_array[free_node_array_index];

    LIST_NODE *list_node = list_node_array + list_node_index;
    list_node->data = item;
    list_node->prev = NULL;
    list_node->next = NULL;
    free_node_array_index++;

    if (list_head->current->prev != NULL)
        list_head->current->prev->next = list_node;

    list_node->next = list_head->current;
    list_node->prev = list_head->current->prev;
    list_head->current->prev = list_node;

    if (list_head->current == list_head->last)
        list_head->last = list_node;
    if (list_head->current == list_head->first)
        list_head->first = list_node;
    list_head->length++;

    list_head->current = list_node;
}

/*adds item to the end of list, and makes the new item the current one. Returns 0 on success, -1 on failure.
*/
int ListAppend(LIST *list, void *item)
{
    if (free_node_array_index >= MAX_LIST_NODE_NUMBER)
        return -1;

    int list_node_index = free_node_array[free_node_array_index];
    LIST_NODE *list_node = list_node_array + list_node_index;
    list_node->data = item;
    list_node->prev = NULL;
    list_node->next = NULL;
    free_node_array_index++;

    LIST_HEAD *list_head = (LIST_HEAD *)list;

    if (list_head->first == NULL)
    {
        list_head->first = list_head->last = list_node;
    }
    else
    {
        list_head->last->next = list_node;
        list_node->prev = list_head->last;
        list_head->last = list_node;
    }
    list_head->length++;
    list_head->current = list_head->last;
    return 1;
}

/*adds item to the front of list, and makes the new item the current one. Returns 0 on success, -1 on failure.
*/
int ListPrepend(LIST *list, void *item)
{
    if (free_node_array_index >= MAX_LIST_NODE_NUMBER)
        return -1;

    int list_node_index = free_node_array[free_node_array_index];

    LIST_NODE *list_node = list_node_array + list_node_index;
    list_node->data = item;
    list_node->prev = NULL;
    list_node->next = NULL;
    free_node_array_index++;

    LIST_HEAD *list_head = (LIST_HEAD *)list;

    if (list_head->first == NULL)
    {
        list_head->first = list_head->last = list_node;
    }
    else
    {
        list_head->first->prev = list_node;
        list_node->next = list_head->first;
        list_head->first = list_node;
    }
    list_head->length++;
    list_head->current = list_head->first;
    return 1;
}

/* Return current item and take it out of list. Make the next item the current one.*/
void *ListRemove(LIST *list)
{
    LIST_HEAD *list_head = (LIST_HEAD *)list;
    if (list_head->current == NULL)
        return NULL;

    void *item = list_head->current->data;
    LIST_NODE *list_node = list_head->current;

    list_head->current = list_node->next;
    if (list_head->first == list_node)
        list_head->first = list_head->first->next;
    else
        list_node->prev->next = list_node->next;

    // check the node whether is the tail in the list and update the next pointer
    if (list_head->last == list_node)
        list_head->last = list_head->last->prev;
    else
        list_node->next->prev = list_node->prev;

    list_node->data = NULL;
    list_node->prev = NULL;
    list_node->next = NULL;

    int list_node_index = (list_node - list_node_array); ///sizeof(LIST_NODE);
    free_node_array_index--;
    free_node_array[free_node_array_index] = list_node_index;

    list_head->length--;

    return item;
}

/* adds list2 to the end of list1. The current pointer is set to the current pointer of list1.
List2 no longer exists after the operation.*/
void ListConcat(LIST *list1, LIST *list2)
{
    LIST_HEAD *list_head_1 = (LIST_HEAD *)list1;
    LIST_HEAD *list_head_2 = (LIST_HEAD *)list2;

    if (list_head_1->last != NULL)
    {
        list_head_1->last->next = list_head_2->first;
        list_head_2->first->prev = list_head_1->last;
    }
    else
    {
        list_head_1->first = list_head_1->last = list_head_2->first;
    }

    list_head_1->length = list_head_1->length + list_head_2->length;
    // delete list2
    list_head_2->current = NULL;
    list_head_2->first = NULL;
    list_head_2->last = NULL;
    list_head_2->length = 0;

    int list_head_index = (list_head_2 - list_head_array); ///sizeof(LIST_HEAD)
    free_head_array_index--;
    free_head_array[free_head_array_index] = list_head_index;

    return;
}

/* delete list. itemFree is a pointer to a routine that frees an item.
 It should be invoked (within ListFree) as: (*itemFree)(itemToBeFreed);*/
void ListFree(LIST *list, void (*itemFree)(void *itemToBeFreed))
{
    LIST_HEAD *list_head = (LIST_HEAD *)list;

    // free node
    LIST_NODE *list_node = list_head->first;
    while (list_node != NULL)
    {
        LIST_NODE *delete_node = list_node;
        list_node = list_node->next;

        delete_node->data = NULL;
        delete_node->prev = NULL;
        delete_node->next = NULL;

        int list_node_index = (delete_node - list_node_array); ///sizeof(LIST_NODE);
        free_node_array_index--;
        free_node_array[free_node_array_index] = list_node_index;

        // free this data
        if (itemFree != NULL)
            itemFree(delete_node);
    }

    // delete list
    list_head->current = NULL;
    list_head->first = NULL;
    list_head->last = NULL;
    list_head->length = 0;

    int list_head_index = (list_head - list_head_array); ///sizeof(LIST_HEAD)
    free_head_array_index--;
    free_head_array[free_head_array_index] = list_head_index;
    return;
}

/* Return last item and take it out of list. Make the new last item the current one.*/
void *ListTrim(LIST *list)
{
    ListLast(list);
    return ListRemove(list);
}

/*
searches list starting at the current item until the end is reached or a match is
found. In this context, a match is determined by the comparator parameter. This
parameter is a pointer to a routine that takes as its first argument an item pointer,
and as its second argument comparisonArg. Comparator returns 0 if the item and
comparisonArg don't match, or 1 if they do. Exactly what constitutes a match is
up to the implementor of comparator. If a match is found, the current pointer is
left at the matched item and the pointer to that item is returned. If no match is
found, the current pointer is left beyond the end of the list and a NULL pointer is
returned.*/
void *ListSearch(LIST *list, int (*comparator)(void* item,void* comparisonArg), void *comparisonArg)
{
    LIST_HEAD *list_head = (LIST_HEAD *)list;
    if (list_head->current == NULL)
        return NULL;

    while( list_head->current != NULL )
    {
        if( comparator( list_head->current->data, comparisonArg ) == 1 )
        {
            LIST_NODE * match_node = list_head->current;
            list_head->current = list_head->current->prev;
            return match_node->data;
        }

        LIST_NODE *next_node = list_head->current->next;
        list_head->current = next_node;
        if (next_node == NULL)
        {
            list_head->status = VALID_BEYOND_END;
            return NULL;
        }
    }

    return NULL;
}
