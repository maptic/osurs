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

  hash_map_put(&map, "key1", (void*)1);
  hash_map_put(&map, "key2", (void*)2);
  hash_map_put(&map, "key3", (void*)3);

  EXPECT_EQ((void*)1, hash_map_get(&map, "key1"));
  EXPECT_EQ((void*)2, hash_map_get(&map, "key2"));
  EXPECT_EQ((void*)3, hash_map_get(&map, "key3"));
  EXPECT_EQ(NULL, hash_map_get(&map, "key4"));

  hash_map_free(&map);
}

TEST(HashMapTest, Remove) {
  HashMap map;
  hash_map_init(&map);

  hash_map_put(&map, "key1", (void*)1);
  hash_map_put(&map, "key2", (void*)2);
  hash_map_put(&map, "key3", (void*)3);

  hash_map_remove(&map, "key2");

  EXPECT_EQ((void*)1, hash_map_get(&map, "key1"));
  EXPECT_EQ(NULL, hash_map_get(&map, "key2"));
  EXPECT_EQ((void*)3, hash_map_get(&map, "key3"));

  hash_map_clear(&map);

  EXPECT_EQ(NULL, hash_map_get(&map, "key1"));
  EXPECT_EQ(NULL, hash_map_get(&map, "key2"));
  EXPECT_EQ(NULL, hash_map_get(&map, "key3"));

  hash_map_free(&map);
}

TEST(HashMapTest, Resize) {
  HashMap map;
  hash_map_init(&map);

  for (int i = 0; i < 100; i++) {
    char key[10];
    sprintf(key, "key%d", i);
    hash_map_put(&map, key, (void*)(i + 1));
  }

  for (int i = 0; i < 100; i++) {
    char key[10];
    sprintf(key, "key%d", i);
    EXPECT_EQ((void*)(i + 1), hash_map_get(&map, key));
  }

  for (int i = 0; i < 100; i++) {
    char key[10];
    sprintf(key, "key%d", i);
    hash_map_remove(&map, key);
  }

  EXPECT_EQ(NULL, hash_map_get(&map, "key1"));
  EXPECT_EQ(NULL, hash_map_get(&map, "key2"));
  EXPECT_EQ(NULL, hash_map_get(&map, "key3"));

  hash_map_free(&map);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
