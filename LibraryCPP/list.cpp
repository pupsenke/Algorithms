#include <cstddef>
#include "list.h"

struct ListItem
{
    Data data;
    ListItem *next;
};

struct List
{
    ListItem *first;
    ListItem *last;
};

List *list_create()
{
    List *list = new List;
    list->first = nullptr;
    list->last = nullptr;
    return list;
}

void list_delete(List *list)
{
    if (list == nullptr)
    {
        return;
    }

    while (list->first != nullptr)
    {
        list_erase_first(list);
    }
    delete list;
}

ListItem *list_first(List *list)
{
    if (list == nullptr)
    {
        return nullptr;
    }

    return list->first;
    //return NULL;
}

ListItem *list_last(List *list)
{
    if (list == nullptr)
    {
        return nullptr;
    }

    return list->last;
    //return NULL;
}

Data list_item_data(const ListItem *item)
{
    return item->data;
    //return (Data)0;
}

ListItem *list_item_next(ListItem *item)
{
    if (item == nullptr)
    {
        return nullptr;
    }

    return item->next;
    //return NULL;
}

ListItem *list_item_prev(ListItem *item)
{
    if (item == nullptr)
    {
        return nullptr;
    }

    return nullptr;
    //return NULL;
}

ListItem *list_insert(List *list, Data data)
{
    if (list == nullptr)
    {
        return nullptr;
    }

    ListItem *item = new ListItem;
    item->data = data;
    item->next = list->first;

    list->first = item;

    if (list->last == nullptr)
    {
        list->last = item;
    }

    return item;
    //return NULL;
}

ListItem *list_insert_after(List *list, ListItem *item, Data data)
{
    if (list == nullptr || item == nullptr)
    {
        return nullptr;
    }

    if (item == nullptr)
    {
        return list_insert(list, data);
    }

    ListItem* new_item = new ListItem;
    new_item->data = data;
    new_item->next = item->next;

    item->next = new_item;

    if (list->last == item)
    {
        list->last = new_item;
    }

    return new_item;
}

ListItem *list_erase_first(List *list)
{
    if (list == nullptr || list->first == nullptr)
    {
        return nullptr;
    }

    ListItem *deleted_item = list->first;
    list->first = deleted_item->next;

    if (list->last == deleted_item)
    {
        list->last = nullptr;
    }

    ListItem *next_item = list->first;
    delete deleted_item;

    return next_item;
    //return NULL;
}

ListItem *list_erase_next(List *list, ListItem *item)
{
    if (list == nullptr || item == nullptr || item->next == nullptr)
    {
        return nullptr;
    }

    if (item == nullptr)
    {
        return list_erase_first(list);
    }

    if (item->next == nullptr)
    {
        return nullptr;
    }

    ListItem* deleted_item = item->next;
    item->next = deleted_item->next;

    if (list->last == deleted_item)
    {
        list->last = item;
    }

    ListItem* next_item = item->next;
    delete deleted_item;

    return next_item;
    //return NULL;
}
