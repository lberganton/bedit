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

static bool is_pair(char ch) {
  return (strchr(single_pairs, ch)) || (strchr(double_pairs, ch));
}

void insert_tab(Section *s) {
  for (u8 i = s->col % TAB_SIZE; i < TAB_SIZE; i++) {
    insert_char(s, ' ');
  }
}

void insert_char(Section *s, char ch) {
  if (s->col + 2 >= BUFF_COL) {
    section_set_msg(s, "Falha ao inserir, buffer de linha cheio");
    return;
  }

  if (AUTO_PAIRS && is_pair(ch)) {
    insert_pairs(s, ch);
    cursor_right(s);
    return;
  }

  if (!s->undo->dirty) {
    s->undo->dirty = true;
    push_undo(s, UNDO_ROW);
  }

  buffer_insert_char(get_utfchar(&ch), s->col, s->buffer->current);
  cursor_right(s);
}

void delete_char(Section *s) {
  if (s->col < s->buffer->current->buffer_len) {
    if (!s->undo->dirty) {
      s->undo->dirty = true;
      push_undo(s, UNDO_ROW);
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
      push_undo(s, UNDO_ROW);
    }

    char ch = utfchar_to_int(s->buffer->current->buffer[s->col - 1]);

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

  u32 len = prev->buffer_len;

  if (!merge_line(s, prev, s->buffer->current)) {
    return;
  }

  cursor_up(s);
  for (u32 i = 0; i < len; i++) {
    cursor_right(s);
  }

  buffer_valid_next(s->buffer->current)->activated = false;
  s->rows--;
}

void insert_pairs(Section *s, char ch) {
  UTFChar first_pair, last_pair;
  char *ptr;

  if ((ptr = strchr(single_pairs, ch))) {
    first_pair = get_utfchar(&ch);
    last_pair = first_pair;
    if (compare_utfchar(s->buffer->current->buffer[s->col], first_pair)) {
      return;
    }
  } else if ((ptr = strchr(double_pairs, ch))) {
    ptrdiff_t pos = ptr - double_pairs;
    if (pos & 1) {
      return;
    }
    first_pair = get_utfchar(&double_pairs[pos]);
    last_pair = get_utfchar(&double_pairs[pos + 1]);
  }

  buffer_insert_char(first_pair, s->col, s->buffer->current);
  buffer_insert_char(last_pair, s->col + 1, s->buffer->current);
}

void insert_new_line(Section *s) {
  BufferNode *current = s->buffer->current;
  BufferNode *new = buffer_insert_next(s->buffer, current);

  memcpy(&new->buffer[0], &current->buffer[s->col],
         (current->buffer_len - s->col) * sizeof(UTFChar));

  new->buffer_len = (current->buffer_len - s->col);
  current->buffer_len = s->col;

  s->rows++;

  cursor_down(s);
  cursor_home(s);

  for (u32 i = 0; i < current->buffer_len && current->buffer[i].size == 1 &&
                  current->buffer[i].data[0] == ' ';
       i++) {
    insert_char(s, ' ');
  }
}

bool merge_line(Section *s, BufferNode *dest, BufferNode *src) {
  if (dest == NULL || src == NULL) {
    section_set_msg(s, "Falha ao mesclar as linhas.");
    return false;
  }

  if (dest->buffer_len + src->buffer_len >= BUFF_COL) {
    section_set_msg(s, "Mesclar as linhas estouraria o buffer");
    return false;
  }

  memcpy(&dest->buffer[dest->buffer_len], &src->buffer[0],
         src->buffer_len * sizeof(UTFChar));

  dest->buffer_len += src->buffer_len;

  return true;
}
