/**
 * @brief Adaptable priority queue data structure
 * @file priority.c
 * @date 2023-01-02
 * @author Merlin Unterfinger
 */

#include "priority.h"

#include <stdio.h>
#include <stdlib.h>

/** The capacity of the priority queue. */
#define INITIAL_CAPACITY 10

// private declarations

static void priority_queue_resize(PriorityQueue *queue);
static int priority_queue_parent(int index);
static int priority_queue_left_child(int index);
static int priority_queue_right_child(int index);
static void priority_queue_swap(PriorityQueueNode **heap, int i, int j);
static void priority_queue_heapify_up(PriorityQueue *queue, int index);
static void priority_queue_heapify_down(PriorityQueue *queue, int index);

// public implementations

PriorityQueue *priority_queue_create() {
    PriorityQueue *queue = malloc(sizeof(PriorityQueue));
    queue->heap = malloc(INITIAL_CAPACITY * sizeof(PriorityQueueNode *));
    queue->capacity = INITIAL_CAPACITY;
    queue->size = 0;
    return queue;
}

PriorityQueueNode *priority_queue_add(PriorityQueue *queue, int priority,
                                      void *data) {
    priority_queue_resize(queue);
    PriorityQueueNode *node = malloc(sizeof(PriorityQueueNode));
    node->priority = priority;
    node->data = data;
    node->index = queue->size;
    queue->heap[queue->size++] = node;
    priority_queue_heapify_up(queue, queue->size - 1);
    return node;
}

void *priority_queue_peek(PriorityQueue *queue) {
    if (queue->size == 0) {
        return NULL;
    }
    return queue->heap[0]->data;
}

void *priority_queue_poll(PriorityQueue *queue) {
    if (queue->size == 0) {
        return NULL;
    }
    void *data = queue->heap[0]->data;
    free(queue->heap[0]);
    queue->heap[0] = queue->heap[--queue->size];
    priority_queue_heapify_down(queue, 0);
    priority_queue_resize(queue);
    return data;
}

void *priority_queue_remove(PriorityQueue *queue, PriorityQueueNode *node) {
    void *data = node->data;
    if (node->index == queue->size - 1) {
        queue->size--;
    } else {
        queue->heap[node->index] = queue->heap[--queue->size];
        queue->heap[node->index]->index = node->index;
        priority_queue_heapify_down(queue, node->index);
        priority_queue_heapify_up(queue, node->index);
    }
    free(node);
    priority_queue_resize(queue);
    return data;
}

void priority_queue_change_priority(PriorityQueue *queue,
                                    PriorityQueueNode *node, int new_priority) {
    node->priority = new_priority;
    priority_queue_heapify_up(queue, node->index);
    priority_queue_heapify_down(queue, node->index);
}

void priority_queue_free(PriorityQueue *queue) {
    for (int i = 0; i < queue->size; i++) {
        free(queue->heap[i]);
    }
    free(queue->heap);
    free(queue);
}

// private implementations

static void priority_queue_resize(PriorityQueue *queue) {
    if (queue->size == queue->capacity) {
        queue->capacity *= 2;
        queue->heap = realloc(queue->heap, queue->capacity * sizeof(PriorityQueueNode*));
    } else if (queue->capacity > INITIAL_CAPACITY && queue->size < queue->capacity / 2) {
        queue->capacity /= 2;
        queue->heap = realloc(queue->heap, queue->capacity * sizeof(PriorityQueueNode*));
    }
}


static int priority_queue_parent(int index) { return (index - 1) / 2; }

static int priority_queue_left_child(int index) { return index * 2 + 1; }

static int priority_queue_right_child(int index) { return index * 2 + 2; }

static void priority_queue_swap(PriorityQueueNode **heap, int i, int j) {
    PriorityQueueNode *temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
    heap[i]->index = i;
    heap[j]->index = j;
}

static void priority_queue_heapify_up(PriorityQueue *queue, int index) {
    while (index > 0 && queue->heap[priority_queue_parent(index)]->priority >
                            queue->heap[index]->priority) {
        priority_queue_swap(queue->heap, index, priority_queue_parent(index));
        index = priority_queue_parent(index);
    }
}

static void priority_queue_heapify_down(PriorityQueue *queue, int index) {
    int minIndex = index;
    int left = priority_queue_left_child(index);
    if (left < queue->size &&
        queue->heap[left]->priority < queue->heap[minIndex]->priority) {
        minIndex = left;
    }
    int right = priority_queue_right_child(index);
    if (right < queue->size &&
        queue->heap[right]->priority < queue->heap[minIndex]->priority) {
        minIndex = right;
    }
    if (index != minIndex) {
        priority_queue_swap(queue->heap, index, minIndex);
        priority_queue_heapify_down(queue, minIndex);
    }
}