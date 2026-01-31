#include "hashmap.h"
#include "string_utils.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *key;
  uint32_t hash;
  unsigned char state; /* 0 empty, 1 occupied, 2 tombstone */
} Entry;

struct HashMap {
  size_t value_size;
  size_t size;
  size_t capacity;
  Entry *entries;
  unsigned char *values;
};

static uint32_t fnv1a_hash(const char *s)
{
  uint32_t h = 2166136261u;
  for (const unsigned char *p = (const unsigned char *)s; *p; p++) {
    h ^= *p;
    h *= 16777619u;
  }
  return h;
}

static size_t next_pow2(size_t x)
{
  size_t p = 1;
  while (p < x) {
    p <<= 1;
  }
  return p;
}

static bool hm_init(HashMap *m, size_t capacity)
{
  m->capacity = capacity;
  m->size = 0;
  m->entries = (Entry *)calloc(capacity, sizeof(Entry));
  if (!m->entries) {
    return false;
  }
  m->values = (unsigned char *)calloc(capacity, m->value_size);
  if (!m->values) {
    free(m->entries);
    m->entries = NULL;
    return false;
  }
  return true;
}

static void hm_free_entries(HashMap *m)
{
  for (size_t i = 0; i < m->capacity; i++) {
    if (m->entries[i].state == 1) {
      free(m->entries[i].key);
    }
  }
}

static bool hm_resize(HashMap *m, size_t new_capacity)
{
  HashMap tmp = {0};
  tmp.value_size = m->value_size;
  if (!hm_init(&tmp, new_capacity)) {
    return false;
  }

  for (size_t i = 0; i < m->capacity; i++) {
    if (m->entries[i].state != 1) {
      continue;
    }
    char *key = m->entries[i].key;
    uint32_t hash = m->entries[i].hash;
    size_t mask = tmp.capacity - 1;
    size_t idx = hash & mask;
    while (tmp.entries[idx].state == 1) {
      idx = (idx + 1) & mask;
    }
    tmp.entries[idx].key = key;
    tmp.entries[idx].hash = hash;
    tmp.entries[idx].state = 1;
    memcpy(tmp.values + idx * tmp.value_size,
           m->values + i * m->value_size,
           tmp.value_size);
    tmp.size++;
  }

  free(m->entries);
  free(m->values);
  *m = tmp;
  return true;
}

HashMap *hm_create(size_t value_size, size_t initial_capacity)
{
  if (value_size == 0) {
    return NULL;
  }

  size_t cap = initial_capacity < 16 ? 16 : next_pow2(initial_capacity);
  HashMap *m = (HashMap *)calloc(1, sizeof(HashMap));
  if (!m) {
    return NULL;
  }
  m->value_size = value_size;
  if (!hm_init(m, cap)) {
    free(m);
    return NULL;
  }
  return m;
}

void hm_destroy(HashMap *m)
{
  if (!m) {
    return;
  }
  hm_free_entries(m);
  free(m->entries);
  free(m->values);
  free(m);
}

size_t hm_size(const HashMap *m)
{
  return m ? m->size : 0;
}

static Entry *hm_find_entry(HashMap *m, const char *key, uint32_t hash,
                            size_t *out_index)
{
  size_t mask = m->capacity - 1;
  size_t idx = hash & mask;
  Entry *first_tombstone = NULL;

  for (;;) {
    Entry *e = &m->entries[idx];
    if (e->state == 0) {
      if (out_index) {
        *out_index = (size_t)(first_tombstone ? (first_tombstone - m->entries)
                                             : idx);
      }
      return NULL;
    }
    if (e->state == 2) {
      if (!first_tombstone) {
        first_tombstone = e;
      }
    } else if (e->hash == hash && strcmp(e->key, key) == 0) {
      if (out_index) {
        *out_index = idx;
      }
      return e;
    }
    idx = (idx + 1) & mask;
  }
}

bool hm_has(const HashMap *m, const char *key)
{
  if (!m || !key) {
    return false;
  }
  uint32_t hash = fnv1a_hash(key);
  size_t mask = m->capacity - 1;
  size_t idx = hash & mask;

  for (;;) {
    const Entry *e = &m->entries[idx];
    if (e->state == 0) {
      return false;
    }
    if (e->state == 1 && e->hash == hash && strcmp(e->key, key) == 0) {
      return true;
    }
    idx = (idx + 1) & mask;
  }
}

void *hm_get(HashMap *m, const char *key)
{
  if (!m || !key) {
    return NULL;
  }
  uint32_t hash = fnv1a_hash(key);
  size_t idx = 0;
  Entry *e = hm_find_entry(m, key, hash, &idx);
  if (!e) {
    return NULL;
  }
  return m->values + idx * m->value_size;
}

const void *hm_get_const(const HashMap *m, const char *key)
{
  return hm_get((HashMap *)m, key);
}

bool hm_put(HashMap *m, const char *key, const void *value)
{
  if (!m || !key || !value) {
    return false;
  }

  if ((m->size + 1) * 10 >= m->capacity * 7) {
    if (!hm_resize(m, m->capacity * 2)) {
      return false;
    }
  }

  uint32_t hash = fnv1a_hash(key);
  size_t idx = 0;
  Entry *e = hm_find_entry(m, key, hash, &idx);
  if (e) {
    memcpy(m->values + idx * m->value_size, value, m->value_size);
    return true;
  }

  char *key_copy = str_dup(key);
  if (!key_copy) {
    return false;
  }

  m->entries[idx].key = key_copy;
  m->entries[idx].hash = hash;
  m->entries[idx].state = 1;
  memcpy(m->values + idx * m->value_size, value, m->value_size);
  m->size++;
  return true;
}

void *hm_get_or_put(HashMap *m, const char *key, const void *default_value)
{
  if (!m || !key) {
    return NULL;
  }

  if ((m->size + 1) * 10 >= m->capacity * 7) {
    if (!hm_resize(m, m->capacity * 2)) {
      return NULL;
    }
  }

  uint32_t hash = fnv1a_hash(key);
  size_t idx = 0;
  Entry *e = hm_find_entry(m, key, hash, &idx);
  if (e) {
    return m->values + idx * m->value_size;
  }

  char *key_copy = str_dup(key);
  if (!key_copy) {
    return NULL;
  }

  m->entries[idx].key = key_copy;
  m->entries[idx].hash = hash;
  m->entries[idx].state = 1;
  if (default_value) {
    memcpy(m->values + idx * m->value_size, default_value, m->value_size);
  } else {
    memset(m->values + idx * m->value_size, 0, m->value_size);
  }
  m->size++;
  return m->values + idx * m->value_size;
}

bool hm_remove(HashMap *m, const char *key)
{
  if (!m || !key) {
    return false;
  }
  uint32_t hash = fnv1a_hash(key);
  size_t idx = 0;
  Entry *e = hm_find_entry(m, key, hash, &idx);
  if (!e) {
    return false;
  }

  free(m->entries[idx].key);
  m->entries[idx].key = NULL;
  m->entries[idx].state = 2;
  m->size--;
  return true;
}

void hm_clear(HashMap *m)
{
  if (!m) {
    return;
  }
  hm_free_entries(m);
  memset(m->entries, 0, sizeof(Entry) * m->capacity);
  memset(m->values, 0, m->capacity * m->value_size);
  m->size = 0;
}

void hm_foreach(HashMap *m, hm_iter_fn fn, void *user)
{
  if (!m || !fn) {
    return;
  }
  for (size_t i = 0; i < m->capacity; i++) {
    if (m->entries[i].state == 1) {
      if (!fn(m->entries[i].key, m->values + i * m->value_size, user)) {
        return;
      }
    }
  }
}
