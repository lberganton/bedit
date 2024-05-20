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

  new->prev = prev;
  new->next = next;
  new->buffer_len = 0;
  new->buffer->size = 0;

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

BufferNode *buffer_insert_begin(Buffer *b) {
  BufferNode *new = node_create(NULL, b->begin);

  b->nodes++;

  if (b->begin == NULL) {
    b->end = new;
  } else {
    b->begin->prev = new;
  }

  b->begin = new;
  return new;
}

BufferNode *buffer_insert_end(Buffer *b) {
  BufferNode *new = node_create(b->end, NULL);

  b->nodes++;

  if (b->begin == NULL) {
    b->begin = new;
  } else {
    b->end->next = new;
  }

  b->end = new;
  return new;
}

BufferNode *buffer_insert_next(Buffer *b, BufferNode *n) {
  BufferNode *new = node_create(n, n->next);

  b->nodes++;

  if (b->end == n) {
    b->end = n;
  }

  if (new->next) {
    new->next->prev = new;
  }

  n->next = new;
  return new;
}

bool buffer_insert_char(UTFChar ch, u32 index, BufferNode *n) {
  if (n->buffer_len == BUFF_SIZE) {
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
