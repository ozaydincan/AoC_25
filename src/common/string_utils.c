#include "string_utils.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char *str_trim(char *s)
{
  if (!s) {
    return NULL;
  }

  char *start = s;
  while (*start && isspace((unsigned char)*start)) {
    start++;
  }

  char *end = start + strlen(start);
  while (end > start && isspace((unsigned char)end[-1])) {
    end--;
  }

  size_t new_len = (size_t)(end - start);
  if (start != s && new_len > 0) {
    memmove(s, start, new_len);
  } else if (start != s && new_len == 0) {
    s[0] = '\0';
    return s;
  }
  s[new_len] = '\0';
  return s;
}

char **str_split(const char *s, char delim, size_t *out_count)
{
  if (out_count) {
    *out_count = 0;
  }
  if (!s) {
    return NULL;
  }

  size_t len = strlen(s);
  size_t count = 1;
  for (size_t i = 0; i < len; i++) {
    if (s[i] == delim) {
      count++;
    }
  }

  char **parts = (char **)malloc(sizeof(char *) * count);
  if (!parts) {
    return NULL;
  }

  size_t idx = 0;
  const char *start = s;
  for (size_t i = 0; i <= len; i++) {
    if (s[i] == delim || s[i] == '\0') {
      size_t part_len = (size_t)(&s[i] - start);
      char *part = (char *)malloc(part_len + 1);
      if (!part) {
        free_split(parts, idx);
        return NULL;
      }
      if (part_len > 0) {
        memcpy(part, start, part_len);
      }
      part[part_len] = '\0';
      parts[idx++] = part;
      start = &s[i + 1];
    }
  }

  if (out_count) {
    *out_count = count;
  }
  return parts;
}

void free_split(char **parts, size_t count)
{
  if (!parts) {
    return;
  }
  for (size_t i = 0; i < count; i++) {
    free(parts[i]);
  }
  free(parts);
}

bool str_startswith(const char *s, const char *prefix)
{
  if (!s || !prefix) {
    return false;
  }
  size_t plen = strlen(prefix);
  return strncmp(s, prefix, plen) == 0;
}

bool str_endswith(const char *s, const char *suffix)
{
  if (!s || !suffix) {
    return false;
  }
  size_t slen = strlen(s);
  size_t tlen = strlen(suffix);
  if (tlen > slen) {
    return false;
  }
  return memcmp(s + (slen - tlen), suffix, tlen) == 0;
}

char *str_dup(const char *s)
{
  if (!s) {
    return NULL;
  }
  size_t len = strlen(s);
  char *out = (char *)malloc(len + 1);
  if (!out) {
    return NULL;
  }
  memcpy(out, s, len + 1);
  return out;
}
