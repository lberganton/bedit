/*
 * File: window.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "defs.h"
#include "ui.h"
#include <stdlib.h>

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

void paint_windows(u32 row, u32 top_row, Windows *w, Buffer *b) {
  int i;

  for (i = 0; i < COLS; i++) {
    mvwaddch(w->command, 0, i, ' ' | COLOR_PAIR(PAIR_BACKGROUND));
  }

  for (i = 0; i < COLS; i++) {
    mvwaddch(w->status, 0, i, ' ' | COLOR_PAIR(PAIR_STATUS));
  }

  wattrset(w->rows, COLOR_PAIR(PAIR_ROW_NUMBER));
  for (i = 0; (size_t)i < b->nodes; i++) {
    char str[16];
    sprintf(str, "%3d", top_row + i);

    if (top_row + i == row) {
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
