/**
 * @brief Hashmap data structure
 *
 * Map abstract data type (ADT) implementation as a hashmap. The implementation
 * uses closed-addressing (separate chaining) to handle duplicate index values.
 * A bucket of the hashmap contains an entry, which can hold a reference to the
 * next entry in the same bucket.
 *
 * Note: This is not a multimap, entries on existing keys are replaced. This
 * implementation is not thread-safe.
 *
 * @file hashmap.h
 * @date: 2022-12-19
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_DS_HASHMAP_H_
#define OSURS_DS_HASHMAP_H_

/**
 * @brief Entry of the hashmap.
 *
 * An entry in the hashmap, which knows the next entry (if there are any) in the
 * same bucket (same index).
 */
typedef struct HashMapEntry {
    char* key;                 /**< Key of the entry. */
    void* value;               /**< Value of the entry. */
    struct HashMapEntry* next; /**< NULL or the next entry in the bucket. */
} HashMapEntry;

/**
 * @brief A hashmap.
 *
 * Abstract data type (ADT) map implementation as a hashmap.
 */
typedef struct HashMap {
    HashMapEntry** entries; /**< Buckets for the entries. */
    int size;               /**< Number of entries in the hashmap. */
    int capacity;           /**< Bucket capacity of the hashmap. */
} HashMap;

/**
 * @brief Initialize the hashmap.
 *
 * @param map A hashmap.
 */
void hash_map_init(HashMap* map);

/**
 * @brief Put a new entry into the hashmap.
 *
 * Puts the entry into the bucket at the index (hash value) of the key. If the
 * bucket is not empty, new entry is created and placed to the beginning of the
 * chain. Then the old entry in the array is replaced with the new entry. If a
 * key already exists, the value is replaced.
 *
 * @param map A hashmap.
 * @param key The key.
 * @param value A void pointer to the value.
 */
void hash_map_put(HashMap* map, const char* key, void* value);

/**
 * @brief Get a value from a key.
 *
 * Returns a void pointer to the value or NULL if the key is not existing in the
 * hashmap.
 *
 * @param map A hashmap.
 * @param key The key.
 * @return void* The pointer pointing to the value.
 */
void* hash_map_get(HashMap* map, const char* key);

/**
 * @brief Remove an entry from the hashmap.
 *
 * @param map A hashmap structure.
 * @param key The key.
 */
void hash_map_remove(HashMap* map, const char* key);

/**
 * @brief Print the hashmap content.
 *
 * Prints the key, its index (hash value) and the address (void pointer) of the
 * value.
 *
 * @param map A hashmap struct.
 */
void hash_map_print(HashMap* map);

/**
 * @brief Clear all values in the hashmap.
 *
 * Frees all entries in the hashmap. It is important to note, that the values
 * itself have to be freed manually if needed, by iterating over all entries
 * before clearing.
 *
 * @param map A hashmap struct.
 */
void hash_map_clear(HashMap* map);

/**
 * @brief Free hashmap and entries.
 *
 * It is important to note, that the values itself have to be freed manually if
 * needed, by iterating over all entries before clearing and freeing the
 * hashmap.
 * @param map A hashmap struct.
 */
void hash_map_free(HashMap* map);

#endif  // OSURS_DS_HASHMAP_H_