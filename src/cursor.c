#include "section.h"
#include <ncurses.h>

void cursor_set(Section *s, WINDOW *w, u32 y, u32 x) {
  s->cx = x;
  s->cy = y;
  wmove(w, y, x);
}

void cursor_right(Section *s, WINDOW *w) {
  u8 encoding = get_char_encoding(&s->buffer->current->buffer[s->col - 1]);

  if (s->col + encoding > s->buffer->current->buffer_len + 1) {
    if (s->row == get_rows(s)) {
      return;
    }
  }

  s->col += encoding;

  cursor_set(s, w, s->cy, s->cx + 1);
}
