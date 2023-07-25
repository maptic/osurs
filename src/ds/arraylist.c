/**
 * @brief Arraylist data structure
 * @file arraylist.c
 * @date: 2022-12-21
 * @author: Merlin Unterfinger
 */

#include "osurs/ds/arraylist.h"

#include <stdio.h>
#include <stdlib.h>

/** The initial arraylist capacity. */
#define INITIAL_CAPACITY 10

// private declarations

static void array_list_ensure_capacity(ArrayList* list);

// public implementations

void array_list_init(ArrayList* list) {
    list->elements = malloc(INITIAL_CAPACITY * sizeof(void*));
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
    list->elements[list->size++] = element;
}

void array_list_add_at(ArrayList* list, int index, void* element) {
    if (index < 0 || index > list->size) {
        printf("Index out of bounds\n");
        exit(1);
    }
    array_list_ensure_capacity(list);
    for (size_t i = list->size; i > index; i--) {
        list->elements[i] = list->elements[i - 1];
    }
    list->elements[index] = element;
    list->size++;
}

void* array_list_get(ArrayList* list, int index) {
    if (index >= list->size || index < 0) {
        printf("Index out of bounds\n");
        exit(1);
    }
    return list->elements[index];
}

void array_list_free(ArrayList* list) {
    free(list->elements);
    free(list);
}

// private implementations

static void array_list_ensure_capacity(ArrayList* list) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->elements =
            realloc(list->elements, list->capacity * sizeof(void*));
    }
}