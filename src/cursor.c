/*
 * File: cursor.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "section.h"
#include "ui.h"
#include <ncurses.h>

void cursor_up(Section *s, WINDOW *w) {
  if (s->row == 0) {
    return;
  }

  u32 maxx = getmaxx(w) - 1;
  u32 begy = getbegy(w);

  if (s->cy == begy) {
    text_up(s, w);
  } else {
    s->cy--;
  }

  s->buffer->current = s->buffer->current->prev;
  s->row--;

  if (s->col > s->buffer->current->buffer_len) {
    s->col = s->buffer->current->buffer_len;

    if (s->col <= s->beg_col) {
      s->beg_col = s->col - 1 % maxx;
    }

    s->cx = s->col - s->beg_col;
  }

  wmove(w, s->cy, s->cx);
}

void cursor_down(Section *s, WINDOW *w) {
  if (s->row == get_rows(s) - 1) {
    return;
  }

  u32 maxx = getmaxx(w) - 1;
  u32 maxy = getmaxy(w) - 1;

  if (s->cy == maxy) {
    text_down(s, w);
  } else {
    s->cy++;
  }

  s->buffer->current = s->buffer->current->next;
  s->row++;

  if (s->col > s->buffer->current->buffer_len) {
    s->col = s->buffer->current->buffer_len;

    if (s->col <= s->beg_col) {
      s->beg_col = s->col - 1 % maxx;
    }

    s->cx = s->col - s->beg_col;
  }

  wmove(w, s->cy, s->cx);
}

void cursor_right(Section *s, WINDOW *w) {
  u32 maxx = getmaxx(w) - 1;
  u32 maxy = getmaxy(w) - 1;

  if (s->col + 1 > s->buffer->current->buffer_len) {
    if (s->row == get_rows(s) - 1) {
      return;
    }

    s->row++;
    s->cx = 0;
    s->col = 0;
    s->beg_col = 0;

    if (s->cy == maxy) {
      text_down(s, w);
    } else {
      s->cy++;
    }

    s->buffer->current = s->buffer->current->next;
  } else {
    if (s->cx == maxx) {
      s->beg_col++;
    } else {
      s->cx++;
    }
    s->col++;
  }

  wmove(w, s->cy, s->cx);
}

void cursor_left(Section *s, WINDOW *w) {
  u32 maxx = getmaxx(w) - 1;

  if (s->col == 0) {
    if (s->row == 0) {
      return;
    }

    s->buffer->current = s->buffer->current->prev;

    s->row--;
    s->col = s->buffer->current->buffer_len;

    if (s->col > maxx) {
      s->beg_col = s->col - maxx;
    }

    s->cx = s->col - s->beg_col;

    if (s->cy == 0) {
      text_up(s, w);
    } else {
      s->cy--;
    }
  } else {
    if (s->cx == 0) {
      s->beg_col--;
    } else {
      s->cx--;
    }
    s->col--;
  }

  wmove(w, s->cy, s->cx);
}

void cursor_home(Section *s, WINDOW *w) {
  s->beg_col = 0;
  s->cx = 0;
  s->col = 0;
}

void cursor_end(Section *s, WINDOW *w) {
  u32 maxx = getmaxx(w) - 1;

  s->col = s->buffer->current->buffer_len;

  if (s->col > maxx && s->col - s->beg_col > maxx) {
    s->beg_col = s->col - maxx;
  }

  s->cx = s->col - s->beg_col;
}
