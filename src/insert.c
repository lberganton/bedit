/*
 * File: insert.c
 * Author: Lucas Berganton
 * Created: 05/20/2024
 */
#include "buffer.h"
#include "section.h"
#include "ui.h"
#include <ctype.h>
#include <string.h>

void insert_char(Section *s, char ch) {
  if (!buffer_insert_char(get_utfchar(&ch), s->col, s->buffer->current)) {
    paint_command_bar("Erro: Falha ao inserir, buffer de linha cheio",
                      COLOR_PAIR(PAIR_TEXT), s);
    return;
  }

  cursor_right(s);
}

void delete_char(Section *s) {
  if (s->col == s->buffer->current->buffer_len) {
    delete_line(s);
  } else {
    buffer_delete_char(s->col, s->buffer->current);
  }
}

void backspace_char(Section *s) {
  if (s->row == 0 && s->col == 0) {
    return;
  }

  if (s->col == 0) {
    backspace_line(s);
  } else {
    buffer_delete_char(s->col - 1, s->buffer->current);
  }


  cursor_left(s);
}

void insert_new_line(Section *s) {
  BufferNode *current = s->buffer->current;
  BufferNode *new = buffer_insert_next(s->buffer, current);

  memcpy(&new->buffer[0], &current->buffer[s->col],
         (current->buffer_len - s->col) * sizeof(UTFChar));

  new->buffer_len = (current->buffer_len - s->col);
  current->buffer_len = s->col;

  cursor_down(s);
  cursor_home(s);
}

void delete_line(Section *s) {
  if (s->row == s->buffer->nodes - 1) {
    return;
  }

  BufferNode *current = s->buffer->current;
  BufferNode *next = current->next;

  if (current->buffer_len + next->buffer_len >= BUFF_COL) {
    section_set_msg(s, "Erro: Apagar a linha estouraria o buffer");
    return;
  }

  memcpy(&current->buffer[current->buffer_len], &next->buffer[0],
         next->buffer_len * sizeof(UTFChar));
  current->buffer_len += next->buffer_len;

  buffer_remove_node(s->buffer, next);
}

void backspace_line(Section *s) {
  if (s->row == 0) {
    return;
  }

  BufferNode *prev = s->buffer->current->prev;
  BufferNode *current = prev->next;

  if (current->buffer_len + prev->buffer_len >= BUFF_COL) {
    section_set_msg(s, "Erro: Apagar a linha estouraria o buffer");
    return;
  }

  memcpy(&prev->buffer[prev->buffer_len], &current->buffer[0],
         current->buffer_len * sizeof(UTFChar));
  prev->buffer_len += current->buffer_len;

  buffer_remove_node(s->buffer, current);
}
