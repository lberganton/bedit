/*
 * File: section.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#pragma once

#include "buffer.h"
#include "defs.h"
#include "file.h"
#include "undo.h"

typedef enum Mode { MODE_NORMAL, MODE_INSERT } Mode;

typedef struct Section {
  // This is the argument that the user passed in the command line or in the
  // program itself to save the file. It can be either only file name or the
  // full path.
  char file_full[BUFF_STR];

  // If the user do not provide a file name as a argument in the command line,
  // this will be set as 'true', and he will not be able to write in a file
  // before provide a name.
  bool unamed;

  // File name, directory and extension, if the section starts 'unamed', the
  // first two will not be initialized, and the last will be set as 'unknow'.
  char file_name[BUFF_STR];
  char file_path[BUFF_STR];
  FileExtension file_extension;

  // This is the buffer that stores the current message to be displayed on the
  // command bar.
  char msg[BUFF_STR];

  // Editor mode.
  Mode mode;

  // Informs if there is a change not writed in the current file.
  bool dirty;

  // The pointer to the buffer that stores all the characters.
  Buffer *buffer;

  // The total of rows.
  u32 rows;

  // The actual row and column number.
  u32 row;
  u32 col;

  // The number of the row to be displayed in the screen top, and the first
  // column to be displayed. Basically, is where the screen is located on the X
  // and Y axis.
  u32 beg_row;
  u32 beg_col;

  // The cursor coordinates.
  u32 cursor_y;
  u32 cursor_x;

  // In NCurses a delimited part of a screen is called 'window'. This is a
  // pointer to all the windows needed by the editor.
  WINDOW *window_text;
  WINDOW *window_rows;
  WINDOW *window_status;
  WINDOW *window_command;

  // The stack that stores the user's changes.
  UndoStack *undo;

  bool debug;
} Section;

void section_set_msg(Section *s, const char *msg, ...);

void text_up(Section *s);
void text_down(Section *s);

void file_save(Section *s);
void pop_undo(Section *s);
