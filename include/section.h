/*
 * File: section.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#pragma once

#include "buffer.h"
#include "defs.h"
#include "file.h"

#define MODE_NORMAL " NORMAL "
#define MODE_INSERT " INSERÇÃO "
#define MODE_COMMAND MODE_NORMAL

typedef struct Section {
  char *file_input;
  char *file_name;
  char *file_directory;
  bool unamed;
  FileExtension file_extension;

  bool dirty;

  Buffer *buffer;

  u32 beg_row;
  u32 beg_col;
  u32 row;
  u32 col;

  u32 cy;
  u32 cx;
} Section;

Section *section_unamed(void);
Section *section_open(const char *file_name);
void mode_normal(Section *s);
void mode_insert(Section *s);
void mode_command(Section *s);
u32 get_rows(Section *s);
void text_up(Section *s);
void text_down(Section *s);
