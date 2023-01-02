#include <gtest/gtest.h>

extern "C" {
#include "../src/ds/arraylist.h"
#include "../src/ds/hashmap.h"
#include "../src/ds/linkedlist.h"
#include "../src/ds/priority.h"
#include "../src/ds/queue.h"
#include "../src/ds/stack.h"
}

// ArrayList

TEST(ArrayListTest, CreateAndDestroy) {
    ArrayList *list = array_list_create();
    array_list_free(list);
}

TEST(ArrayListTest, AddAndGet) {
    ArrayList *list = array_list_create();

    int i1 = 1;
    int i2 = 2;
    int i3 = 3;

    array_list_add(list, &i1);
    array_list_add(list, &i2);
    array_list_add(list, &i3);

    EXPECT_EQ(i1, *(int *)array_list_get(list, 0));
    EXPECT_EQ(i2, *(int *)array_list_get(list, 1));
    EXPECT_EQ(i3, *(int *)array_list_get(list, 2));

    array_list_free(list);
}

TEST(ArrayListTest, AddAt) {
    ArrayList *list = array_list_create();

    int i1 = 1;
    int i2 = 2;
    int i3 = 3;
    int i4 = 4;

    array_list_add(list, &i1);
    array_list_add(list, &i2);
    array_list_add(list, &i3);
    array_list_add_at(list, 1, &i4);

    EXPECT_EQ(i1, *(int *)array_list_get(list, 0));
    EXPECT_EQ(i4, *(int *)array_list_get(list, 1));
    EXPECT_EQ(i2, *(int *)array_list_get(list, 2));
    EXPECT_EQ(i3, *(int *)array_list_get(list, 3));

    array_list_free(list);
}

// HashMap

TEST(HashMapTest, PutAndGet) {
    HashMap map;
    hash_map_init(&map);

    int i1 = 1;
    int i2 = 2;
    int i3 = 3;
    hash_map_put(&map, (char *)"key1", (void *)&i1);
    hash_map_put(&map, (char *)"key2", (void *)&i2);
    hash_map_put(&map, (char *)"key3", (void *)&i3);

    EXPECT_EQ(i1, *(int *)hash_map_get(&map, (char *)"key1"));
    EXPECT_EQ(i2, *(int *)hash_map_get(&map, (char *)"key2"));
    EXPECT_EQ(i3, *(int *)hash_map_get(&map, (char *)"key3"));
    EXPECT_EQ(NULL, hash_map_get(&map, (char *)"key4"));

    hash_map_free(&map);
}

TEST(HashMapTest, RandomGet) {
    HashMap map;
    hash_map_init(&map);

    EXPECT_TRUE(hash_map_get_random(&map) == NULL);

    int i1 = 1;
    int i2 = 2;
    int i3 = 3;
    hash_map_put(&map, (char *)"key1", (void *)&i1);
    hash_map_put(&map, (char *)"key2", (void *)&i2);
    hash_map_put(&map, (char *)"key3", (void *)&i3);

    EXPECT_TRUE(hash_map_get_random(&map) != NULL);
    int value = *(int *)hash_map_get_random(&map);
    EXPECT_TRUE((value >= 1) && (value <= 3));

    hash_map_free(&map);
}

TEST(HashMapTest, Remove) {
    HashMap map;
    hash_map_init(&map);

    hash_map_put(&map, (char *)"key1", (void *)1);
    hash_map_put(&map, (char *)"key2", (void *)2);
    hash_map_put(&map, (char *)"key3", (void *)3);

    hash_map_remove(&map, (char *)"key2");

    EXPECT_EQ((void *)1, hash_map_get(&map, (char *)"key1"));
    EXPECT_EQ(NULL, hash_map_get(&map, (char *)"key2"));
    EXPECT_EQ((void *)3, hash_map_get(&map, (char *)"key3"));

    hash_map_clear(&map);

    EXPECT_EQ(NULL, hash_map_get(&map, (char *)"key1"));
    EXPECT_EQ(NULL, hash_map_get(&map, (char *)"key2"));
    EXPECT_EQ(NULL, hash_map_get(&map, (char *)"key3"));

    hash_map_free(&map);
}

TEST(HashMapTest, Resize) {
    const int size = 100;
    int int_arr[size];
    HashMap map;
    hash_map_init(&map);

    for (int i = 0; i < size; i++) {
        char key[10];
        int_arr[i] = i + 1;
        sprintf(key, "key%d", i);
        hash_map_put(&map, key, (void *)&int_arr[i]);
    }

    for (int i = 0; i < size; i++) {
        char key[10];
        sprintf(key, "key%d", i);
        EXPECT_EQ(int_arr[i], *(int *)hash_map_get(&map, key));
    }

    for (int i = 0; i < size; i++) {
        char key[10];
        sprintf(key, "key%d", i);
        hash_map_remove(&map, key);
    }

    EXPECT_EQ(NULL, hash_map_get(&map, (char *)"key1"));
    EXPECT_EQ(NULL, hash_map_get(&map, (char *)"key2"));
    EXPECT_EQ(NULL, hash_map_get(&map, (char *)"key3"));

    hash_map_free(&map);
}

// LinkedList

TEST(LinkedListTest, TestAddFirst) {
    LinkedList list;
    linked_list_init(&list);

    int i1 = 1;
    int i2 = 2;
    int i3 = 3;
    linked_list_add_first(&list, (void *)&i1);
    linked_list_add_first(&list, (void *)&i2);
    linked_list_add_first(&list, (void *)&i3);

    EXPECT_EQ(list.size, 3);
    EXPECT_EQ(*(int *)linked_list_get_first(&list), i3);
    EXPECT_EQ(*(int *)linked_list_get_last(&list), i1);

    linked_list_clear(&list);
}

TEST(LinkedListTest, TestAddLast) {
    LinkedList list;
    linked_list_init(&list);

    int i1 = 1;
    int i2 = 2;
    int i3 = 3;
    linked_list_add_last(&list, (void *)&i1);
    linked_list_add_last(&list, (void *)&i2);
    linked_list_add_last(&list, (void *)&i3);

    EXPECT_EQ(list.size, 3);
    EXPECT_EQ(*(int *)linked_list_get_first(&list), i1);
    EXPECT_EQ(*(int *)linked_list_get_last(&list), i3);

    linked_list_clear(&list);
}

TEST(LinkedListTest, TestRemoveFirst) {
    LinkedList list;
    linked_list_init(&list);

    int i1 = 1;
    int i2 = 2;
    int i3 = 3;
    linked_list_add_last(&list, (void *)&i1);
    linked_list_add_last(&list, (void *)&i2);
    linked_list_add_last(&list, (void *)&i3);

    void *value = linked_list_remove_first(&list);
    EXPECT_EQ(*(int *)value, i1);
    EXPECT_EQ(list.size, 2);
    EXPECT_EQ(*(int *)linked_list_get_first(&list), i2);
    EXPECT_EQ(*(int *)linked_list_get_last(&list), i3);

    value = linked_list_remove_first(&list);
    EXPECT_EQ(*(int *)value, i2);
    EXPECT_EQ(list.size, 1);
    EXPECT_EQ(*(int *)linked_list_get_first(&list), i3);
    EXPECT_EQ(*(int *)linked_list_get_last(&list), i3);

    linked_list_clear(&list);
}

TEST(LinkedListTest, TestRemoveLast) {
    LinkedList list;
    linked_list_init(&list);

    int i1 = 1;
    int i2 = 2;
    int i3 = 3;
    linked_list_add_last(&list, (void *)&i1);
    linked_list_add_last(&list, (void *)&i2);
    linked_list_add_last(&list, (void *)&i3);

    void *value = linked_list_remove_last(&list);
    EXPECT_EQ(*(int *)value, i3);
    EXPECT_EQ(list.size, 2);
    EXPECT_EQ(*(int *)linked_list_get_first(&list), i1);
    EXPECT_EQ(*(int *)linked_list_get_last(&list), i2);

    value = linked_list_remove_last(&list);
    EXPECT_EQ(*(int *)value, i2);
    EXPECT_EQ(list.size, 1);
    EXPECT_EQ(*(int *)linked_list_get_first(&list), i1);
    EXPECT_EQ(*(int *)linked_list_get_last(&list), i1);

    linked_list_clear(&list);
}

// PriorityQueue

TEST(PriorityQueueTest, Create) {
    PriorityQueue* queue = priority_queue_create();
    EXPECT_NE(queue, nullptr);
    priority_queue_free(queue);
}

TEST(PriorityQueueTest, Add) {
    PriorityQueue* queue = priority_queue_create();
    int value1 = 5;
    int value2 = 10;
    int value3 = 15;
    PriorityQueueNode* node2 = priority_queue_add(queue, 2, &value2);
    PriorityQueueNode* node1 = priority_queue_add(queue, 3, &value1);
    PriorityQueueNode* node3 = priority_queue_add(queue, 1, &value3);
    EXPECT_EQ(node1->data, &value1);
    EXPECT_EQ(node1->priority, 3);
    EXPECT_EQ(node2->data, &value2);
    EXPECT_EQ(node2->priority, 2);
    EXPECT_EQ(node3->data, &value3);
    EXPECT_EQ(node3->priority, 1);
    priority_queue_free(queue);
}


TEST(PriorityQueueTest, Peek) {
    PriorityQueue* queue = priority_queue_create();
    int value1 = 5;
    int value2 = 10;
    int value3 = 15;
    priority_queue_add(queue, 2, &value2);
    priority_queue_add(queue, 3, &value1);
    priority_queue_add(queue, 1, &value3);
    int* result = (int*) priority_queue_peek(queue);
    EXPECT_EQ(*result, value3);
    priority_queue_free(queue);
}

TEST(PriorityQueueTest, Poll) {
    PriorityQueue* queue = priority_queue_create();
    int value1 = 5;
    int value2 = 10;
    int value3 = 15;
    int value4 = 20;
    int value5 = 25;
    priority_queue_add(queue, 3, &value1);
    priority_queue_add(queue, 2, &value2);
    priority_queue_add(queue, 1, &value3);
    priority_queue_add(queue, 4, &value4);
    priority_queue_add(queue, 5, &value5);
    int* result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value3);
    result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value2);
    result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value1);
    result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value4);
    result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value5);
    priority_queue_free(queue);
}

TEST(PriorityQueueTest, Remove) {
    PriorityQueue* queue = priority_queue_create();
    int value1 = 5;
    int value2 = 10;
    int value3 = 15;
    int value4 = 20;
    int value5 = 25;
    PriorityQueueNode* node1 = priority_queue_add(queue, 3, &value1);
    PriorityQueueNode* node2 = priority_queue_add(queue, 2, &value2);
    PriorityQueueNode* node3 = priority_queue_add(queue, 1, &value3);
    PriorityQueueNode* node4 = priority_queue_add(queue, 4, &value4);
    PriorityQueueNode* node5 = priority_queue_add(queue, 5, &value5);
    void* data = priority_queue_remove(queue, node2);
    int* result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value3);
    result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value1);
    result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value4);
    result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value5);
    priority_queue_free(queue);
}


TEST(PriorityQueueTest, ChangePriority) {
    PriorityQueue* queue = priority_queue_create();
    int value1 = 5;
    int value2 = 10;
    int value3 = 15;
    int value4 = 20;
    int value5 = 25;
    PriorityQueueNode* node1 = priority_queue_add(queue, 3, &value1);
    PriorityQueueNode* node2 = priority_queue_add(queue, 2, &value2);
    PriorityQueueNode* node3 = priority_queue_add(queue, 1, &value3);
    PriorityQueueNode* node4 = priority_queue_add(queue, 4, &value4);
    PriorityQueueNode* node5 = priority_queue_add(queue, 5, &value5);
    priority_queue_change_priority(queue, node2, 6);
    priority_queue_change_priority(queue, node4, 2);
    int* result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value3);
    result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value4);
    result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value1);
    result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value5);
    result = (int*) priority_queue_poll(queue);
    EXPECT_EQ(*result, value2);
    priority_queue_free(queue);
}

// Queue

TEST(QueueTest, TestCreate) {
    Queue *queue = queue_create();
    EXPECT_TRUE(queue_is_empty(queue));
    queue_free(queue);
}

TEST(QueueTest, TestEnqueueDequeue) {
    Queue *queue = queue_create();
    queue_enqueue(queue, (void *)1);
    queue_enqueue(queue, (void *)2);
    queue_enqueue(queue, (void *)3);
    EXPECT_EQ(queue_dequeue(queue), (void *)1);
    EXPECT_EQ(queue_dequeue(queue), (void *)2);
    EXPECT_EQ(queue_dequeue(queue), (void *)3);
    EXPECT_TRUE(queue_is_empty(queue));
    queue_free(queue);
}

TEST(QueueTest, TestIsEmpty) {
    Queue *queue = queue_create();
    EXPECT_TRUE(queue_is_empty(queue));
    queue_enqueue(queue, (void *)1);
    EXPECT_FALSE(queue_is_empty(queue));
    queue_dequeue(queue);
    EXPECT_TRUE(queue_is_empty(queue));
    queue_free(queue);
}

// Stack

TEST(StackTest, Empty) {
    Stack stack;
    stack_init(&stack);

    EXPECT_TRUE(stack_is_empty(&stack));
    EXPECT_TRUE(stack_pop(&stack) == NULL);
    EXPECT_TRUE(stack_peek(&stack) == NULL);
    EXPECT_EQ(stack.size, 0);

    stack_push(&stack, (void *)1);
    EXPECT_FALSE(stack_is_empty(&stack));

    stack_clear(&stack);
}

TEST(StackTest, PushPop) {
    Stack stack;
    stack_init(&stack);

    int i1 = 1;
    int i2 = 2;
    int i3 = 3;
    stack_push(&stack, (void *)&i1);
    stack_push(&stack, (void *)&i2);
    stack_push(&stack, (void *)&i3);

    EXPECT_EQ(*(int *)stack_peek(&stack), 3);
    EXPECT_EQ(stack.size, 3);

    stack_pop(&stack);
    EXPECT_EQ(*(int *)stack_peek(&stack), 2);
    EXPECT_EQ(stack.size, 2);

    stack_clear(&stack);
}
