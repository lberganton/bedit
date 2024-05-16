#include "ui.h"
#include "section.h"
#include <ncurses.h>

void cursor_enable(bool state) {
  curs_set(state);
}

void cursor_set(Section *s, WINDOW *w, u32 y, u32 x) {
  wmove(w, y, x);
}

void cursor_right(Section *s, WINDOW *w) {
  if (s->row == get_rows(s) - 1) {
    return;
  }

  u32 maxx = getmaxx(w) - 1;
  u32 maxy = getmaxy(w) - 1;
  u8 encoding = get_char_encoding(&s->buffer->current->buffer[s->col]);

  if (s->col + encoding > s->buffer->current->buffer_len) {    
    s->row++;
    s->cx = 0;
    s->col = 0;
    s->beg_col = 0;

    if (s->cy == maxy) {
      text_down(s, w);
    }
    else {
      s->cy++;
    }

    s->buffer->current = s->buffer->current->next;
  }
  else {
    if (s->cx == maxx) {
      s->beg_col++;
    }
    else {
      s->cx++;
    }
    s->col += encoding;
  }

  cursor_set(s, w, s->cy, s->cx);
}
