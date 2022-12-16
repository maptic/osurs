/**
 * @brief Seat reservation optimization.
 * @file hashmap.c
 * @date: 2022-08-22
 * @author: Tobias Meier
 */

#ifndef OSURS_UTILS_HASHMAP_H_
#define OSURS_UTILS_HASHMAP_H_

typedef struct HashMapEntry {
  char* key;
  void* value;
  struct HashMapEntry* next;
} HashMapEntry;

typedef struct HashMap {
  HashMapEntry** entries;
  int size;
  int capacity;
} HashMap;

void hash_map_init(HashMap* map);
void hash_map_put(HashMap* map, char* key, void* value);
void* hash_map_get(HashMap* map, char* key);
void hash_map_remove(HashMap* map, char* key);
void hash_map_clear(HashMap* map);
void hash_map_free(HashMap* map);

#endif  // OSURS_UTILS_HASHMAP_H_