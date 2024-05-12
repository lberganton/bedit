/*
 * File: file.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "file.h"
#include "unistd.h"

bool file_exist(const char *name) {
  return !access(name, F_OK);
}

bool file_can_read(const char *name) {
  return !access(name, R_OK);
}

bool file_can_write(const char *name) {
  return !access(name, W_OK);
}

