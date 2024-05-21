/*
 * File: section.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#pragma once

#include "buffer.h"
#include "defs.h"
#include "file.h"

typedef struct Windows {
  WINDOW *text;
  WINDOW *rows;
  WINDOW *status;
  WINDOW *command;
} Windows;

typedef struct Section {
  char file_input[BUFF_STR];
  char file_name[BUFF_STR];
  char file_directory[BUFF_STR];
  FileExtension file_extension;
  bool unamed;

  bool dirty;

  Buffer *buffer;

  u32 beg_row;
  u32 beg_col;
  u32 row;
  u32 col;

  u32 cy;
  u32 cx;

  Windows *window;
  char msg[BUFF_STR];
} Section;

Section *section_unamed(void);
Section *section_open(const char *file_name);
u32 get_rows(Section *s);
void text_up(Section *s);
void text_down(Section *s);
void insert_char(Section *s, char ch);
void delete_char(Section *s);
void backspace_char(Section *s);
void insert_new_line(Section *s);
