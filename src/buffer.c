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
  new->current = new->begin;
  new->nodes = 1;

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

u8 get_char_encoding(char *ch) {
  ABORT(ch == NULL, "Erro: Falha em descobrir o encoding de um caractere.");

  unsigned char *bytes = (unsigned char *)ch;

  if (bytes[0] == 0x09 || bytes[0] == 0x0A || bytes[0] == 0x0D ||
      (0x20 <= bytes[0] && bytes[0] <= 0x7E)) {
    return 1;
  }

  if ((0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
      (0x80 <= bytes[1] && bytes[1] <= 0xBF)) {
    return 2;
  }

  if ((bytes[0] == 0xE0 && (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF)) ||
      (((0xE1 <= bytes[0] && bytes[0] <= 0xEC) || bytes[0] == 0xEE ||
        bytes[0] == 0xEF) &&
       (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF)) ||
      (bytes[0] == 0xED && (0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF))) {
    return 3;
  }

  if ((bytes[0] == 0xF0 && (0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
       (0x80 <= bytes[3] && bytes[3] <= 0xBF)) ||
      ((0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
       (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
       (0x80 <= bytes[3] && bytes[3] <= 0xBF)) ||
      (bytes[0] == 0xF4 && (0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
       (0x80 <= bytes[3] && bytes[3] <= 0xBF))) {
    return 4;
  }

  return 1;
}
