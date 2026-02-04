#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdbool.h>
#include <stddef.h>

/* Remove leading/trailing whitespace in-place */
char *str_trim(char *s);

/* Split string by delimiter.
 * Returns array of malloc'd strings.
 * out_count receives number of tokens.
 */
char **str_split(const char *s, char delim, size_t *out_count);

/* Free result of str_split() */
void free_split(char **parts, size_t count);

/* Check prefix */
bool str_startswith(const char *s, const char *prefix);

/* Check suffix */
bool str_endswith(const char *s, const char *suffix);

/* Duplicate string (portable replacement for strdup) */
char *str_dup(const char *s);

#endif /* STRING_UTILS_H */
