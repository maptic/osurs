/**
 * @brief Queue data structure
 * @file queue.c
 * @date: 2022-12-20
 * @author: Merlin Unterfinger
 */

#include "osurs/ds/queue.h"

#include <stdio.h>
#include <stdlib.h>

void queue_init(Queue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
}

Queue* queue_create() {
    Queue* queue = malloc(sizeof(Queue));
    if (queue == NULL) {
        perror("Error allocating memory for queue");
        exit(1);
    }
    queue_init(queue);
    return queue;
}

void queue_enqueue(Queue* queue, void* data) {
    QueueNode* node = malloc(sizeof(QueueNode));
    if (node == NULL) {
        perror("Error allocating memory for queue node");
        exit(1);
    }
    node->data = data;
    node->next = NULL;
    if (queue->tail == NULL) {
        queue->head = node;
    } else {
        queue->tail->next = node;
    }
    queue->tail = node;
}

void* queue_dequeue(Queue* queue) {
    if (queue->head == NULL) {
        fprintf(stderr, "Error: Queue is empty\n");
        exit(1);
    }
    QueueNode* node = queue->head;
    queue->head = node->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    void* data = node->data;
    free(node);
    return data;
}

bool queue_is_empty(Queue* queue) { return queue->head == NULL; }

void queue_clear(Queue* queue) {
    QueueNode* node = queue->head;
    while (node != NULL) {
        QueueNode* next = node->next;
        free(node);
        node = next;
    }
}

void queue_free(Queue* queue) {
    queue_clear(queue);
    free(queue);
}
