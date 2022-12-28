/**
 * @brief Hashmap data structure
 * @file hashmap.c
 * @date: 2022-12-19
 * @author: Merlin Unterfinger
 */

#include "hashmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** The load factor to trigger resizing of the hashmap capacity. */
#define LOAD_FACTOR 0.75

/** The initial hashmap capacity. */
#define INIT_CAPACITY 10

// private declarations

static unsigned int hash(const char* key, int capacity);
static void hash_map_resize(HashMap* map, int new_capacity);

// public implementations

void hash_map_init(HashMap* map) {
    map->size = 0;
    map->capacity = INIT_CAPACITY;
    map->entries = malloc(sizeof(HashMapEntry*) * map->capacity);
    if (map->entries == NULL) {
        perror("Error allocating memory for hashmap entry array");
        exit(1);
    }
    memset(map->entries, 0, sizeof(HashMapEntry*) * map->capacity);
    map->dynamic_alloc = 0;
}

HashMap* hash_map_create() {
    HashMap* map = malloc(sizeof(HashMap));
    if (map == NULL) {
        perror("Error allocating memory for hash map");
        exit(1);
    }
    hash_map_init(map);
    map->dynamic_alloc = 1;
    return map;
}

void hash_map_put(HashMap* map, const char* key, void* value) {
    if (map->size >= map->capacity * LOAD_FACTOR) {
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
    if (entry == NULL) {
        perror("Error allocating memory for hashmap entry");
        exit(1);
    }
    entry->key = strdup(key);
    entry->value = value;
    entry->next = map->entries[index];
    map->entries[index] = entry;
    map->size++;
}

void* hash_map_get(HashMap* map, const char* key) {
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

void hash_map_remove(HashMap* map, const char* key) {
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
            free(entry->key);
            free(entry);
            map->size--;
            if (map->size <= map->capacity * (1 - LOAD_FACTOR)) {
                hash_map_resize(map, map->capacity / 2);
            }
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

void hash_map_print(HashMap* map) {
    for (size_t i = 0; i < map->capacity; i++) {
        HashMapEntry* entry = map->entries[i];
        while (entry != NULL) {
            printf("{%s (i=%d): %p}\n", entry->key,
                   hash(entry->key, map->capacity), entry->value);
            entry = entry->next;
        }
    }
}

void hash_map_clear(HashMap* map) {
    for (size_t i = 0; i < map->capacity; i++) {
        HashMapEntry* entry = map->entries[i];
        while (entry != NULL) {
            HashMapEntry* next = entry->next;
            free(entry->key);
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
    if (map->dynamic_alloc) free(map);
}

// private implementations

static unsigned int hash(const char* key, int capacity) {
    unsigned int hash = 0;
    for (int i = 0; i < strlen(key); i++) {
        hash = hash * 31 + key[i];
    }
    return hash % capacity;
}

static void hash_map_resize(HashMap* map, int new_capacity) {
    HashMapEntry** old_entries = map->entries;
    int old_capacity = map->capacity;

    map->capacity = new_capacity;
    map->entries = malloc(sizeof(HashMapEntry*) * map->capacity);
    if (map->entries == NULL) {
        perror("Error allocating memory for resizing hashmap entry array");
        exit(1);
    }
    memset(map->entries, 0, sizeof(HashMapEntry*) * map->capacity);

    for (size_t i = 0; i < old_capacity; i++) {
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
