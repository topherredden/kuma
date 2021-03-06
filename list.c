#include "list.h"

void link_behind(klist_node *const base, klist_node *ins)
{
    if (ins->next != NULL)
        ins->next->prev = ins->prev;

    if (ins->prev != NULL)
        ins->prev->next = ins->next;

    if (base->prev == NULL) 
    {
        ins->prev       = NULL;
        ins->next       = base;
        base->prev      = ins;
    } 
    else 
    {
        ins->prev       = base->prev;
        ins->prev->next = ins;
        ins->next       = base;
        base->prev      = ins;
    }
}

void link_after(klist_node *base, klist_node *ins)
{
    if (ins->next)
        ins->next->prev = ins->prev;

    if (ins->prev)
        ins->prev->next = ins->next;

    if (!base->next) 
    {
        ins->prev       = base;
        base->next      = ins;
        ins->next       = NULL;
    } 
    else 
    {
        ins->next       = base->next;
        ins->next->prev = ins;
        ins->prev       = base;
        base->next      = ins;
    }
}

void swap_adjacent(klist_node *n1, klist_node *n2)
{
    if (n1->next == n2) {
        if (n2->next)
            n2->next->prev = n1;

        n1->next = n2->next;

        if (n1->prev)
            n1->prev->next = n2;

        n2->prev = n1->prev;

        n1->prev = n2;
        n2->next = n1;
        return;
    }

    if (n2->next == n1) {
        if (n1->next)
            n1->next->prev = n2;

        n2->next = n1->next;

        if (n2->prev)
            n2->prev->next = n1;

        n1->prev = n2->prev;

        n2->prev = n1;
        n1->next = n2;
        return;
    }
}

void swap(klist_node *n1, klist_node *n2)
{
    if (n1->next == n2 || n2->next == n1) 
    {
        swap_adjacent(n1, n2);
        return;
    }

    klist_node *n1_left  = n1->prev;
    klist_node *n1_right = n1->next;
    klist_node *n2_left  = n2->prev;
    klist_node *n2_right = n2->next;

    if (n1_left)
        n1_left->next = n2;

    n2->prev = n1_left;

    if (n1_right)
        n1_right->prev = n2;

    n2->next = n1_right;

    if (n2_left)
        n2_left->next = n1;

    n1->prev = n2_left;

    if (n2_right)
        n2_right->prev = n1;

    n1->next = n2_right;
}

void *unlink_node(klist *list, klist_node *node)
{
    void *data = node->data;

    if (node->prev != NULL)
        node->prev->next = node->next;

    if (node->prev == NULL)
        list->head = node->next;

    if (node->next == NULL)
        list->tail = node->prev;

    if (node->next != NULL)
        node->next->prev = node->prev;

    free(node);
    list->size--;

    return data;
}

int unlink_all(klist *list, int freed)
{
    if (list->size == 0)
        return 0;

    klist_node *node = list->head;

    while (node) 
    {
        klist_node *tmp = node->next;

        if (freed)
        {
            free(node->data);
        }

        unlink_node(list, node);
        node = tmp;
    }

    return 1;
}

klist_node * get_node_at(klist *list, size_t index)
{
    size_t i;
    klist_node *node = NULL;

    if (index < list->size / 2) 
    {
        node = list->head;
        for (i = 0; i < index; i++)
        {
            node = node->next;
        }
    }
    else 
    {
        node = list->tail;
        for (i = list->size - 1; i > index; i--)
        {
            node = node->prev;
        }
    }

    return node;
}

klist * klist_new()
{
    klist *list = calloc(1, sizeof(klist));

    return list;
}

void klist_destroy(klist *list)
{
    if(list->size > 0)
    {
        klist_remove_all(list);
    }

    free(list);
}

klist_node * klist_add_node_at(klist *list, size_t index)
{
    klist_node *base = klist_get_node_at(list, index);

    klist_node *node = calloc(1, sizeof(klist_node));

    link_behind(base, node);

    if(index == 0)
    {
        list->head = node;
    }

    list->size++;

    return node;
}

klist_node * klist_add_node_first(klist *list)
{
    klist_node *node = calloc(1, sizeof(klist_node));

    if(list->size == 0)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }

    list->size++;

    return node;
}

klist_node * klist_add_node_last(klist *list)
{
    klist_node *node = calloc(1, sizeof(klist_node));

    if(list->size == 0)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;

    return node;
}

void * klist_remove_node(klist *list, klist_node *node)
{
    return unlink_node(list, node);
}

void * klist_remove_node_at(klist *list, size_t index)
{
    klist_node *node = get_node_at(list, index);
    return unlink_node(list, node);
}

void * klist_remove_node_first(klist *list)
{
    return unlink_node(list, list->head);
}

void * klist_remove_node_last(klist *list)
{
    return unlink_node(list, list->tail);
}

void klist_remove_all(klist *list)
{
    int unlinked = unlink_all(list, 0);

    if(unlinked)
    {
        list->head = NULL;
        list->tail = NULL;
    }
}

void klist_remove_all_free(klist *list)
{
    int unlinked = unlink_all(list, 1);

    if(unlinked)
    {
        list->head = NULL;
        list->tail = NULL;
    }
}

size_t klist_size(klist *list)
{
    return list->size;
}

klist_node * klist_get_node_at(klist *list, size_t index)
{
    return get_node_at(list, index);
}

klist_node * klist_get_node_first(klist *list)
{
    return list->head;
}

klist_node * klist_get_node_last(klist *list)
{
    return list->tail;
}

klist_node * klist_get_node_next(klist_node *node)
{
    return node->next;
}

klist_node * klist_get_node_prev(klist_node *node)
{
    return node->prev;
}

klist_node * klist_find_node(klist *list, void *element)
{
    klist_node *node = list->head;
    while (node)
    {
        if (node->data == element)
        {
            return node;
        }

        node = node->next;
    }
    return NULL;
}
