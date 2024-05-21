/*
 * File: cursor.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "section.h"
#include "ui.h"
#include <ncurses.h>

void cursor_up(Section *s) {
  // Returns if the current row is the first. Obviously, if it is, it cannot
  // move up.
  if (s->row == 0) {
    return;
  }

  u32 maxx = getmaxx(s->window->text) - 1;
  u32 begy = getbegy(s->window->text);

  // If the cursor is at the top of the screen, move the text up. In the other
  // hand, just decreases your height.
  if (s->cy == begy) {
    text_up(s);
  } else {
    s->cy--;
  }

  // Set the current row as the previous row.
  s->buffer->current = s->buffer->current->prev;
  s->row--;

  if (s->col > s->buffer->current->buffer_len) {
    // If the row we're going to is shorter than the one we were on, set the
    // column to the end of the row.
    s->col = s->buffer->current->buffer_len;

    if (s->col <= s->beg_col) {
      // If the end of the row is behind of the current begining column, set the
      // new begining column based on the text screen width.
      s->beg_col = s->col % maxx;
    }

    s->cx = s->col - s->beg_col;
  }

  wmove(s->window->text, s->cy, s->cx);
}

void cursor_down(Section *s) {
  // Returns if the current row is the last. Obviously, if it is, it cannot
  // move down.
  if (s->row == get_rows(s) - 1) {
    return;
  }

  u32 maxx = getmaxx(s->window->text) - 1;
  u32 maxy = getmaxy(s->window->text) - 1;

  // If the cursor is at the bottom of the screen, move the text down. In the
  // other hand, just increase your height.
  if (s->cy == maxy) {
    text_down(s);
  } else {
    s->cy++;
  }

  s->buffer->current = s->buffer->current->next;
  s->row++;

  if (s->col > s->buffer->current->buffer_len) {
    // If the row we're going to is shorter than the one we were on, set the
    // column to the end of the row.
    s->col = s->buffer->current->buffer_len;

    if (s->col <= s->beg_col) {
      // If the end of the row is behind of the current begining column, set the
      // new begining column based on the text screen width.
      s->beg_col = s->col % maxx;
    }

    s->cx = s->col - s->beg_col;
  }

  wmove(s->window->text, s->cy, s->cx);
}

void cursor_right(Section *s) {
  u32 maxx = getmaxx(s->window->text) - 1;
  u32 maxy = getmaxy(s->window->text) - 1;

  if (s->col + 1 > s->buffer->current->buffer_len) {
    if (s->row == get_rows(s) - 1) {
      return;
    }

    s->row++;
    s->cx = 0;
    s->col = 0;
    s->beg_col = 0;

    if (s->cy == maxy) {
      text_down(s);
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

  wmove(s->window->text, s->cy, s->cx);
}

void cursor_left(Section *s) {
  u32 maxx = getmaxx(s->window->text) - 1;

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
      text_up(s);
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

  wmove(s->window->text, s->cy, s->cx);
}

void cursor_home(Section *s) {
  s->beg_col = 0;
  s->cx = 0;
  s->col = 0;
}

void cursor_end(Section *s) {
  u32 maxx = getmaxx(s->window->text) - 1;

  s->col = s->buffer->current->buffer_len;

  if (s->col > maxx && s->col - s->beg_col > maxx) {
    s->beg_col = s->col - maxx;
  }

  s->cx = s->col - s->beg_col;
}

void cursor_pgup(Section *s) {
  u32 maxy = getmaxy(s->window->text);

  for (u32 i = 0; i < maxy && s->beg_row > 0; i++) {
    s->beg_row--;
    s->buffer->top = s->buffer->top->prev;
  }

  s->col = 0;
  s->row = s->beg_row;
  s->buffer->current = s->buffer->top;

  s->cy = 0;
  s->cx = 0;
  s->beg_col = 0;
}

void cursor_pgdown(Section *s) {
  u32 maxy = getmaxy(s->window->text);

  for (u32 i = 0; i < maxy && s->beg_row < get_rows(s) - 1; i++) {
    s->beg_row++;
    s->buffer->top = s->buffer->top->next;
  }

  s->col = 0;
  s->row = s->beg_row;
  s->buffer->current = s->buffer->top;

  s->cy = 0;
  s->cx = 0;
  s->beg_col = 0;
}
