/*
 * File: char.c
 * Author: Lucas Berganton
 * Created: 05/18/2024
 */
#include "char.h"
#include <ncurses.h>

void paint_char(WINDOW *w, u32 y, u32 x, attr_t attr, wchar_t ch) {
  attr_t temp;
  wattr_get(w, &temp, NULL, NULL);
  wattrset(w, attr);

  mvwprintw(w, y, x, "%lc", ch);

  wattrset(w, temp);
}

void paint_string(WINDOW *w, u32 y, u32 x, attr_t attr, size_t len,
                  const char *str) {
  attr_t temp;
  wattr_get(w, &temp, NULL, NULL);
  wattrset(w, attr);

  mvwprintw(w, y, x, "%s", str);

  wattrset(w, temp);
}

void paint_wcstring(WINDOW *w, u32 y, u32 x, attr_t attr, size_t len,
                    const wchar_t *str) {
  for (size_t i = 0; str[i] && i < len; i++) {
    paint_char(w, y, x++, attr, str[i]);
  }
}
