/**
 * @brief Queue data structure
 *
 * This file defines a Queue data structure, which is an abstract data type
 * (ADT) that implements a FIFO (first-in, first-out) queue. Elements can be
 * inserted at one end (tail) and removed from the other end (head).
 *
 * @file queue.h
 * @date 2022-12-20
 * @author Merlin Unterfinger
 */

#ifndef OSURS_DS_QUEUE_H_
#define OSURS_DS_QUEUE_H_

#include <stdbool.h>

/**
 * @brief Queue node
 *
 * This structure represents a node in a Queue data structure. It consists of a
 * void pointer to the node's data and a pointer to the next node in the queue.
 */
typedef struct QueueNode {
    void* data;             /**< Data stored in the node. */
    struct QueueNode* next; /**< Pointer to the next node in the queue. */
} QueueNode;

/**
 * @brief Queue
 *
 * This structure represents a Queue data structure. It consists of pointers to
 * the head and tail of the queue (the nodes at the front and back of the queue,
 * respectively).
 */
typedef struct Queue {
    QueueNode* head; /**< Pointer to the head of the queue. */
    QueueNode* tail; /**< Pointer to the tail of the queue. */
} Queue;

/**
 * @brief Initialize a Queue structure
 *
 * This function initializes a Queue structure by setting its head and tail
 * pointers to NULL.
 *
 * @param queue The Queue structure to be initialized.
 */
void queue_init(Queue* queue);

/**
 * @brief Create a new Queue structure
 *
 * This function creates a new Queue structure and returns a pointer to it. The
 * new structure is initialized with its head and tail pointers set to NULL.
 *
 * @return A pointer to the new Queue structure.
 */
Queue* queue_create();

/**
 * @brief Add an element to the end of a Queue
 *
 * This function adds an element to the end of a Queue. If the queue is empty,
 * the element becomes both the head and the tail of the queue.
 *
 * @param queue The Queue to which the element will be added.
 * @param data The element to be added to the queue.
 */
void queue_enqueue(Queue* queue, void* data);

/**
 * @brief Remove an element from the front of a Queue
 *
 * This function removes the element at the front of a Queue and returns a
 * pointer to it. If the queue is empty, the function returns NULL.
 *
 * @param queue The Queue from which the element will be removed.
 * @return The element at the front of the queue, or NULL if the queue is empty.
 */
void* queue_dequeue(Queue* queue);

/**
 * @brief Remove a node from the Queue
 *
 * This function removes the given node from the Queue. If the queue is empty, 
 * the function returns NULL.
 *
 * @param queue The Queue from which the node will be removed.
 * @param node The node to remove.
 */
void queue_remove_node(Queue* queue, QueueNode* node);

/**
 * @brief Remove a node from the queue that contains the given value
 *
 * This function removes the first node with the given value from the Queue. 
 * If the queue is empty, the function returns NULL.
 *
 * @param queue The Queue from which the node will be removed.
 * @param data The element to be removed from the queue.
 */
void queue_remove_node_by_value(Queue* queue, void* data);

/**
 * @brief Check if a Queue is empty
 *
 * This function checks if a Queue is empty (contains no elements).
 *
 * @param queue The Queue to be checked.
 * @return true if the queue is empty, false otherwise.
 */
bool queue_is_empty(Queue* queue);

/**
 * @brief Clear all elements from a Queue
 *
 * This function removes all elements from a Queue, but does not free the queue
 * structure itself.
 *
 * @param queue The Queue to be cleared.
 */
void queue_clear(Queue* queue);

/**
 * @brief Free a Queue structure
 *
 * This function frees a Queue structure, including all nodes and their data.
 *
 * @param queue The Queue structure to be freed.
 */
void queue_free(Queue* queue);

#endif  // OSURS_DS_QUEUE_H_
