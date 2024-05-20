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

bool insert_at(Section *s, UTFChar ch, u32 index) {
  if (!buffer_insert_at(ch, index, s->buffer->current)) {
    paint_command_bar("Erro: Falha ao inserir, buffer de linha cheio",
                      COLOR_PAIR(PAIR_TEXT), s);
    return false;
  }

  cursor_right(s);

  return true;
}
