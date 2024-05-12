/*
 * File: window.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "defs.h"
#include "ui.h"
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

Windows *windows_init(void) {
  Windows *new = (Windows *)malloc(sizeof(Windows));
  ABORT(new == NULL, "Erro: Falha ao alocar memória para janelas.");

  new->command = newwin(1, COLS, LINES - 1, 0);
  new->status = newwin(1, 0, LINES - 2, 0);
  new->rows = newwin(LINES - 2, 3, 0, 0);
  new->text = newwin(LINES - 2, COLS - 3, 0, 3);

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
  for (int i = 0; i < COLS; i++) {
    mvwaddch(w, 0, i, ' ' | COLOR_PAIR(PAIR_STATUS));
  }

  char str[32];
  attr_t attr = 0;

  switch (s->mode) {
  case MODE_COMMAND:
  case MODE_NORMAL:
    sprintf(str, "  NORMAL  ");
    attr = COLOR_PAIR(PAIR_BLUE);
    break;
  case MODE_INSERT:
    sprintf(str, " INSERÇÃO ");
    attr = COLOR_PAIR(PAIR_TEAL);
    break;
  }

  wattrset(w, attr | A_REVERSE);
  mvwprintw(w, 0, 0, "%s", str);

  wattrset(w, attr);
  mvwprintw(w, 0, 10, " %.29s%s ", s->file_name,
            strlen(s->file_name) > 29 ? "..." : "");

  switch (s->file_extension) {
  case EXTENSION_UNKNOWN:
    strcpy(str, FILE_EXTENSION_UNKNOWN);
    break;
  case EXTENSION_TXT:
    strcpy(str, FILE_EXTENSION_TXT);
    break;
  case EXTENSION_C:
    strcpy(str, FILE_EXTENSION_C);
    break;
  case EXTENSION_CPP:
    strcpy(str, FILE_EXTENSION_CPP);
    break;
  }

  wattrset(w, COLOR_PAIR(PAIR_STATUS));
  mvwprintw(w, 0, COLS - 20 - strlen(str), " Formato: %s ", str);

  wattrset(w, attr);
  mvwprintw(w, 0, COLS - 9, " %3" PRIu32 ":%-3" PRIu32 " ", s->row, s->col);
}

void paint_windows(Section *s, Windows *w) {
  int i;

  paint_command_bar(w->command);
  paint_status_bar(s, w->status);

  wattrset(w->rows, COLOR_PAIR(PAIR_ROW_NUMBER));
  for (i = 0; (size_t)i < s->buffer->nodes; i++) {
    char str[16];
    sprintf(str, "%3d", s->top_row + i);

    if (s->top_row + i == s->row) {
      wattrset(w->rows, COLOR_PAIR(PAIR_SELECTED_ROW_NUMBER));
      mvwprintw(w->rows, i, 0, "%s", str);
      wattrset(w->rows, COLOR_PAIR(PAIR_ROW_NUMBER));
    } else {
      mvwprintw(w->rows, i, 0, "%s", str);
    }
  }

  while (i < getmaxy(w->rows)) {
    mvwprintw(w->rows, i, 0, "%3c", '~');
    i++;
  }

  wrefresh(w->command);
  wrefresh(w->status);
  wrefresh(w->rows);
  wrefresh(w->text);
}
