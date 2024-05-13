/*
 * File: ui.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "ui.h"
#include "defs.h"
#include <ncurses.h>

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

static void paint_background(WINDOW *w, ColorPair color) {
  for (int i = 0; i < getmaxy(w); i++) {
    for (int j = 0; j < getmaxx(w); j++) {
      mvwaddch(w, i, j, ' ' | COLOR_PAIR(color));
    }
  }
}

void paint_command_bar(Section *s, Windows *w) {
  paint_background(w->command, PAIR_BACKGROUND);
}

void paint_status_bar(char *mode, Section *s, Windows *w) {
  paint_background(w->status, PAIR_STATUS);

  ColorPair color = mode == MODE_INSERT ? PAIR_TEAL : PAIR_BLUE;

  
}

void paint_rows(Section *s, Windows *w) {

}

