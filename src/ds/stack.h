/**
 * @brief Stack data structure
 *
 * This file defines a Stack data structure, which is an abstract data type
 * (ADT) that implements a LIFO (last-in, first-out) stack. Elements can be
 * inserted at the top and removed from the top.
 *
 * @file stack.h
 * @date 2022-12-21
 * @author Merlin Unterfinger
 */

#ifndef OSURS_DS_STACK_H_
#define OSURS_DS_STACK_H_

#include <stdbool.h>

/**
 * @brief Stack node
 *
 * This structure represents a node in a Stack data structure. It consists of a
 * void pointer to the node's data and a pointer to the next node in the stack.
 */
typedef struct StackNode {
    void* data;             /**< Data stored in the node. */
    struct StackNode* next; /**< Pointer to the next node in the stack. */
} StackNode;

/**
 * @brief Stack
 *
 * This structure represents a Stack data structure. It consists of a pointer to
 * the top of the stack (the node at the top of the stack) and an integer size
 * representing the number of elements in the stack.
 */
typedef struct Stack {
    StackNode* top; /**< Pointer to the top of the stack. */
    int size;       /**< Number of elements in the stack. */
} Stack;

/**
 * @brief Initialize a Stack structure
 *
 * This function initializes a Stack structure by setting its top pointer to
 * NULL and its size to 0.
 *
 * @param stack The Stack structure to be initialized.
 */
void stack_init(Stack* stack);

/**
 * @brief Create a new Stack structure
 *
 * This function creates a new Stack structure and returns a pointer to it. The
 * new structure is initialized with its top pointer set to NULL and its size
 * set to 0.
 *
 * @return A pointer to the new Stack structure.
 */
Stack* stack_create();

/**
 * @brief Check if a Stack is empty
 *
 * This function checks if a Stack is empty (contains no elements).
 *
 * @param stack The Stack to be checked.
 * @return true if the stack is empty, false otherwise.
 */
bool stack_is_empty(Stack* stack);

/**
 * @brief Push an element onto a Stack
 *
 * This function pushes an element onto the top of a Stack.
 *
 * @param stack The Stack onto which the element will be pushed.
 * @param data The element to be pushed onto the stack.
 */
void stack_push(Stack* stack, void* data);

/**
 * @brief Pop the top element from a Stack
 *
 * This function removes the element at the top of a Stack and returns a pointer
 * to it. If the stack is empty, the function returns NULL.
 *
 * @param stack The Stack from which the element will be removed.
 * @return The element at the top of the stack, or NULL if the stack is empty.
 */
void* stack_pop(Stack* stack);

/**
 * @brief Return the top element of a Stack
 *
 * This function returns a pointer to the element at the top of a Stack, without
 * removing it from the stack. If the stack is empty, the function returns NULL.
 *
 * @param stack The Stack from which the top element will be returned.
 * @return The element at the top of the stack, or NULL if the stack is empty.
 */
void* stack_peek(Stack* stack);

/**
 * @brief Clear all elements from a Stack
 *
 * This function removes all elements from a Stack, but does not free the stack
 * structure itself.
 *
 * @param stack The Stack to be cleared.
 */
void stack_clear(Stack* stack);

/**
 * @brief Free a Stack structure
 *
 * This function frees a Stack structure, including all nodes and their data.
 *
 * @param stack The Stack structure to be freed.
 */
void stack_free(Stack* stack);

#endif  // OSURS_DS_STACK_H_
