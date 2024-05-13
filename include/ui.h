/*
 * File: ui.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#pragma once

#include "buffer.h"
#include "defs.h"
#include "section.h"
#include <ncurses.h>

#define KEY_ESC 27

typedef enum Palletes {
  PALETTE_TEXT = 8,  // Text
  PALETTE_BASE,      // Background
  PALETTE_SURFACE_0, // Selected row
  PALETTE_SURFACE_1, // Row number
  PALETTE_LAVENDER,  // Selected row number
  PALETTE_BLUE,      // Blue
  PALETTE_TEAL       // Green
} Palletes;

typedef enum ColorPair {
  PAIR_TEXT = 1,
  PAIR_BACKGROUND,
  PAIR_SELECTED_ROW,
  PAIR_ROW_NUMBER,
  PAIR_SELECTED_ROW_NUMBER,
  PAIR_STATUS,
  PAIR_BLUE,
  PAIR_TEAL
} ColorPair;

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

int get_key(void);
void paint_windows(Section *s, Windows *w);

void paint_char(WINDOW *w, u32 y, u32 x, char ch, ColorPair color);

void paint_background(WINDOW *w, ColorPair color);
void paint_command_bar(Section *s, Windows *w);
void paint_status_bar(Section *s, Windows *w);
void paint_rows(Section *s, Windows *w);
