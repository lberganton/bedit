/*
 * File: section.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * Basic things for a Bedit section.
 */
#pragma once

#include "defs.h"
#include "file.h"

typedef struct Section {
  char *file_name;
  bool dirty;

  u32 row;
  u32 col;
} Section;

Section *section_unamed(void);
Section *section_open(char *file);
