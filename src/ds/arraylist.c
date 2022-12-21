/**
 * @brief Arraylist data structure
 * @file arraylist.c
 * @date: 2022-12-21
 * @author: Merlin Unterfinger
 */

#include "arraylist.h"

#include <stdio.h>
#include <stdlib.h>

/** The initial arraylist capacity. */
#define INITIAL_CAPACITY 10

// private declarations

static void array_list_ensure_capacity(ArrayList *list);

// public implementations

void array_list_init(ArrayList *list) {
    list->data = malloc(INITIAL_CAPACITY * sizeof(void*));
    list->capacity = INITIAL_CAPACITY;
    list->size = 0;
}

ArrayList* array_list_create() {
    ArrayList* list = malloc(sizeof(ArrayList));
    array_list_init(list);
    return list;
}

void array_list_add(ArrayList* list, void* element) {
    array_list_ensure_capacity(list);
    list->data[list->size++] = element;
}

void array_list_add_at(ArrayList* list, int index, void* element) {
    if (index < 0 || index > list->size) {
        printf("Index out of bounds\n");
        exit(1);
    }
    array_list_ensure_capacity(list);
    for (int i = list->size; i > index; i--) {
        list->data[i] = list->data[i-1];
    }
    list->data[index] = element;
    list->size++;
}

void* array_list_get(ArrayList* list, int index) {
    if (index >= list->size || index < 0) {
        printf("Index out of bounds\n");
        exit(1);
    }
    return list->data[index];
}

void array_list_free(ArrayList* list) {
    free(list->data);
    free(list);
}

// private implementations

static void array_list_ensure_capacity(ArrayList* list) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(void*));
    }
}