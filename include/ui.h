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
#include <stdbool.h>

#define KEY_ESC 27

typedef enum Palletes {
  PALETTE_TEXT = 8,
  PALETTE_BASE,
  PALETTE_SURFACE_0,
  PALETTE_SURFACE_1,
  PALETTE_LAVENDER,
  PALETTE_BLUE,
  PALETTE_TEAL
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

void initialize_colors(void);

void initialize_windows(Section *s);
void free_windows(Section *s);
void refresh_windows(Section *s);
void resize_windows(Section *s);

int get_key(void);

void paint_background(WINDOW *w, attr_t attr);
void paint_command_bar(char *msg, attr_t attr, Section *s);
void paint_status_bar(Mode mode, Section *s);
void paint_rows(Section *s);

void cursor_up(Section *s);
void cursor_down(Section *s);
void cursor_left(Section *s);
void cursor_right(Section *s);
void cursor_home(Section *s);
void cursor_end(Section *s);
void cursor_pgup(Section *s);
void cursor_pgdown(Section *s);
void cursor_nextword(Section *s);
void cursor_prevword(Section *s);
