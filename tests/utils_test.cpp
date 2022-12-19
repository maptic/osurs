#include <gtest/gtest.h>

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/utils/hashmap.h"
}

TEST(HashMapTest, PutAndGet) {
    HashMap map;
    hash_map_init(&map);

    hash_map_put(&map, (char *)"key1", (void *)1);
    hash_map_put(&map, (char *)"key2", (void *)2);
    hash_map_put(&map, (char *)"key3", (void *)3);

    EXPECT_EQ((void *)1, hash_map_get(&map, (char *)"key1"));
    EXPECT_EQ((void *)2, hash_map_get(&map, (char *)"key2"));
    EXPECT_EQ((void *)3, hash_map_get(&map, (char *)"key3"));
    EXPECT_EQ(NULL, hash_map_get(&map, (char *)"key4"));

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
