/*
 * File: insert.c
 * Author: Lucas Berganton
 * Created: 05/20/2024
 */
#include "input.h"
#include "ui.h"
#include <ctype.h>
#include <stddef.h>
#include <string.h>

static const char single_pairs[] = "\'\"";
static const char double_pairs[] = "()<>[]{}";

static bool is_pair(wchar_t ch) {
  return (strchr(single_pairs, ch)) || (strchr(double_pairs, ch));
}

void insert_tab(Section *s) {
  for (u8 i = s->col % TAB_SIZE; i < TAB_SIZE; i++) {
    insert_char(s, ' ');
  }
}

void insert_char(Section *s, wchar_t ch) {
  if (AUTO_PAIRS && is_pair(ch)) {
    if (s->col + 1 >= BUFF_COL) {
      section_set_msg(s, "Falha ao inserir, buffer de linha cheio");
      return;
    }
    insert_pairs(s, ch);
    cursor_right(s);
    return;
  }

  if (s->col >= BUFF_COL) {
    section_set_msg(s, "Falha ao inserir, buffer de linha cheio");
    return;
  }

  if (!s->undo->dirty) {
    s->undo->dirty = true;
    push_undo(s, UNDO_ROW, s->buffer->current, NULL);
  }

  buffer_insert_char(ch, s->col, s->buffer->current);
  cursor_right(s);
}

void delete_char(Section *s) {
  if (s->col < s->buffer->current->string_length) {
    if (!s->undo->dirty) {
      s->undo->dirty = true;
      push_undo(s, UNDO_ROW, s->buffer->current, NULL);
    }
    buffer_delete_char(s->col, s->buffer->current);
    return;
  }

  if (s->row == s->rows - 1) {
    return;
  }

  BufferNode *next = buffer_valid_next(s->buffer->current);

  if (!merge_line(s, s->buffer->current, next)) {
    return;
  }

  next->activated = false;
  s->rows--;
}

void backspace_char(Section *s) {
  if (s->col > 0) {
    if (!s->undo->dirty) {
      s->undo->dirty = true;
      push_undo(s, UNDO_ROW, s->buffer->current, NULL);
    }

    wchar_t ch = s->buffer->current->vector[s->col - 1];

    buffer_delete_char(s->col - 1, s->buffer->current);
    cursor_left(s);

    if (AUTO_PAIRS && ch) {
      char *ptr;

      if ((ptr = strrchr(single_pairs, ch))) {
        ptrdiff_t pos = ptr - single_pairs;
        if (ch == single_pairs[pos]) {
          buffer_delete_char(s->col, s->buffer->current);
        }
      } else if ((ptr = strrchr(double_pairs, ch))) {
        ptrdiff_t pos = ptr - double_pairs;
        if (!(pos & 1) && (ch == double_pairs[pos])) {
          buffer_delete_char(s->col, s->buffer->current);
        }
      }
    }

    return;
  }

  if (s->row == 0) {
    return;
  }

  BufferNode *prev = buffer_valid_prev(s->buffer->current);
  BufferNode *current = s->buffer->current;

  push_undo(s, UNDO_REMOVE_ROW, current, prev);

  u32 len = prev->string_length;

  if (!merge_line(s, prev, current)) {
    return;
  }

  cursor_up(s);
  for (u32 i = 0; i < len; i++) {
    cursor_right(s);
  }

  current->activated = false;
  s->rows--;
}

void insert_pairs(Section *s, wchar_t ch) {
  wchar_t first_pair, last_pair;
  char *ptr;

  if ((ptr = strchr(single_pairs, ch))) {
    first_pair = (char)ch;
    last_pair = first_pair;
    if (s->buffer->current->vector[s->col] == first_pair) {
      return;
    }
  } else if ((ptr = strchr(double_pairs, ch))) {
    ptrdiff_t pos = ptr - double_pairs;
    if (pos & 1) {
      return;
    }
    first_pair = double_pairs[pos];
    last_pair = double_pairs[pos + 1];
  }

  buffer_insert_char(first_pair, s->col, s->buffer->current);
  buffer_insert_char(last_pair, s->col + 1, s->buffer->current);
}

void insert_new_line(Section *s) {
  BufferNode *current = s->buffer->current;
  BufferNode *new = buffer_insert_next(s->buffer, current);

  push_undo(s, UNDO_NEW_ROW, new, current);

  buffer_increase_vector(new, current->string_length - s->col);

  memcpy(&new->vector[0], &current->vector[s->col],
         (current->string_length - s->col) * sizeof(wchar_t));

  if (current->vector_length && s->col <= current->vector_length - 50) {
    buffer_decrease_vector(current, current->string_length - s->col);
  }

  new->string_length = current->string_length - s->col;
  current->string_length = s->col;

  s->rows++;

  cursor_down(s);
  cursor_home(s);

  for (u32 i = 0; i < current->string_length && current->vector[i] == ' ';
       i++) {
    insert_char(s, ' ');
  }
}

bool merge_line(Section *s, BufferNode *dest, BufferNode *src) {
  if (dest == NULL || src == NULL) {
    section_set_msg(s, "Falha ao mesclar as linhas.");
    return false;
  }

  if (dest->string_length + src->string_length >= BUFF_COL) {
    section_set_msg(s, "Mesclar as linhas estouraria o buffer");
    return false;
  }

  if (dest->string_length + src->string_length >= dest->vector_length) {
    buffer_increase_vector(dest, src->string_length);
  }

  memcpy(&dest->vector[dest->string_length], &src->vector[0],
         src->string_length * sizeof(wchar_t));

  dest->string_length += src->string_length;

  return true;
}
