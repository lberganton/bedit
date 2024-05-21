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
  if (s->col < s->buffer->current->buffer_len) {
    buffer_delete_char(s->col, s->buffer->current);
    return;
  }

  if (s->row == s->buffer->nodes - 1) {
    return;
  }

  if (!merge_line(s, s->buffer->current, s->buffer->current->next)) {
    return;
  }

  buffer_remove_node(s->buffer, s->buffer->current->next);
}

void backspace_char(Section *s) {
  if (s->col > 0) {
    buffer_delete_char(s->col - 1, s->buffer->current);
    cursor_left(s);
    return;
  }

  u32 len = s->buffer->current->prev->buffer_len;

  if (!merge_line(s, s->buffer->current->prev, s->buffer->current)) {
    return;
  }

  cursor_up(s);
  for (u32 i = 0; i < len; i++) {
    cursor_right(s);
  }

  buffer_remove_node(s->buffer, s->buffer->current->next);
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

  for (u32 i = 0; i < current->buffer_len &&
                  current->buffer[i].size == 1 &&
                  current->buffer[i].data[0] == ' ';
                  i++) {
    insert_char(s, ' ');
  }
}

bool merge_line(Section *s, BufferNode *dest, BufferNode *src) {
  if (dest == NULL || src == NULL) {
    section_set_msg(s, "Erro: Falha ao mesclar as linhas.");
    return false;
  }

  if (dest->buffer_len + src->buffer_len >= BUFF_COL) {
    section_set_msg(s, "Erro: Mesclar as linhas estouraria o buffer");
    return false;
  }

  memcpy(&dest->buffer[dest->buffer_len], &src->buffer[0],
         src->buffer_len * sizeof(UTFChar));

  dest->buffer_len += src->buffer_len;

  return true;
}
