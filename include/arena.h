#ifndef ARENA_H
#define ARENA_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  unsigned char *base;
  size_t total_capacity;
  size_t used;
} Arena;

/* Create an arena with given capacity in bytes.
 * Returns {0} on failure (base == NULL).
 */
Arena arena_create(size_t capacity_bytes);

/* Free all memory owned by arena (safe to call multiple times). */
void arena_destroy(Arena *a);

/* Reset arena (keep buffer, just forget allocations). */
void arena_reset(Arena *a);

/* Allocate `size` bytes with `align` alignment (power of two recommended).
 * Returns NULL if not enough space or invalid args.
 */
void *arena_alloc_aligned(Arena *a, size_t size, size_t align);

/* Allocate `size` bytes aligned to sizeof(void*). */
void *arena_alloc(Arena *a, size_t size);

/* Allocate `count * size` bytes and zero-init. */
void *arena_calloc(Arena *a, size_t count, size_t size);

/* Duplicate a string into arena (NUL-terminated). Returns NULL on failure. */
char *arena_strdup(Arena *a, const char *s);

/* Optional: grow arena (realloc underlying buffer).
 * Useful if you don’t want to guess capacity up front.
 * Returns true on success.
 */
bool arena_grow(Arena *a, size_t new_capacity_bytes);

#endif
