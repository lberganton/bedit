/*
 * File: ui.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#pragma once

#include <ncurses.h>

typedef enum Palletes {
  PALETTE_TEXT = 8,  // Text
  PALETTE_BASE,      // Background
  PALETTE_SURFACE_0, // Selected row
  PALETTE_SURFACE_1, // Row number
  PALETTE_LAVENDER,  // Selected row number
  PALETTE_BLUE,      // Blue
  PALETTE_TEAL       // Green
} Palletes;

typedef enum ColorPairs {
  PAIR_TEXT,
  PAIR_BACKGROUND,
  PAIR_SELECTED_ROW,
  PAIR_ROW_NUMBER,
  PAIR_SELECTED_ROW_NUMBER,
  PAIR_BLUE,
  PAIR_TEAL
} ColorPairs;

typedef struct Windows {
  WINDOW *text;
  WINDOW *rows;
  WINDOW *status;
  WINDOW *command;
} Windows;

void ui_init(void);
void ui_end(void);
Windows *windows_init(void);
void windows_end(Windows *w);
