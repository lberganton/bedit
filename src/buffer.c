/*
 * File: buffer.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "buffer.h"
#include "defs.h"
#include <stdlib.h>

static BufferNode *node_create(BufferNode *prev, BufferNode *next) {
  BufferNode *new = (BufferNode *)malloc(sizeof(BufferNode));
  ABORT(new == NULL, "Erro: Falha ao alocar memória para buffer.");

  new->prev = prev;
  new->next = next;
  new->buffer_len = 0;
  new->buffer->size = 0;

  return new;
}

void buffer_insert_begin(Buffer *b) {
  BufferNode *new = node_create(NULL, b->begin);

  if (b->begin == NULL) {
    b->end = new;
  } else {
    b->begin->prev = new;
  }

  b->begin = new;
}

void buffer_insert_end(Buffer *b) {
  BufferNode *new = node_create(b->end, NULL);

  if (b->begin == NULL) {
    b->begin = new;
  } else {
    b->end->next = new;
  }

  b->end = new;
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

void buffer_read_file(Buffer *b, FILE *f) {
  if (fgetc(f) == EOF) {
    return;
  }

  fseek(f, 0, SEEK_SET);

  BufferNode *aux = b->begin;
  char buffer[BUFF_SIZE];
  u32 pos = 0;
  int ch;

  while ((ch = fgetc(f)) != EOF) {
    ABORT(pos >= BUFF_SIZE, "Erro: Estouro no buffer de linha.");

    if (ch != '\n') {
      buffer[pos++] = ch;
      continue;
    }

    u32 i = 0;
    while (i < pos) {
      aux->buffer[aux->buffer_len++] = get_utfchar(&buffer[i]);
      i += get_encoding(&buffer[i]);
    }

    buffer_insert_end(b);
    aux = b->end;
    b->nodes++;
    pos = 0;
  }
}
