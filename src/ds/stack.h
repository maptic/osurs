/**
 * @brief Stack data structure
 *
 * Stack abstract data type (ADT) implementation. A LIFO (last-in, first-out) stack data structure that allows for the insertion of elements at the top and the removal of elements from the top.
 *
 * @file stack.h
 * @date: 2022-12-21
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_DS_STACK_H_
#define OSURS_DS_STACK_H_

#include <stdbool.h>

// Define a structure for stack nodes
typedef struct StackNode
{
    void *data;
    struct StackNode *next;
} StackNode;

// Define a structure for the stack
typedef struct Stack
{
    StackNode *top;
    int size;
} Stack;

// Initialize a stack
void stack_init(Stack *stack);
// Function to create a new stack

Stack *stack_create();

// Function to check if the stack is empty
bool stack_is_empty(Stack *stack);

// Function to push an element onto the stack
void stack_push(Stack *stack, void *data);

// Function to pop the top element from the stack
void *stack_pop(Stack *stack);

// Function to return the top element of the stack
void *stack_peek(Stack *stack);

// Function to free the memory used by the stack nodes
void stack_clear(Stack *stack);

// Function to free the memory used by the stack and the nodes
void stack_free(Stack *stack);

#endif  // OSURS_DS_STACK_H_
