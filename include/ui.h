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
void refresh_windows(Windows *s);

int get_key(void);

void paint_command_bar(char *msg, attr_t attr, WINDOW *w);
void paint_status_bar(char *mode, Section *s, WINDOW *w);
void paint_rows(Section *s, WINDOW *rows, WINDOW *text);

void cursor_up(Section *s, WINDOW *w);
void cursor_down(Section *s, WINDOW *w);
void cursor_left(Section *s, WINDOW *w);
void cursor_right(Section *s, WINDOW *w);
void cursor_home(Section *s, WINDOW *w);
void cursor_end(Section *s, WINDOW *w);

void text_up(Section *s, WINDOW *w);
void text_down(Section *s, WINDOW *w);
