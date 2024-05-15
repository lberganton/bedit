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

#define MODE_NORMAL " NORMAL "
#define MODE_INSERT " INSERÇÃO "
#define MODE_COMMAND MODE_NORMAL

typedef struct Section {
  char *file_name;
  bool dirty;
  bool unamed;
  FileExtension file_extension;

  Buffer *buffer;
  u32 top_row;
  u32 row;
  u32 col;

  u32 cy;
  u32 cx;
} Section;

Section *section_unamed(void);
Section *section_open(char *file);
void mode_normal(Section *s);
void mode_insert(Section *s);
void mode_command(Section *s);
u32 get_rows(Section *s);
