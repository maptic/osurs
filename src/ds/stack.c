/**
 * @brief Stack data structure
 * @file stack.c
 * @date: 2022-12-21
 * @author: Merlin Unterfinger
 */

#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

StackNode *stack_new_node(void *data)
{
    StackNode *node = (StackNode *)malloc(sizeof(StackNode));
    node->data = data;
    node->next = NULL;
    return node;
}

void stack_init(Stack *stack)
{
    stack->top = NULL;
    stack->size = 0;
}

Stack *stack_create()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL) {
        perror("Error allocating memory for stack");
        exit(1);
    }
    stack_init(stack);
    return stack;
}

bool stack_is_empty(Stack *stack)
{
    return stack->top == NULL;
}

void stack_push(Stack *stack, void *data)
{
    StackNode *node = (StackNode *)malloc(sizeof(StackNode));
    if (node == NULL) {
        perror("Error allocating memory for stack node");
        exit(1);
    }
    node->data = data;
    node->next = stack->top;
    stack->top = node;
    stack->size++;
}

void *stack_pop(Stack *stack)
{
    if (stack_is_empty(stack))
        return NULL;
    StackNode *temp = stack->top;
    void *data = temp->data;
    stack->top = temp->next;
    free(temp);
    stack->size--;
    return data;
}

void *stack_peek(Stack *stack)
{
    if (stack_is_empty(stack))
        return NULL;
    return stack->top->data;
}

void stack_clear(Stack *stack)
{
    StackNode *node = stack->top;
    while (node != NULL)
    {
        StackNode *temp = node;
        node = node->next;
        free(temp);
    }
}

void stack_free(Stack *stack)
{
    stack_clear(stack);
    free(stack);
}
