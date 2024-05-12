/*
 * File: ui.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#pragma once

#include <ncurses.h>

typedef struct Windows {
  WINDOW *text;
  WINDOW *rows;
  WINDOW *status;
  WINDOW *command;
} Windows;

void init_ui(void);
Windows *windows_init(void);
void windows_end(Windows *w);
