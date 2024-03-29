/**
 * @brief Data structures for efficient data handling.
 *
 * This header file contains a collection of data structures that are designed
 * to handle and manipulate data in an efficient manner. The data structures
 * included in this file are:
 *  - @b ArrayList: A dynamic array data structure that automatically resizes
 * itself as elements are added or removed. Array lists are useful for
 * situations where fast random access to elements is required, as they provide
 * constant time access to any element in the list.
 *  - @b HashMap: A data structure that maps keys to values using a hash
 * function. Hash maps are useful for situations where it is necessary to
 * quickly search for and retrieve values based on a key.
 *  - @b LinkedList: A linear data structure that consists of a set of nodes,
 * where each node contains a data element and a reference (link) to the next
 * node in the list. Linked lists are useful for situations where it is
 * necessary to insert or remove elements from the middle of a list, as they do
 * not require the reallocation of memory like arrays do.
 *  - @b PriorityQueue: An adaptable priority queue is a type of priority queue
 * that allows the priorities of its elements to be changed after they have been
 * added to the queue, using so-called location-aware entries.
 *  - @b Queue: A linear data structure that follows the first-in, first-out
 * (FIFO) principle. Queues are useful for situations where it is necessary to
 * process items in the order in which they were received.
 *  - @b Stack: A linear data structure that follows the last-in, first-out
 * (LIFO) principle. Stacks are useful for situations where it is necessary to
 * process items in the reverse order in which they were received.
 *
 * @file ds.h
 * @date 2022-12-28
 * @author Merlin Unterfinger
 */

#ifndef OSURS_DS_H_
#define OSURS_DS_H_

#include "osurs/ds/arraylist.h"
#include "osurs/ds/hashmap.h"
#include "osurs/ds/linkedlist.h"
#include "osurs/ds/priority.h"
#include "osurs/ds/queue.h"
#include "osurs/ds/stack.h"

#endif  // OSURS_DS_H_