/**
 * @brief Arraylist data structure
 *
 * This file defines an ArrayList data structure, which is a dynamic array that
 * can hold a variable number of elements.
 *
 * @file arraylist.h
 * @date 2022-12-21
 * @author Merlin Unterfinger
 */

#ifndef OSURS_DS_ARRAYLIST_H_
#define OSURS_DS_ARRAYLIST_H_

#include <stddef.h>

/**
 * @brief Arraylist
 *
 * This structure represents an ArrayList data structure. It consists of a
 * pointer to an array of void pointers (elements), an integer capacity
 * representing the number of elements in the list, and an integer size
 * representing the list's capacity.
 */
typedef struct {
    void** elements; /**< Data elements array. */
    size_t capacity; /**< Number of elements in the list. */
    size_t size;     /**< List capacity. */
} ArrayList;

/**
 * @brief Initialize an ArrayList structure
 *
 * This function initializes an ArrayList structure by setting its elements
 * array to NULL, its capacity to 0, and its size to 0.
 *
 * @param list The ArrayList structure to be initialized.
 */
void array_list_init(ArrayList* list);

/**
 * @brief Create a new ArrayList structure
 *
 * This function creates a new ArrayList structure and returns a pointer to it.
 * The new structure is initialized with its elements array set to NULL, its
 * capacity set to 0, and its size set to 0.
 *
 * @return A pointer to the new ArrayList structure.
 */
ArrayList* array_list_create();

/**
 * @brief Add an element to the end of an ArrayList
 *
 * This function adds an element to the end of an ArrayList. If the list's
 * current capacity is not large enough to hold the new element, the capacity is
 * increased to accommodate it.
 *
 * @param list The ArrayList to which the element will be added.
 * @param element The element to be added to the list.
 */
void array_list_add(ArrayList* list, void* element);

/**
 * @brief Add an element to an ArrayList at a specific index
 *
 * This function adds an element to an ArrayList at a specific index. If the
 * list's current capacity is not large enough to hold the new element, the
 * capacity is increased to accommodate it. If the index is greater than the
 * current size of the list, the element is added to the end of the list.
 *
 * @param list The ArrayList to which the element will be added.
 * @param index The index at which the element will be inserted.
 * @param element The element to be added to the list.
 */
void array_list_add_at(ArrayList* list, int index, void* element);

/**
 * @brief Get an element from an ArrayList
 *
 * This function retrieves an element from an ArrayList at a specific index. If
 * the index is out of range (less than 0 or greater than or equal to the list's
 * size), the function returns NULL.
 *
 * @param list The ArrayList from which the element will be retrieved.
 * @param index The index of the element to be retrieved.
 * @return The element at the specified index, or NULL if the index is out of
 * range.
 */
void* array_list_get(ArrayList* list, int index);

/**
 * @brief Free an ArrayList structure
 *
 * This function frees an ArrayList structure, including the elements array and
 * the structure itself.
 *
 * @param list The ArrayList structure to be freed.
 */
void array_list_free(ArrayList* list);

#endif  // OSURS_DS_ARRAYLIST_H_
