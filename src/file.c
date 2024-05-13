/*
 * File: file.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "file.h"
#include "unistd.h"
#include <string.h>

bool file_exist(const char *name) { return !access(name, F_OK); }

bool file_can_read(const char *name) { return !access(name, R_OK); }

bool file_can_write(const char *name) { return !access(name, W_OK); }

FileExtension file_get_extension(const char *name) {
  size_t limit = 16;
  char *aux;
  char str[128];

  strncpy(str, name, 128);

  char *format = strtok(str, ".");

  while ((aux = strtok(NULL, "."))) {
    format = aux;
  }

  if (format == NULL) {
    return EXTENSION_UNKNOWN;
  }
  if (strncmp(format, "txt", limit) == 0) {
    return EXTENSION_TXT;
  }
  if (strncmp(format, "c", limit) == 0) {
    return EXTENSION_C;
  }
  if (strncmp(format, "cpp", limit) == 0) {
    return EXTENSION_CPP;
  }

  return EXTENSION_UNKNOWN;
}
