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
  u32 maxx = getmaxx(w) - 1;
  u32 maxy = getmaxy(w) - 1;
  u8 encoding = get_char_encoding(&s->buffer->current->buffer[s->col]);

  if (s->col + encoding > s->buffer->current->buffer_len) {
    if (s->row == get_rows(s) - 1) {
      return;
    }

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

void cursor_left(Section *s, WINDOW *w) {
  u32 begx = getbegx(w);
  u32 maxx = getmaxx(w) - 1;
  u32 maxy = getmaxy(w) - 1;
  u8 encoding = get_char_encoding(&s->buffer->current->buffer[s->col]);

  if (s->col < encoding) {    
    if (s->row == 0) {
      return;
    }

    s->buffer->current = s->buffer->current->prev;

    s->row--;
    s->cx = maxx;
    s->col = s->buffer->current->buffer_len + 1;
    s->beg_col = s->buffer->current->buffer_len + 1 - begx;

    if (s->cy == 0) {
      text_up(s, w);
    }
    else {
      s->cy--;
    }
  }
  else {
    if (s->cx == 0) {
      s->beg_col--;
    }
    else {
      s->cx--;
    }
    s->col -= encoding;
  }

  cursor_set(s, w, s->cy, s->cx);
}
