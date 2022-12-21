/**
 * @brief Queue data structure
 * 
 * Queue abstract data type (ADT) implementation. A FIFO (first-in, first-out) queue data structure that allows for the insertion of elements at one end (tail) and the removal of elements from the other end (head).
 * 
 * @file queue.h
 * @date: 2022-12-20
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_DS_QUEUE_H_
#define OSURS_DS_QUEUE_H_

#include <stdbool.h>

typedef struct QueueNode {
    void* data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* head;
    QueueNode* tail;
} Queue;

void queue_init(Queue* queue);
Queue* queue_create();
void queue_enqueue(Queue* queue, void* data);
void* queue_dequeue(Queue* queue);
bool queue_is_empty(Queue* queue);
void queue_clear(Queue* queue);
void queue_free(Queue* queue);

#endif  // OSURS_DS_QUEUE_H_
