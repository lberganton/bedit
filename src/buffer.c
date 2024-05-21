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
  ABORT(n == NULL, "Erro: Tentativa de encadeamento com um buffer vazio.");
  BufferNode *new = node_create(n, n->next);

  b->nodes++;

  if (b->end == n) {
    b->end = new;
  }

  return new;
}

void buffer_remove_node(Buffer *b, BufferNode *n) {
  ABORT(n == NULL, "Erro: Tentativa de remover de um buffer vazio.");

  if (b->begin == n) {
    b->begin = b->begin->next;
  }
  if (b->end == n) {
    b->end = b->end->prev;
  }

  if (n->prev) {
    n->prev->next = n->next;
  }
  if (n->next) {
    n->next->prev = n->prev;
  }

  b->nodes--;
  free(n);
}

bool buffer_insert_char(UTFChar ch, u32 index, BufferNode *n) {
  // Make sure the column buffer is at maximum and can insert a new character.
  if (n->buffer_len == BUFF_COL) {
    return false;
  }

  // Move all characters from the index forward.
  memcpy(&n->buffer[index + 1], &n->buffer[index],
         (n->buffer_len - index) * sizeof(UTFChar));

  // Insert the new character.
  n->buffer[index] = ch;
  n->buffer_len++;

  return true;
}

bool buffer_delete_char(u32 index, BufferNode *n) {
  // Make sure the index isn't out of bounds and there are characters to remove.
  if (index >= n->buffer_len || n->buffer_len == 0) {
    return false;
  }

  // Move all characters in front to the position of the character to be
  // deleted.
  memcpy(&n->buffer[index], &n->buffer[index + 1],
         (n->buffer_len - index) * sizeof(UTFChar));

  n->buffer_len--;

  return true;
}
