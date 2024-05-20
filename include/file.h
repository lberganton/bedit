/*
 * File: file.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * Provides structs, macros and function for leading with files.
 */
#pragma once

#include <stdbool.h>
#include "defs.h"

typedef enum FileExtension {
  EXTENSION_UNKNOWN,
  EXTENSION_TXT,
  EXTENSION_C,
  EXTENSION_CPP,
} FileExtension;

bool file_exist(const char *name);
bool file_can_read(const char *name);
bool file_can_write(const char *name);
void file_get_input(const char *input, char *buff);
void file_get_name(const char *input, char *buff);
void file_get_directory(const char *name, char *buff);
FileExtension file_get_extension(const char *name);
u32 file_get_size(const char *input);
