/**
 * @brief Hashmap implementation.
 * @file hashmap.c
 * @date: 2022-08-22
 * @author: Merlin Unterfinger
 */

#include "hashmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// private declarations

static unsigned int hash(char* key, int capacity);
static void hash_map_resize(HashMap* map, int new_capacity);

// public implementations

void hash_map_init(HashMap* map) {
  map->size = 0;
  map->capacity = 10;
  map->entries = malloc(sizeof(HashMapEntry*) * map->capacity);
  memset(map->entries, 0, sizeof(HashMapEntry*) * map->capacity);
}

void hash_map_put(HashMap* map, char* key, void* value) {
  if (map->size >= map->capacity * 0.75) {
    hash_map_resize(map, map->capacity * 2);
  }
  unsigned int index = hash(key, map->capacity);
  HashMapEntry* entry = map->entries[index];
  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      entry->value = value;
      return;
    }
    entry = entry->next;
  }
  entry = malloc(sizeof(HashMapEntry));
  entry->key = key;
  entry->value = value;
  entry->next = map->entries[index];
  map->entries[index] = entry;
  map->size++;
}

void* hash_map_get(HashMap* map, char* key) {
  unsigned int index = hash(key, map->capacity);
  HashMapEntry* entry = map->entries[index];
  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      return entry->value;
    }
    entry = entry->next;
  }
  return NULL;
}

void hash_map_remove(HashMap* map, char* key) {
  unsigned int index = hash(key, map->capacity);
  HashMapEntry* entry = map->entries[index];
  HashMapEntry* prev = NULL;
  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      if (prev == NULL) {
        map->entries[index] = entry->next;
      } else {
        prev->next = entry->next;
      }
      free(entry);
      map->size--;
      if (map->size <= map->capacity * 0.25) {
        hash_map_resize(map, map->capacity / 2);
      }
      return;
    }
    prev = entry;
    entry = entry->next;
  }
}

void hash_map_clear(HashMap* map) {
  for (int i = 0; i < map->capacity; i++) {
    HashMapEntry* entry = map->entries[i];
    while (entry != NULL) {
      HashMapEntry* next = entry->next;
      free(entry);
      entry = next;
    }
    map->entries[i] = NULL;
  }
  map->size = 0;
}

void hash_map_free(HashMap* map) {
  hash_map_clear(map);
  free(map->entries);
  map->entries = NULL;
  map->size = 0;
  map->capacity = 0;
}

// private implemenations

static unsigned int hash(char* key, int capacity) {
  unsigned int hash = 0;
  for (int i = 0; i < strlen(key); i++) {
    hash = hash * 31 + key[i];
  }
  return hash % capacity;
}

void hash_map_resize(HashMap* map, int new_capacity) {
  HashMapEntry** old_entries = map->entries;
  int old_capacity = map->capacity;

  map->capacity = new_capacity;
  map->entries = malloc(sizeof(HashMapEntry*) * map->capacity);
  memset(map->entries, 0, sizeof(HashMapEntry*) * map->capacity);

  for (int i = 0; i < old_capacity; i++) {
    HashMapEntry* entry = old_entries[i];
    while (entry != NULL) {
      HashMapEntry* next = entry->next;
      unsigned int index = hash(entry->key, map->capacity);
      entry->next = map->entries[index];
      map->entries[index] = entry;
      entry = next;
    }
  }

  free(old_entries);
}
