/*
 * File: window.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "defs.h"
#include "ui.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

Windows *windows_init(void) {
  Windows *new = (Windows *)malloc(sizeof(Windows));
  ABORT(new == NULL, "Erro: Falha ao alocar memória para janelas.");

  new->command = newwin(1, COLS, LINES - 1, 0);
  new->status = newwin(1, 0, LINES - 2, 0);
  new->rows = newwin(LINES - 2, 4, 0, 0);
  new->text = newwin(LINES - 2, COLS - 4, 0, 4);

  keypad(new->command, true);
  keypad(new->status, true);
  keypad(new->rows, true);
  keypad(new->text, true);

  wrefresh(new->command);
  wrefresh(new->status);
  wrefresh(new->rows);
  wrefresh(new->text);

  return new;
}

void windows_end(Windows *w) {
  wclear(w->command);
  wclear(w->status);
  wclear(w->rows);
  wclear(w->text);

  wrefresh(w->command);
  wrefresh(w->status);
  wrefresh(w->rows);
  wrefresh(w->text);

  delwin(w->command);
  delwin(w->status);
  delwin(w->rows);
  delwin(w->text);

  free(w);
}

static void paint_command_bar(WINDOW *w) {
  for (int i = 0; i < COLS; i++) {
    mvwaddch(w, 0, i, ' ' | COLOR_PAIR(PAIR_BACKGROUND));
  }
}

static void paint_status_bar(Section *s, WINDOW *w) {
  // Paint the background of status bar.
  for (int i = 0; i < COLS; i++) {
    mvwaddch(w, 0, i, ' ' | COLOR_PAIR(PAIR_STATUS));
  }

  char buffer[32];
  attr_t attr = 0;

  // Select what color is gonna be used, based on the actual mode.
  switch (s->mode) {
  case MODE_COMMAND:
  case MODE_NORMAL:
    sprintf(buffer, "  NORMAL  ");
    attr = COLOR_PAIR(PAIR_BLUE);
    break;
  case MODE_INSERT:
    sprintf(buffer, " INSERÇÃO ");
    attr = COLOR_PAIR(PAIR_TEAL);
    break;
  }

  // Print the actual mode.
  wattrset(w, attr | A_REVERSE);
  mvwprintw(w, 0, 0, "%s", buffer);

  // Print the actual file.
  wattrset(w, attr);
  mvwprintw(w, 0, 10, " %.*s%s ", STATUS_BAR_FILE_NAME_LEN, s->file_name,
            strlen(s->file_name) > STATUS_BAR_FILE_NAME_LEN ? "..." : "");

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
  wattrset(w, attr);
  mvwprintw(w, 0, COLS - 9, " %3" PRIu32 ":%-3" PRIu32 " ", s->row, s->col);
}

static void paint_rows(Section *s, WINDOW *w) {
  BufferNode *aux = s->buffer->begin;
  int i = 0;
  int row = 0;

  wattrset(w, COLOR_PAIR(PAIR_ROW_NUMBER));

  // Loop that compares wheter the maximum of y have been rechead, or all the
  // rows of the buffer has been printed.
  while (i < getmaxy(w) && (size_t)row < s->buffer->nodes) {
    char str[16];

    // Print the row number
    sprintf(str, "%3d ", s->top_row + row++);

    // Check if the actual row is where the user is.
    if (s->top_row + i == s->row) {
      wattrset(w, COLOR_PAIR(PAIR_SELECTED_ROW_NUMBER));
      mvwprintw(w, i, 0, "%s", str);
      wattrset(w, COLOR_PAIR(PAIR_ROW_NUMBER));
    } else {
      mvwprintw(w, i, 0, "%s", str);
    }

    i++;

    // Check if the current node buffer is larger than terminal columns
    // terminal. If it is, print the buffer in separated lines without the line
    // Number
    int len = aux->buffer_len;
    while (len > COLS - getmaxx(w) && i < getmaxy(w)) {
      mvwprintw(w, i, 0, "    ");
      i++;
      len -= COLS - getmaxx(w);
    }

    aux = aux->next;
  }

  while (i < getmaxy(w)) {
    mvwprintw(w, i, 0, "%-4c", '~');
    i++;
  }
}

void paint_text(Section *s, WINDOW *w) {
  BufferNode *node = s->buffer->begin;
  int maxx = getmaxx(w);
  int maxy = getmaxy(w);

  for (int i = 0; i < maxy; i++) {
    for (int j = 0; j < maxx; j++) {
      mvwaddch(w, i, j, ' ' | COLOR_PAIR(PAIR_TEXT));
    }
  }

  int i = 0;

  while (i < maxy && node) {
    size_t len = 0;
    int x = 0;

    while (len < node->buffer_len && i < maxy) {
      if (x > COLS - getbegx(w) - 1) {
        i++;
        x = 0;
      }
      mvwaddch(w, i, x, node->buffer[len] | COLOR_PAIR(PAIR_TEXT));
      len++;
      x++;
    }

    i++;
    node = node->next;
  }
}

void paint_windows(Section *s, Windows *w) {
  paint_command_bar(w->command);
  paint_status_bar(s, w->status);
  paint_rows(s, w->rows);
  paint_text(s, w->text);

  wrefresh(w->command);
  wrefresh(w->status);
  wrefresh(w->rows);
  wrefresh(w->text);
}
