
#ifndef FILEIO_H
#define FILEIO_H

#include <stddef.h>

/* Read entire file into NUL-terminated buffer.
 * Returns malloc'd buffer or NULL on failure.
 */
char *read_file(const char *path);

/* Read file into array of lines.
 * Each line is malloc'd.
 * out_count receives number of lines.
 * Returns NULL on failure.
 */
char **read_lines(const char *path, size_t *out_count);

/* Free array returned by read_lines() */
void free_lines(char **lines, size_t count);

#endif /* FILEIO_H */
