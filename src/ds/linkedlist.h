/**
 * @brief Linked list data structure
 * 
 * Linked list abstract data type (ADT) implementation. Each node of the doubly linked list contains a value and two pointers: one pointing to the previous node in the list and one pointing to the next node in the list. This allows for efficient insertion and deletion of elements from the list, as well as the ability to easily traverse the list in both directions. 
 * The head of the list points to the first node, and the tail of the list points to the last node. If the list is empty, both the head and the tail will be null.
 * 
 * Note: Accessing elements via indexes in a doubly linked list is slower, with a time complexity of O(n), compared to an array, which has a time complexity of O(1), since elements in a doubly linked list are not stored consecutively in memory and must be accessed by iterating through the elements.
 * 
 * @file linkedlist.h
 * @date: 2022-12-20
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_DS_LINKEDLIST_H_
#define OSURS_DS_LINKEDLIST_H_

typedef struct ListNode {
    struct ListNode* prev;
    struct ListNode* next;
    void* data;
} ListNode;

typedef struct LinkedList {
    ListNode* head;
    ListNode* tail;
    int size;
} LinkedList;

void linked_list_init(LinkedList* list);
LinkedList* linked_list_create();

void linked_list_add_first(LinkedList* list, void* data);
void linked_list_add_last(LinkedList* list, void* data);
void* linked_list_remove_first(LinkedList* list);
void* linked_list_remove_last(LinkedList* list);
void* linked_list_get_first(LinkedList* list);
void* linked_list_get_last(LinkedList* list);
void linked_list_clear(LinkedList* list);
void linked_list_free(LinkedList* list);

#endif  // OSURS_DS_LINKEDLIST_H_