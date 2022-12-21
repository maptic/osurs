/**
 * @brief Linkedlist data structure
 *
 * This file defines a LinkedList data structure, which is an abstract data type
 * (ADT) that implements a doubly linked list. Each node of the list contains a
 * value and two pointers: one pointing to the previous node in the list and one
 * pointing to the next node in the list. This allows for efficient insertion
 * and deletion of elements at the ends of the list, as well as the ability to
 * easily traverse the list in both directions. The head of the list points to
 * the first node, and the tail of the list points to the last node. If the list
 * is empty, both the head and the tail will be null.
 *
 * @note Accessing elements via indexes in a doubly linked list is slower, with
 * a time complexity of O(n), compared to an array, which has a time complexity
 * of O(1), since elements in a doubly linked list are not stored consecutively
 * in memory and must be accessed by iterating through the elements.
 *
 * @file linkedlist.h
 * @date 2022-12-20
 * @author Merlin Unterfinger
 */

#ifndef OSURS_DS_LINKEDLIST_H_
#define OSURS_DS_LINKEDLIST_H_

/**
 * @brief Linked list node
 *
 * This structure represents a node in a LinkedList data structure. It consists
 * of pointers to the previous and next nodes in the list, as well as a void
 * pointer to the node's data.
 */
typedef struct ListNode {
    struct ListNode* prev; /**< Pointer to the previous node in the list. */
    struct ListNode* next; /**< Pointer to the next node in the list. */
    void* data;            /**< Data stored in the node. */
} ListNode;

/**
 * @brief Linked list
 *
 * This structure represents a LinkedList data structure. It consists of
 * pointers to the head and tail of the list (the first and last nodes in the
 * list), as well as an integer size representing the number of elements in the
 * list.
 */
typedef struct LinkedList {
    ListNode* head; /**< Pointer to the head of the list. */
    ListNode* tail; /**< Pointer to the tail of the list. */
    int size;       /**< Number of elements in the list. */
} LinkedList;

/**
 * @brief Initialize a LinkedList structure
 *
 * This function initializes a LinkedList structure by setting its head and tail
 * pointers to NULL and its size to 0.
 *
 * @param list The LinkedList structure to be initialized.
 */
void linked_list_init(LinkedList* list);

/**
 * @brief Create a new LinkedList structure
 *
 * This function creates a new LinkedList structure and returns a pointer to it.
 * The new structure is initialized with its head and tail pointers set to NULL
 * and its size set to 0.
 *
 * @return A pointer to the new LinkedList structure.
 */
LinkedList* linked_list_create();

/**
 * @brief Add an element to the beginning of a LinkedList
 *
 * This function adds an element to the beginning of a LinkedList.
 *
 * @param list The LinkedList to which the element will be added.
 * @param data The element to be added to the list.
 */
void linked_list_add_first(LinkedList* list, void* data);

/**
 * @brief Add an element to the end of a LinkedList
 *
 * This function adds an element to the end of a LinkedList.
 *
 * @param list The LinkedList to which the element will be added.
 * @param data The element to be added to the list.
 */
void linked_list_add_last(LinkedList* list, void* data);

/**
 * @brief Remove the first element from a LinkedList
 *
 * This function removes the first element from a LinkedList and returns a
 * pointer to it. If the list is empty, the function returns NULL.
 *
 * @param list The LinkedList from which the element will be removed.
 * @return The first element of the list, or NULL if the list is empty.
 */
void* linked_list_remove_first(LinkedList* list);

/**
 * @brief Remove the last element from a LinkedList
 *
 * This function removes the last element from a LinkedList and returns a
 * pointer to it. If the list is empty, the function returns NULL.
 *
 * @param list The LinkedList from which the element will be removed.
 * @return The last element of the list, or NULL if the list is empty.
 */
void* linked_list_remove_last(LinkedList* list);

/**
 * @brief Get the first element of a LinkedList
 *
 * This function returns a pointer to the first element of a LinkedList, without
 * removing it from the list. If the list is empty, the function returns NULL.
 *
 * @param list The LinkedList from which the first element will be returned.
 * @return The first element of the list, or NULL if the list is empty.
 */
void* linked_list_get_first(LinkedList* list);

/**
 * @brief Get the last element of a LinkedList
 *
 * This function returns a pointer to the last element of a LinkedList, without
 * removing it from the list. If the list is empty, the function returns NULL.
 *
 * @param list The LinkedList from which the last element will be returned.
 * @return The last element of the list, or NULL if the list is empty.
 */
void* linked_list_get_last(LinkedList* list);

/**
 * @brief Clear all elements from a LinkedList
 *
 * This function removes all elements from a LinkedList, but does not free the
 * list structure itself.
 *
 * @param list The LinkedList to be cleared.
 */
void linked_list_clear(LinkedList* list);

/**
 * @brief Free a LinkedList structure
 *
 * This function frees a LinkedList structure, including all nodes and their
 * data.
 *
 * @param list The LinkedList structure to be freed.
 */
void linked_list_free(LinkedList* list);

#endif  // OSURS_DS_LINKEDLIST_H_
