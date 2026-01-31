#include "arena.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

Arena arena_create(size_t capacity_bytes)
{
  Arena a = {0};
  if (capacity_bytes == 0) {
    return a;
  }
  a.base = (unsigned char *)malloc(capacity_bytes);
  if (!a.base) {
    return a;
  }
  a.total_capacity = capacity_bytes;
  a.used = 0;
  return a;
}

void arena_destroy(Arena *a)
{
  if (!a) {
    return;
  }
  free(a->base);
  a->base = NULL;
  a->total_capacity = 0;
  a->used = 0;
}

void arena_reset(Arena *a)
{
  if (!a) {
    return;
  }
  a->used = 0;
}

void *arena_alloc_aligned(Arena *a, size_t size, size_t align)
{
  if (!a || !a->base || size == 0 || align == 0) {
    return NULL;
  }

  uintptr_t base = (uintptr_t)a->base;
  uintptr_t current = base + a->used;
  uintptr_t rem = current % align;
  uintptr_t aligned = rem ? (current + (align - rem)) : current;
  size_t new_used = (size_t)(aligned - base) + size;

  if (new_used > a->total_capacity) {
    return NULL;
  }

  a->used = new_used;
  return (void *)aligned;
}

void *arena_alloc(Arena *a, size_t size)
{
  return arena_alloc_aligned(a, size, sizeof(void *));
}

void *arena_calloc(Arena *a, size_t count, size_t size)
{
  size_t total = count * size;
  void *ptr = arena_alloc(a, total);
  if (!ptr) {
    return NULL;
  }
  memset(ptr, 0, total);
  return ptr;
}

char *arena_strdup(Arena *a, const char *s)
{
  if (!s) {
    return NULL;
  }
  size_t len = strlen(s) + 1;
  char *out = (char *)arena_alloc_aligned(a, len, 1);
  if (!out) {
    return NULL;
  }
  memcpy(out, s, len);
  return out;
}

bool arena_grow(Arena *a, size_t new_capacity_bytes)
{
  if (!a || !a->base) {
    return false;
  }
  if (new_capacity_bytes <= a->total_capacity) {
    return true;
  }

  unsigned char *new_base = (unsigned char *)realloc(a->base, new_capacity_bytes);
  if (!new_base) {
    return false;
  }
  a->base = new_base;
  a->total_capacity = new_capacity_bytes;
  return true;
}
