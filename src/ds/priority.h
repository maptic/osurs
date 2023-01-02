/**
 * @brief Adaptable priority queue data structure
 *
 * This file defines an adaptable priority queue data structure and its
 * associated functions. An adaptable priority queue is a type of priority queue
 * that allows the priorities of its elements to be changed after they have been
 * added to the queue, using so-called location-aware entries.
 *
 * Adaptable priority queues are typically implemented using a heap data
 * structure, where the priorities of elements are used to determine the order
 * in which they are stored in the heap. The heap property ensures that the
 * element with the highest priority is always at the top of the heap, making it
 * easy to extract the highest-priority element from the queue.
 *
 * @file priority.h
 * @date 2023-01-02
 * @author Merlin Unterfinger
 */

#ifndef OSURS_DS_PRIORITY_H_
#define OSURS_DS_PRIORITY_H_

/**
 * @brief A node in a priority queue
 *
 * @note The fields of this struct are intended for internal use only and should
 * not be accessed directly
 */
typedef struct PriorityQueueNode {
    int priority;
    void *data;
    int index;
} PriorityQueueNode;

/**
 * @brief A priority queue
 *
 * @note The fields of this struct are intended for internal use only and should
 * not be accessed directly
 */
typedef struct {
    PriorityQueueNode **heap;
    int capacity;
    int size;
} PriorityQueue;

/**
 * @brief Creates a new priority queue
 *
 * @return A pointer to the newly created priority queue
 */
PriorityQueue *priority_queue_create();

/**
 * @brief Adds an element to the priority queue
 *
 * @param queue The priority queue to add the element to
 * @param priority The priority of the element being added
 * @param data The data of the element being added
 *
 * @return A pointer to the node in the priority queue corresponding to the
 * added element
 */
PriorityQueueNode *priority_queue_add(PriorityQueue *queue, int priority,
                                      void *data);

/**
 * @brief Returns the data of the element at the front of the priority queue
 *
 * @param queue The priority queue
 *
 * @return The data of the element at the front of the queue, or NULL if the
 * queue is empty
 */
void *priority_queue_peek(PriorityQueue *queue);

/**
 * @brief Removes and returns the data of the element at the front of the
 * priority queue
 *
 * @param queue The priority queue
 *
 * @return The data of the element at the front of the queue, or NULL if the
 * queue is empty
 */
void *priority_queue_poll(PriorityQueue *queue);

/**
 * @brief Removes a specific element from the priority queue, frees the node,
 * and returns its data
 *
 * @param queue The priority queue
 * @param node The node to remove from the priority queue
 *
 * @return The data of the removed node, or NULL if the queue is empty
 */
void *priority_queue_remove(PriorityQueue *queue, PriorityQueueNode *node);

/**
 * @brief Changes the priority of a specific element in the priority queue
 *
 * @param queue The priority queue
 * @param node The node whose priority is being changed
 * @param new_priority The new priority of the node
 */
void priority_queue_change_priority(PriorityQueue *queue,
                                    PriorityQueueNode *node, int new_priority);

/**
 * @brief Frees a priority queue and all of its elements
 *
 * @param queue The priority queue to be freed
 */
void priority_queue_free(PriorityQueue *queue);

#endif  // OSURS_DS_PRIORITY_H_
