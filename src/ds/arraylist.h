/**
 * @brief Arraylist data structure
 * @file arraylist.h
 * @date: 2022-12-21
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_DS_ARRAYLIST_H_
#define OSURS_DS_ARRAYLIST_H_

typedef struct {
    void** data;
    int capacity;
    int size;
} ArrayList;

void array_list_init(ArrayList* list);
ArrayList* array_list_create();
void array_list_add(ArrayList* list, void* element);
void array_list_add_at(ArrayList* list, int index, void* element);
void* array_list_get(ArrayList* list, int index);
void array_list_free(ArrayList* list);

#endif  // OSURS_DS_ARRAYLIST_H_