/*
 * File: ui.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "ui.h"
#include "defs.h"
#include <ncurses.h>
#include <string.h>

void initialize_colors(void) {
  ASSERT(!has_colors() || !can_change_color(),
         "O terminal atual não suporta cores.");

  start_color();

  init_color(PALETTE_TEXT, 800, 835, 953);
  init_color(PALETTE_BASE, 117, 117, 179);
  init_color(PALETTE_SURFACE_0, 191, 195, 265);
  init_color(PALETTE_SURFACE_1, 269, 277, 351);
  init_color(PALETTE_LAVENDER, 703, 742, 992);
  init_color(PALETTE_BLUE, 535, 710, 976);
  init_color(PALETTE_TEAL, 578, 882, 832);

  init_pair(PAIR_TEXT, PALETTE_TEXT, PALETTE_BASE);
  init_pair(PAIR_BACKGROUND, PALETTE_SURFACE_0, PALETTE_BASE);
  init_pair(PAIR_SELECTED_ROW, PALETTE_TEXT, PALETTE_SURFACE_0);
  init_pair(PAIR_ROW_NUMBER, PALETTE_SURFACE_1, PALETTE_BASE);
  init_pair(PAIR_SELECTED_ROW_NUMBER, PALETTE_LAVENDER, PALETTE_BASE);
  init_pair(PAIR_STATUS, PALETTE_BLUE, PALETTE_SURFACE_1);
  init_pair(PAIR_BLUE, PALETTE_BLUE, PALETTE_BASE);
  init_pair(PAIR_TEAL, PALETTE_TEAL, PALETTE_BASE);
}

void paint_char(WINDOW *w, u32 y, u32 x, attr_t attr, wchar_t ch) {
  attr_t temp;
  wattr_get(w, &temp, NULL, NULL);
  wattrset(w, attr);

  mvwprintw(w, y, x, "%lc", ch);

  wattrset(w, temp);
}

void paint_string(WINDOW *w, u32 y, u32 x, attr_t attr, const char *str) {
  attr_t temp;
  wattr_get(w, &temp, NULL, NULL);
  wattrset(w, attr);

  mvwprintw(w, y, x, "%s", str);

  wattrset(w, temp);
}

void paint_background(WINDOW *w, attr_t attr) {
  for (int i = 0; i < getmaxy(w); i++) {
    for (int j = 0; j < getmaxx(w); j++) {
      mvwaddch(w, i, j, ' ' | attr);
    }
  }
}

void paint_command_bar(char *msg, attr_t attr, Section *s) {
  paint_background(s->window_command, COLOR_PAIR(PAIR_BACKGROUND));

  if (msg == NULL) {
    return;
  }

  paint_string(s->window_command, 0, 0, attr, msg);
}

void paint_status_bar(Section *s) {
  char buffer[BUFF_STR];
  WINDOW *w = s->window_status;
  static const char *mode_string[] = {" NORMAL ", " INSERÇÃO ", " NORMAL "};

  paint_background(w, COLOR_PAIR(PAIR_STATUS));

  // Set color scheme.
  attr_t color;

  if (s->mode == MODE_INSERT) {
    color = COLOR_PAIR(PAIR_TEAL);
  } else {
    color = COLOR_PAIR(PAIR_BLUE);
  }

  // Print the actual mode.
  paint_string(w, 0, 0, color | A_REVERSE, mode_string[s->mode]);

  // Print the actual file.
  snprintf(buffer, 64, " %.*s%s ", STATUS_BAR_FILE_NAME_LEN,
           s->unamed ? NO_NAME_FILE : s->file_name,
           strlen(s->file_name) > STATUS_BAR_FILE_NAME_LEN ? "..." : "");

  paint_string(w, 0, getcurx(w), color, buffer);

  if (s->dirty) {
    paint_string(w, 0, getcurx(w), COLOR_PAIR(PAIR_STATUS), " [Modificado] ");
  }

  // Select the string to represent the file format.
  switch (s->file_extension) {
  case EXTENSION_UNKNOWN:
    strncpy(buffer, FILE_EXTENSION_UNKNOWN, BUFF_STR);
    break;
  case EXTENSION_TXT:
    strncpy(buffer, FILE_EXTENSION_TXT, BUFF_STR);
    break;
  case EXTENSION_C:
    strncpy(buffer, FILE_EXTENSION_C, BUFF_STR);
    break;
  case EXTENSION_CPP:
    strncpy(buffer, FILE_EXTENSION_CPP, BUFF_STR);
    break;
  case EXTENSION_CHEADER:
    strncpy(buffer, FILE_EXTENSION_CHEADER, BUFF_STR);
    break;
  case EXTENSION_CSHARP:
    strncpy(buffer, FILE_EXTENSION_CSHARP, BUFF_STR);
    break;
  case EXTENSION_JAVA:
    strncpy(buffer, FILE_EXTENSION_JAVA, BUFF_STR);
    break;
  case EXTENSION_JAVASCRIPT:
    strncpy(buffer, FILE_EXTENSION_JAVASCRIPT, BUFF_STR);
    break;
  case EXTENSION_RUST:
    strncpy(buffer, FILE_EXTENSION_RUST, BUFF_STR);
    break;
  case EXTENSION_PYTHON:
    strncpy(buffer, FILE_EXTENSION_PYTHON, BUFF_STR);
    break;
  }

  // Print the file format.
  wattrset(w, COLOR_PAIR(PAIR_STATUS));
  mvwprintw(w, 0, COLS - 20 - strlen(buffer), " Formato: %s ", buffer);

  // Print the lines and colunes coordinates.
  wattrset(w, color);
  mvwprintw(w, 0, COLS - 9, " %3" PRIu32 ":%-3" PRIu32 " ", s->row + 1,
            s->col + 1);
}

void paint_rows(Section *s) {
  WINDOW *rows = s->window_rows;
  WINDOW *text = s->window_text;

  u32 n = s->beg_row;
  u32 y = 0;
  u32 x = 0;

  u32 len = 3;
  u32 maxy = getmaxy(rows);
  u32 maxx = getmaxx(text);

  char buffer[16];

  BufferNode *node = s->buffer->top;
  u32 pos = s->beg_col;

  // Runs the loop until it has printed all lines or until it reaches the
  // y-coordinate limit
  while (y < maxy && n < s->rows) {
    snprintf(buffer, 16, "%*" PRIu32 " ", len, 1 + s->beg_row + y);

    attr_t attr_row, attr_text;

    // Verify that the row in this iteration is the user's current row and sets
    // the appropriate attributes.
    if (s->beg_row + y == s->row) {
      attr_row = COLOR_PAIR(PAIR_SELECTED_ROW_NUMBER);
      attr_text = COLOR_PAIR(PAIR_SELECTED_ROW);
    } else {
      attr_row = COLOR_PAIR(PAIR_ROW_NUMBER);
      attr_text = COLOR_PAIR(PAIR_TEXT);
    }

    // Paint the row number.
    paint_string(rows, y, 0, attr_row, buffer);

    // Print the characters of the row until the row ends or the x maximum is
    // reached.
    while (pos < node->string_length && x < maxx) {
      paint_char(text, y, x, attr_text, node->vector[pos]);
      pos++;
      x++;
    }

    // Paints the remaining background of the row.
    while (x < maxx) {
      wchar_t ch = ' ';
      paint_char(text, y, x, attr_text, ch);
      x++;
    }

    node = buffer_valid_next(node);
    n++;
    pos = s->beg_col;

    x = 0;
    y++;
  }

  // Verify that all rows has been printed and the y's limit has not been
  // reached.
  while (y < maxy) {
    // Put the '~' in a empty row.
    snprintf(buffer, 16, "%-*c ", len, '~');
    paint_string(rows, y, 0, COLOR_PAIR(PAIR_ROW_NUMBER), buffer);

    // Paint the background.
    for (int x = 0; x < COLS; x++) {
      wchar_t ch = ' ';
      paint_char(text, y, x, COLOR_PAIR(PAIR_BACKGROUND), ch);
    }

    y++;
  }
}
