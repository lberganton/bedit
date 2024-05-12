/*
 * File: ui.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "ui.h"
#include "defs.h"
#include <ncurses.h>

void ui_init(void) {
  inistcr();
  ABORT(!has_colors() || !can_change_color(),
        "O terminal atual não suporta cores.");
  
  init_color(PALETTE_TEXT, 800, 835, 953);
  init_color(PALETTE_BASE, 117, 117, 179);
  init_color(PALETTE_SURFACE_0, 191, 195, 265);
  init_color(PALETTE_SURFACE_1, 269, 277, 351);
  init_color(PALETTE_LAVENDER, 703, 742, 992);
  init_color(PALETTE_BLUE, 535, 710, 976);
  init_color(PALETTE_TEAL, 578, 882, 832);

  init_pair(PAIR_TEXT, PALETTE_TEXT, PALETTE_BASE);
  init_pair(PAIR_BACKGROUND, PALETTE_BASE, PALETTE_BASE);
  init_pair(PAIR_SELECTED_ROW, PALETTE_TEXT, PALETTE_SURFACE_0);
  init_pair(PAIR_ROW_NUMBER, PALETTE_SURFACE_1, PALETTE_BASE);
  init_pair(PAIR_SELECTED_ROW_NUMBER, PALETTE_LAVENDER, PALETTE_BASE);
  init_pair(PAIR_BACKGROUND, PALETTE_BLUE, PALETTE_BASE);
  init_pair(PAIR_TEAL, PALETTE_TEAL, PALETTE_BASE);
}

void ui_end(void) {
  endwin();
}
