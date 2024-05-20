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
  new->status = newwin(1, COLS, LINES - 2, 0);
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

void refresh_windows(Windows *s) {
  refresh();
  wrefresh(s->command);
  wrefresh(s->rows);
  wrefresh(s->status);
  wrefresh(s->text);
}
