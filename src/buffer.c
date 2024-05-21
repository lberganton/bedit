/*
 * File: buffer.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "buffer.h"
#include "defs.h"
#include <stdlib.h>
#include <string.h>

static BufferNode *node_create(BufferNode *prev, BufferNode *next) {
  BufferNode *new = (BufferNode *)malloc(sizeof(BufferNode));
  ABORT(new == NULL, "Erro: Falha ao alocar memória para buffer.");

  new->activated = true;
  new->prev = prev;
  new->next = next;
  new->buffer_len = 0;
  new->buffer->size = 0;

  if (prev) {
    prev->next = new;
  }
  if (next) {
    next->prev = new;
  }

  return new;
}

Buffer *buffer_init(void) {
  Buffer *new = (Buffer *)malloc(sizeof(Buffer));
  ABORT(new == NULL, "Erro: Falha ao alocar memória para buffer.");

  new->begin = node_create(NULL, NULL);
  new->end = new->begin;
  new->current = new->begin;
  new->top = new->begin;
  new->nodes = 1;

  return new;
}

BufferNode *buffer_insert_next(Buffer *b, BufferNode *n) {
  BufferNode *new = node_create(n, n->next);

  b->nodes++;

  if (b->end == n) {
    b->end = new;
  }

  return new;
}

bool buffer_insert_char(UTFChar ch, u32 index, BufferNode *n) {
  if (n->buffer_len == BUFF_COL) {
    return false;
  }

  memcpy(&n->buffer[index + 1], &n->buffer[index],
         (n->buffer_len - index) * sizeof(UTFChar));

  n->buffer[index] = ch;
  n->buffer_len++;

  return true;
}

bool buffer_delete_char(u32 index, BufferNode *n) {
  if (index == n->buffer_len) {
    return false;
  }

  memcpy(&n->buffer[index], &n->buffer[index + 1],
         (n->buffer_len - index) * sizeof(UTFChar));

  n->buffer_len--;

  return true;
}
