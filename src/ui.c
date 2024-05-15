/*
 * File: ui.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "ui.h"
#include "defs.h"
#include <ncurses.h>
#include <string.h>

void ui_init(void) {
  initscr();

  ABORT(!has_colors() || !can_change_color(),
        "O terminal atual não suporta cores.");

  cbreak();
  noecho();
  curs_set(false);

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

  refresh();
}

void ui_end(void) { endwin(); }

static u8 paint_char(WINDOW *w, u32 y, u32 x, attr_t attr, char *ch) {
  u8 encoding = get_char_encoding(ch);

  if (encoding == 1) {
    mvwaddch(w, y, x, *ch | attr);
    return encoding;
  }

  attr_t temp;
  wattr_get(w, &temp, NULL, NULL);

  wattrset(w, attr);
  mvwprintw(w, y, x, "%.*s", encoding, ch);

  wattrset(w, temp);
  return encoding;
}

static void paint_string(WINDOW *w, u32 y, u32 x, attr_t attr, size_t len,
                         char *str) {
  for (size_t i = 0; str[i] && i < len; i++) {
    paint_char(w, y, x++, attr, &str[i]);
  }
}

static void paint_background(WINDOW *w, attr_t attr) {
  for (int i = 0; i < getmaxy(w); i++) {
    for (int j = 0; j < getmaxx(w); j++) {
      mvwaddch(w, i, j, ' ' | attr);
    }
  }
}

void paint_command_bar(char *msg, attr_t attr, WINDOW *w) {
  paint_background(w, COLOR_PAIR(PAIR_BACKGROUND));

  if (msg == NULL) {
    return;
  }

  paint_string(w, 0, 0, attr, COLS, msg);
}

void paint_status_bar(char *mode, Section *s, WINDOW *w) {
  paint_background(w, COLOR_PAIR(PAIR_STATUS));
  char buffer[64];

  // Set color scheme.
  attr_t color;

  if (strncmp(mode, MODE_INSERT, 32) == 0) {
    color = COLOR_PAIR(PAIR_TEAL);
  } else {
    color = COLOR_PAIR(PAIR_BLUE);
  }

  // Print the actual mode.
  paint_string(w, 0, 0, color | A_REVERSE, 16, mode);

  // Print the actual file.
  snprintf(buffer, 64, " %.*s%s ", STATUS_BAR_FILE_NAME_LEN, s->file_name,
           strlen(s->file_name) > STATUS_BAR_FILE_NAME_LEN ? "..." : "");

  paint_string(w, 0, 8, color, STATUS_BAR_FILE_NAME_LEN, buffer);

  if (s->dirty) {
    paint_string(w, 0, 8 + strlen(buffer), COLOR_PAIR(PAIR_STATUS), 16,
                 " [Modificado] ");
  }

  // Select the string to represent the file format.
  switch (s->file_extension) {
  case EXTENSION_UNKNOWN:
    strcpy(buffer, FILE_EXTENSION_UNKNOWN);
    break;
  case EXTENSION_TXT:
    strcpy(buffer, FILE_EXTENSION_TXT);
    break;
  case EXTENSION_C:
    strcpy(buffer, FILE_EXTENSION_C);
    break;
  case EXTENSION_CPP:
    strcpy(buffer, FILE_EXTENSION_CPP);
    break;
  }

  // Print the file format.
  wattrset(w, COLOR_PAIR(PAIR_STATUS));
  mvwprintw(w, 0, COLS - 20 - strlen(buffer), " Formato: %s ", buffer);

  // Print the lines and colunes coordinates.
  wattrset(w, color);
  mvwprintw(w, 0, COLS - 9, " %3" PRIu32 ":%-3" PRIu32 " ", s->row, s->col);
}

void paint_rows(Section *s, WINDOW *rows, WINDOW *text) {
  u32 n = 0;
  u32 y = 0;
  u32 x = 0;
  bool print_num = true;

  u32 len = 3;
  u32 maxy = getmaxy(rows);
  u32 begx = getbegx(text);

  char buffer[16];

  BufferNode *node = s->buffer->top;
  u32 pos = 0;

  // Runs the loop until it has printed all lines or until it reaches the
  // y-coordinate limit
  while (y < maxy && n < s->buffer->nodes) {
    // Check if is the next number can be printed or is a continuation of the
    // previous row.
    if (print_num)
      snprintf(buffer, 16, "%*" PRIu32 " ", len, s->top_row + n);
    else
      snprintf(buffer, 16, "%*c ", len, ' ');

    attr_t attr_row, attr_text;

    // Verify that the row in this iteration is the user's current row and sets
    // the appropriate attributes.
    if (s->top_row + n == s->cy + 1) {
      attr_row = COLOR_PAIR(PAIR_SELECTED_ROW_NUMBER);
      attr_text = COLOR_PAIR(PAIR_SELECTED_ROW);
    } else {
      attr_row = COLOR_PAIR(PAIR_ROW_NUMBER);
      attr_text = COLOR_PAIR(PAIR_TEXT);
    }

    // Paint the row number.
    paint_string(rows, y, 0, attr_row, 16, buffer);

    // Print the characters of the row until the row ends or the x maximum is
    // reached.
    while (pos < node->buffer_len && x < COLS - begx) {
      u8 encoding = paint_char(text, y, x, attr_text, &node->buffer[pos]);
      pos += encoding;
      x++;
    }

    // Check if the x limit has not been reached.
    if (x < COLS - begx) {
      // Paints the remaining background of the row.
      while (x < COLS - begx) {
        char ch = ' ';
        paint_char(text, y, x, attr_text, &ch);
        x++;
      }

      print_num = true;
      node = node->next;
      pos = 0;
      n++;
    } else {
      print_num = false;
    }

    x = 0;
    y++;
  }

  // Verify that all rows has been printed and the y's limit has not been
  // reached.
  while (y < maxy) {
    // Put the '~' in a empty row.
    snprintf(buffer, 16, "%-*c ", len, '~');
    paint_string(rows, y, 0, COLOR_PAIR(PAIR_ROW_NUMBER), 16, buffer);

    // Paint the background.
    for (int x = 0; x < COLS; x++) {
      char ch = ' ';
      paint_char(text, y, x, COLOR_PAIR(PAIR_BACKGROUND), &ch);
    }

    y++;
  }
}

void refresh_windows(Windows *s) {
  wrefresh(s->command);
  wrefresh(s->rows);
  wrefresh(s->status);
  wrefresh(s->text);
}
