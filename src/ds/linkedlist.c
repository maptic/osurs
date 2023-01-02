/**
 * @brief Linkedlist data structure
 * @file linkedlist.c
 * @date: 2022-12-20
 * @author: Merlin Unterfinger
 */

#include "linkedlist.h"

#include <stdio.h>
#include <stdlib.h>

void linked_list_init(LinkedList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

LinkedList* linked_list_create() {
    LinkedList* list = malloc(sizeof(LinkedList));
    if (list == NULL) {
        perror("Error allocating memory for linked list");
        exit(1);
    }
    linked_list_init(list);
    return list;
}

void linked_list_add_first(LinkedList* list, void* data) {
    ListNode* node = malloc(sizeof(ListNode));
    if (node == NULL) {
        perror("Error allocating memory for first linked list node");
        exit(1);
    }
    node->data = data;
    node->next = list->head;
    node->prev = NULL;  // Initialize the prev pointer to NULL to avoid valgrind error: Conditional jump or move depends on uninitialised value(s)
    if (list->head != NULL) {
        list->head->prev = node;
    }
    list->head = node;
    if (list->tail == NULL) {
        list->tail = node;
    }
    list->size++;
}

void linked_list_add_last(LinkedList* list, void* data) {
    ListNode* node = malloc(sizeof(ListNode));
    if (node == NULL) {
        perror("Error allocating memory for last linked list node");
        exit(1);
    }
    node->data = data;
    node->prev = list->tail;
    node->next = NULL;  // Initialize the next pointer to NULL to avoid valgrind error: Conditional jump or move depends on uninitialised value(s)
    if (list->tail != NULL) {
        list->tail->next = node;
    }
    list->tail = node;
    if (list->head == NULL) {
        list->head = node;
    }
    list->size++;
}

void* linked_list_remove_first(LinkedList* list) {
    if (list->head == NULL) {
        return NULL;
    }
    ListNode* node = list->head;
    list->head = node->next;
    if (list->head != NULL) {
        list->head->prev = NULL;
    }
    // update the tail of the list if it was pointing to the current head
    if (list->tail == node) {
        list->tail = NULL;
    }
    list->size--;
    void* data = node->data;
    free(node);
    return data;
}

void* linked_list_remove_last(LinkedList* list) {
    if (list->tail == NULL) {
        return NULL;
    }
    ListNode* node = list->tail;
    list->tail = node->prev;
    if (list->tail != NULL) {
        list->tail->next = NULL;
    }
    // update the head of the list if it was pointing to the current tail
    if (list->head == node) {
        list->head = NULL;
    }
    list->size--;
    void* data = node->data;
    free(node);
    return data;
}

void* linked_list_get_first(LinkedList* list) {
    if (list->head == NULL) {
        return NULL;
    }
    return list->head->data;
}

void* linked_list_get_last(LinkedList* list) {
    if (list->tail == NULL) {
        return NULL;
    }
    return list->tail->data;
}

void linked_list_clear(LinkedList* list) {
    while (list->size > 0) {
        linked_list_remove_first(list);
    }
}

void linked_list_free(LinkedList* list) {
    linked_list_clear(list);
    free(list);
}
