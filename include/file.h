/*
 * File: file.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * Provides structs, macros and function for leading with files.
 */
#pragma once

#include <stdbool.h>

typedef struct FileBuffer {
  char *name;
  bool dirty;
} FileBuffer;
