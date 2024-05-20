/*
 * File: insert.c
 * Author: Lucas Berganton
 * Created: 05/20/2024
 */
#include "section.h"
#include "buffer.h"
#include "ui.h"
#include <string.h>
#include <ctype.h>

bool insert_at(Section *s, UTFChar ch, u32 index) {
  if (!buffer_insert_at(ch, s->col, s->buffer->current)) {
    return false;
  }
  return true;
}
