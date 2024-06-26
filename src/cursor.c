/*
 * File: cursor.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "section.h"
#include "ui.h"
#include <ctype.h>
#include <ncurses.h>

void cursor_up(Section *s) {
  // Returns if the current row is the first. Obviously, if it is, it cannot
  // move up.
  if (s->row == 0) {
    return;
  }

  u32 maxx = getmaxx(s->window_text) - 1;
  u32 begy = getbegy(s->window_text);

  // If the cursor is at the top of the screen, move the text up. In the other
  // hand, just decreases your height.
  if (s->cursor_y == begy) {
    text_up(s);
  } else {
    s->cursor_y--;
  }

  // Set the current row as the previous row.
  s->buffer->current = buffer_valid_prev(s->buffer->current);
  s->row--;

  if (s->col > s->buffer->current->string_length) {
    // If the row we're going to is shorter than the one we were on, set the
    // column to the end of the row.
    s->col = s->buffer->current->string_length;

    if (s->col <= s->beg_col) {
      // If the end of the row is behind of the current begining column, set the
      // new begining column based on the text screen width.
      s->beg_col = s->col % maxx;
    }

    s->cursor_x = s->col - s->beg_col;
  }

  s->undo->dirty = false;
  wmove(s->window_text, s->cursor_y, s->cursor_x);
}

void cursor_down(Section *s) {
  // Returns if the current row is the last. Obviously, if it is, it cannot
  // move down.
  if (s->row == s->rows - 1) {
    return;
  }

  u32 maxx = getmaxx(s->window_text) - 1;
  u32 maxy = getmaxy(s->window_text) - 1;

  // If the cursor is at the bottom of the screen, move the text down. In the
  // other hand, just increase your height.
  if (s->cursor_y == maxy) {
    text_down(s);
  } else {
    s->cursor_y++;
  }

  s->buffer->current = buffer_valid_next(s->buffer->current);
  s->row++;

  if (s->col > s->buffer->current->string_length) {
    // If the row we're going to is shorter than the one we were on, set the
    // column to the end of the row.
    s->col = s->buffer->current->string_length;

    if (s->col <= s->beg_col) {
      // If the end of the row is behind of the current begining column, set the
      // new begining column based on the text screen width.
      s->beg_col = s->col % maxx;
    }

    s->cursor_x = s->col - s->beg_col;
  }

  s->undo->dirty = false;
  wmove(s->window_text, s->cursor_y, s->cursor_x);
}

void cursor_right(Section *s) {
  u32 maxx = getmaxx(s->window_text) - 1;
  u32 maxy = getmaxy(s->window_text) - 1;

  if (s->col + 1 > s->buffer->current->string_length) {
    if (s->row == s->rows - 1) {
      return;
    }

    s->undo->dirty = false;

    s->row++;
    s->cursor_x = 0;
    s->col = 0;
    s->beg_col = 0;

    if (s->cursor_y == maxy) {
      text_down(s);
    } else {
      s->cursor_y++;
    }

    s->buffer->current = buffer_valid_next(s->buffer->current);
  } else {
    if (s->cursor_x == maxx) {
      s->beg_col++;
    } else {
      s->cursor_x++;
    }
    s->col++;
  }

  wmove(s->window_text, s->cursor_y, s->cursor_x);
}

void cursor_left(Section *s) {
  u32 maxx = getmaxx(s->window_text) - 1;

  if (s->col == 0) {
    if (s->row == 0) {
      return;
    }

    s->undo->dirty = false;
    s->buffer->current = buffer_valid_prev(s->buffer->current);

    s->row--;
    s->col = s->buffer->current->string_length;

    if (s->col > maxx) {
      s->beg_col = s->col - maxx;
    }

    s->cursor_x = s->col - s->beg_col;

    if (s->cursor_y == 0) {
      text_up(s);
    } else {
      s->cursor_y--;
    }
  } else {
    if (s->cursor_x == 0) {
      s->beg_col--;
    } else {
      s->cursor_x--;
    }
    s->col--;
  }

  wmove(s->window_text, s->cursor_y, s->cursor_x);
}

void cursor_home(Section *s) {
  s->beg_col = 0;
  s->cursor_x = 0;
  s->col = 0;

  s->undo->dirty = false;
}

void cursor_end(Section *s) {
  u32 maxx = getmaxx(s->window_text) - 1;

  s->col = s->buffer->current->string_length;

  if (s->col > maxx && s->col - s->beg_col > maxx) {
    s->beg_col = s->col - maxx;
  }

  s->cursor_x = s->col - s->beg_col;

  s->undo->dirty = false;
}

void cursor_pgup(Section *s) {
  u32 maxy = getmaxy(s->window_text);

  for (u32 i = 0; i < maxy && s->beg_row > 0; i++) {
    s->beg_row--;
    s->buffer->top = buffer_valid_prev(s->buffer->top);
  }

  s->col = 0;
  s->row = s->beg_row;
  s->buffer->current = s->buffer->top;

  s->cursor_y = 0;
  s->cursor_x = 0;
  s->beg_col = 0;

  s->undo->dirty = false;
}

void cursor_pgdown(Section *s) {
  u32 maxy = getmaxy(s->window_text);

  for (u32 i = 0; i < maxy && s->beg_row < s->rows - 1; i++) {
    s->beg_row++;
    s->buffer->top = buffer_valid_next(s->buffer->top);
  }

  s->col = 0;
  s->row = s->beg_row;
  s->buffer->current = s->buffer->top;

  s->cursor_y = 0;
  s->cursor_x = 0;
  s->beg_col = 0;

  s->undo->dirty = false;
}

void cursor_nextword(Section *s) {
  if (s->row == s->rows - 1 && s->col + 1 > s->buffer->current->string_length) {
    return;
  }

  while (s->buffer->current->vector_length == 0 || !isblank(s->buffer->current->vector[s->col])) {
    if (s->col >= s->buffer->current->string_length) {
      cursor_right(s);
      return;
    }
    cursor_right(s);
  }

  while (isblank(s->buffer->current->vector[s->col])) {
    if (s->col >= s->buffer->current->string_length) {
      cursor_right(s);
      return;
    }
    cursor_right(s);
  }
}

void cursor_prevword(Section *s) {
  if (s->row == 0 && s->col == 0) {
    return;
  }

  while (s->buffer->current->vector_length == 0 || !isblank(s->buffer->current->vector[s->col])) {
    if (s->col == 0) {
      cursor_left(s);
      return;
    }
    cursor_left(s);
  }

  while (isblank(s->buffer->current->vector[s->col])) {
    if (s->col == 0) {
      cursor_left(s);
      return;
    }
    cursor_left(s);
  }
}
