
#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

typedef struct HashMap HashMap;

/* Create a hashmap with string keys and fixed-size values.
 * value_size: bytes stored per entry (sizeof(int), sizeof(MyStruct), ...)
 * initial_capacity: bucket hint (implementation may round up).
 * Returns NULL on allocation failure.
 */
HashMap *hm_create(size_t value_size, size_t initial_capacity);

/* Free all memory (including internally-copied keys). */
void hm_destroy(HashMap *m);

/* Number of stored entries. */
size_t hm_size(const HashMap *m);

/* Check whether key exists. */
bool hm_has(const HashMap *m, const char *key);

/* Get pointer to stored value for key (modifiable), or NULL if not found. */
void *hm_get(HashMap *m, const char *key);

/* Get pointer to stored value for key (const), or NULL if not found. */
const void *hm_get_const(const HashMap *m, const char *key);

/* Insert or overwrite key with value (copies value_size bytes).
 * Key is copied internally.
 */
bool hm_put(HashMap *m, const char *key, const void *value);

/* Get existing value or insert default if missing.
 * If default_value == NULL, inserted value is zero-initialized.
 * Returns pointer to stored value, or NULL on allocation failure.
 */
void *hm_get_or_put(HashMap *m, const char *key, const void *default_value);

/* Remove key. Returns true if removed, false if absent. */
bool hm_remove(HashMap *m, const char *key);

/* Remove all entries; keep allocated structures for reuse. */
void hm_clear(HashMap *m);

/* Iteration callback: return false to stop early. */
typedef bool (*hm_iter_fn)(const char *key, void *value, void *user);

/* Iterate all entries (order unspecified). */
void hm_foreach(HashMap *m, hm_iter_fn fn, void *user);

#endif /* HASHMAP_H */
