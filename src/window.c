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

void initialize_windows(Section *s) {
  s->window_command = newwin(1, COLS, LINES - 1, 0);
  s->window_status = newwin(1, COLS, LINES - 2, 0);
  s->window_rows = newwin(LINES - 2, 4, 0, 0);
  s->window_text = newwin(LINES - 2, COLS - 4, 0, 4);

  keypad(s->window_command, true);
  keypad(s->window_status, true);
  keypad(s->window_rows, true);
  keypad(s->window_text, true);

  wrefresh(s->window_command);
  wrefresh(s->window_status);
  wrefresh(s->window_rows);
  wrefresh(s->window_text);
}

void free_windows(Section *s) {
  wclear(s->window_command);
  wclear(s->window_status);
  wclear(s->window_rows);
  wclear(s->window_text);

  wrefresh(s->window_command);
  wrefresh(s->window_status);
  wrefresh(s->window_rows);
  wrefresh(s->window_text);

  delwin(s->window_command);
  delwin(s->window_status);
  delwin(s->window_rows);
  delwin(s->window_text);
}

void refresh_windows(Section *s) {
  refresh();
  wrefresh(s->window_text);
  wrefresh(s->window_rows);
  wrefresh(s->window_status);
  wrefresh(s->window_command);
}
