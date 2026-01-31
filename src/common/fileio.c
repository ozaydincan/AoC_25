#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(const char *path)
{
  FILE *f = fopen(path, "rb");
  if (!f) {
    return NULL;
  }

  if (fseek(f, 0, SEEK_END) != 0) {
    fclose(f);
    return NULL;
  }

  long size = ftell(f);
  if (size < 0) {
    fclose(f);
    return NULL;
  }

  if (fseek(f, 0, SEEK_SET) != 0) {
    fclose(f);
    return NULL;
  }

  char *buf = (char *)malloc((size_t)size + 1);
  if (!buf) {
    fclose(f);
    return NULL;
  }

  size_t read = fread(buf, 1, (size_t)size, f);
  if (read != (size_t)size && ferror(f)) {
    free(buf);
    fclose(f);
    return NULL;
  }

  buf[read] = '\0';
  fclose(f);
  return buf;
}

char **read_lines(const char *path, size_t *out_count)
{
  if (out_count) {
    *out_count = 0;
  }

  char *buf = read_file(path);
  if (!buf) {
    return NULL;
  }

  size_t len = strlen(buf);
  size_t count = 0;
  for (size_t i = 0; i < len; i++) {
    if (buf[i] == '\n') {
      count++;
    }
  }
  if (len > 0 && buf[len - 1] != '\n') {
    count++;
  }

  char **lines = (char **)calloc(count > 0 ? count : 1, sizeof(char *));
  if (!lines) {
    free(buf);
    return NULL;
  }

  size_t line_idx = 0;
  char *start = buf;
  for (size_t i = 0; i <= len; i++) {
    if (buf[i] == '\n' || buf[i] == '\0') {
      size_t line_len = (size_t)(&buf[i] - start);
      if (line_len > 0 && start[line_len - 1] == '\r') {
        line_len--;
      }
      char *line = (char *)malloc(line_len + 1);
      if (!line) {
        for (size_t j = 0; j < line_idx; j++) {
          free(lines[j]);
        }
        free(lines);
        free(buf);
        return NULL;
      }
      if (line_len > 0) {
        memcpy(line, start, line_len);
      }
      line[line_len] = '\0';
      if (count > 0) {
        lines[line_idx++] = line;
      } else {
        free(line);
      }
      start = &buf[i + 1];
    }
  }

  if (out_count) {
    *out_count = count;
  }
  free(buf);
  return lines;
}

void free_lines(char **lines, size_t count)
{
  if (!lines) {
    return;
  }
  for (size_t i = 0; i < count; i++) {
    free(lines[i]);
  }
  free(lines);
}
