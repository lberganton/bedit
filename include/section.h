/*
 * File: section.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * Basic things for a Bedit section.
 */
#pragma once

#include "buffer.h"
#include "defs.h"
#include "file.h"

typedef enum Mode { MODE_NORMAL, MODE_INSERT, MODE_COMMAND } Mode;

typedef struct Section {
  char *file_name;
  bool dirty;
  bool unamed;
  FileExtension file_extension;

  Buffer *buffer;
  u32 top_row;
  u32 cy;
  u32 cx;

  Mode mode;
} Section;

Section *section_unamed(void);
Section *section_open(char *file);
void mode_normal(Section *s);
void mode_insert(Section *s);
void mode_command(Section *s);
