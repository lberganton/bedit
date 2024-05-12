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

  return new;
}

static void insert_begin(Buffer *b) {
  BufferNode *new = node_create(NULL, b->begin);

  if (b->begin == NULL) {
    b->end = new;
  } else {
    b->begin->prev = new;
  }

  b->begin = new;
}

static void insert_end(Buffer *b) {
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
  new->nodes = 0;

  return new;
}

void buffer_read_file(Buffer *b, FILE *f) {
  if (fgetc(f) == EOF) {
    return;
  }

  fseek(f, 0, SEEK_SET);

  BufferNode *aux = b->begin;
  int ch;

  while ((ch = fgetc(f)) != EOF) {
    if (ch == '\n') {
      insert_end(b);
      aux = b->end;
      b->nodes++;
      continue;
    }

    aux->buffer[aux->buffer_len++] = ch;

    ABORT(aux->buffer_len >= BUFF_SIZE, "Erro: Estouro no buffer de linha.");
  }
}

void buffer_print(Buffer *b) {
  BufferNode *aux = b->begin;

  while (aux) {
    for (size_t i = 0; i < aux->buffer_len; i++) {
      addch(aux->buffer[i]);
    }
    aux = aux->next;
  }
}

bool is_utf(char ch) { return ch == -61; }
