/*
 * File: file.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * Provides structs, macros and function for leading with files.
 */
#pragma once

#include <stdbool.h>

bool file_exist(const char *name);
bool file_can_read(const char *name);
bool file_can_write(const char *name);
