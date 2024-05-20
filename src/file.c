/*
 * File: file.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "file.h"
#include "defs.h"
#include "unistd.h"
#include <string.h>

bool file_exist(const char *name) { return !access(name, F_OK); }

bool file_can_read(const char *name) { return !access(name, R_OK); }

bool file_can_write(const char *name) { return !access(name, W_OK); }

void file_get_input(const char *input, char *buff) { strcpy(buff, input); }

void file_get_name(const char *input, char *buff) {
  const char *slash = strrchr(input, '/');
  slash = slash == NULL ? input : slash;
  strncpy(buff, slash + 1, BUFF_SIZE);
}

void file_get_directory(const char *name, char *buff) {
  const char *slash = strrchr(name, '/');
  u32 index = (u32)(slash - name);
  strncpy(buff, name, index);
}

FileExtension file_get_extension(const char *name) {
  const char *dot = strrchr(name, '.');

  if (strcmp(dot, ".txt") == 0) {
    return EXTENSION_TXT;
  }
  if (strcmp(dot, ".c") == 0) {
    return EXTENSION_C;
  }
  if (strcmp(dot, ".cpp") == 0) {
    return EXTENSION_CPP;
  }

  return EXTENSION_UNKNOWN;
}

u32 file_get_size(const char *input) {
  FILE *f = fopen(input, "r");
  ABORT(f == NULL, "Erro: Falha ao determinar tamanho de arquivo.\n");

  fseek(f, 0, SEEK_END);
  return (u32)ftell(f);
}
