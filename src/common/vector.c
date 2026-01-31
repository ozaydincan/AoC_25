#include "vector.h"

#include <stdlib.h>
#include <string.h>

static bool vec_grow(Vector *v, size_t min_cap)
{
  if (!v || v->elem_size == 0) {
    return false;
  }

  size_t new_cap = v->cap == 0 ? 4 : v->cap;
  while (new_cap < min_cap) {
    if (new_cap > SIZE_MAX / 2) {
      return false;
    }
    new_cap *= 2;
  }

  if (new_cap > SIZE_MAX / v->elem_size) {
    return false;
  }

  size_t new_bytes = new_cap * v->elem_size;
  unsigned char *new_data = (unsigned char *)realloc(v->data, new_bytes);
  if (!new_data) {
    return false;
  }
  v->data = new_data;
  v->cap = new_cap;
  return true;
}

Vector vec_create(size_t elem_size)
{
  Vector v = {0};
  if (elem_size == 0) {
    return v;
  }
  v.elem_size = elem_size;
  return v;
}

void vec_destroy(Vector *v)
{
  if (!v) {
    return;
  }
  free(v->data);
  v->data = NULL;
  v->len = 0;
  v->cap = 0;
  v->elem_size = 0;
}

void vec_clear(Vector *v)
{
  if (!v) {
    return;
  }
  v->len = 0;
}

bool vec_reserve(Vector *v, size_t min_cap)
{
  if (!v) {
    return false;
  }
  if (min_cap <= v->cap) {
    return true;
  }
  return vec_grow(v, min_cap);
}

bool vec_resize(Vector *v, size_t new_len)
{
  if (!v) {
    return false;
  }
  if (new_len > v->cap) {
    if (!vec_grow(v, new_len)) {
      return false;
    }
  }
  if (new_len > v->len) {
    size_t old_bytes = v->len * v->elem_size;
    size_t new_bytes = new_len * v->elem_size;
    memset(v->data + old_bytes, 0, new_bytes - old_bytes);
  }
  v->len = new_len;
  return true;
}

bool vec_push(Vector *v, const void *elem)
{
  if (!v || !elem) {
    return false;
  }
  if (v->len == v->cap) {
    if (!vec_grow(v, v->len + 1)) {
      return false;
    }
  }
  memcpy(v->data + v->len * v->elem_size, elem, v->elem_size);
  v->len++;
  return true;
}

bool vec_extend(Vector *v, const void *elems, size_t n)
{
  if (!v || !elems) {
    return false;
  }
  if (n == 0) {
    return true;
  }
  if (v->len + n > v->cap) {
    if (!vec_grow(v, v->len + n)) {
      return false;
    }
  }
  memcpy(v->data + v->len * v->elem_size, elems, n * v->elem_size);
  v->len += n;
  return true;
}

bool vec_pop(Vector *v, void *out)
{
  if (!v || v->len == 0) {
    return false;
  }
  v->len--;
  if (out) {
    memcpy(out, v->data + v->len * v->elem_size, v->elem_size);
  }
  return true;
}

void *vec_get(Vector *v, size_t index)
{
  if (!v || index >= v->len) {
    return NULL;
  }
  return v->data + index * v->elem_size;
}

const void *vec_get_const(const Vector *v, size_t index)
{
  if (!v || index >= v->len) {
    return NULL;
  }
  return v->data + index * v->elem_size;
}

bool vec_set(Vector *v, size_t index, const void *elem)
{
  if (!v || !elem || index >= v->len) {
    return false;
  }
  memcpy(v->data + index * v->elem_size, elem, v->elem_size);
  return true;
}

bool vec_insert(Vector *v, size_t index, const void *elem)
{
  if (!v || !elem || index > v->len) {
    return false;
  }
  if (v->len == v->cap) {
    if (!vec_grow(v, v->len + 1)) {
      return false;
    }
  }
  size_t move_bytes = (v->len - index) * v->elem_size;
  memmove(v->data + (index + 1) * v->elem_size,
          v->data + index * v->elem_size, move_bytes);
  memcpy(v->data + index * v->elem_size, elem, v->elem_size);
  v->len++;
  return true;
}

bool vec_remove(Vector *v, size_t index, void *out)
{
  if (!v || index >= v->len) {
    return false;
  }
  if (out) {
    memcpy(out, v->data + index * v->elem_size, v->elem_size);
  }
  size_t move_bytes = (v->len - index - 1) * v->elem_size;
  memmove(v->data + index * v->elem_size,
          v->data + (index + 1) * v->elem_size, move_bytes);
  v->len--;
  return true;
}
