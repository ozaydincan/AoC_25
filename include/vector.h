
#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h> // bool
#include <stddef.h>  // size_t

typedef struct {
  unsigned char *data; // raw storage
  size_t len;          // number of elements
  size_t cap;          // capacity (in elements)
  size_t elem_size;    // bytes per element
} Vector;

/* Create a vector that stores elements of elem_size bytes.
 * On failure returns {0} (v.data == NULL).
 */
Vector vec_create(size_t elem_size);

/* Free vector storage. Safe to call multiple times.
 * Does NOT free heap memory referenced by elements.
 */
void vec_destroy(Vector *v);

/* Remove all elements (len->0), keep allocated capacity. */
void vec_clear(Vector *v);

/* Ensure capacity for at least min_cap elements. */
bool vec_reserve(Vector *v, size_t min_cap);

/* Resize vector to new_len elements.
 * If growing, new elements are zero-initialized.
 */
bool vec_resize(Vector *v, size_t new_len);

/* Push one element at the end (copies elem_size bytes from elem). */
bool vec_push(Vector *v, const void *elem);

/* Append n elements from contiguous array elems (n * elem_size bytes). */
bool vec_extend(Vector *v, const void *elems, size_t n);

/* Pop last element. If out != NULL copy removed element into out. */
bool vec_pop(Vector *v, void *out);

/* Get pointer to element at index (modifiable), or NULL if out of range. */
void *vec_get(Vector *v, size_t index);

/* Get pointer to element at index (const), or NULL if out of range. */
const void *vec_get_const(const Vector *v, size_t index);

/* Set element at index (copies elem_size bytes from elem). */
bool vec_set(Vector *v, size_t index, const void *elem);

/* Insert element at index (0..len), shifting right. */
bool vec_insert(Vector *v, size_t index, const void *elem);

/* Remove element at index, shifting left. If out != NULL copy removed element.
 */
bool vec_remove(Vector *v, size_t index, void *out);

/* Typed access convenience:
 *   int *p = VEC_AT(int, &v, i);
 */
#define VEC_AT(T, VEC_PTR, IDX) ((T *)vec_get((VEC_PTR), (IDX)))

#endif /* VECTOR_H */
