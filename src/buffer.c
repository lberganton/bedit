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

void buffer_read_file(const char *file_name, Buffer *b) {
  // Open the file.
  FILE *f = fopen(file_name, "r");
  ABORT(f == NULL, "Erro: Falha ao abrir o arquivo.");
  
  // Return the function if the file is empty.
  if (fgetc(f) == EOF) {
    fclose(f);
    return;
  }

  // Moves the file cursor to start.
  fseek(f, 0, SEEK_SET);

  BufferNode *aux = b->begin;
  char buffer[BUFF_SIZE];
  u32 pos = 0;
  int ch;

  // Loops until the file reaches the end.
  while ((ch = fgetc(f)) != EOF) {
    ABORT(pos >= BUFF_SIZE, "Erro: Estouro no buffer de linha.");

    // If the character read isn't a new line, put it in the buffer and go to
    // the next iteration.
    if (ch != '\n') {
      buffer[pos++] = ch;

      // If the characters is a tab, fill the next indexes with spaces.
      if (ch == '\t') {
        for (u32 i = 1; i < TAB_SIZE; i++) {
          ABORT(pos >= BUFF_SIZE, "Erro: Estouro no buffer de linha.");
          buffer[pos++] = ' ';
        }
      }

      continue;
    }

    u32 i = 0;

    // Iterates over the buffer converting the default char type in UTFChar to
    // put it in the node (line) buffer.
    while (i < pos) {
      aux->buffer[aux->buffer_len++] = get_utfchar(&buffer[i]);
      i += get_utf_len(&buffer[i]);
    }

    // Inserts a new node at the end of the list.
    buffer_insert_end(b);
    aux = b->end;

    // Increases the number of nodes (lines).
    b->nodes++;

    pos = 0;
  }

  fclose(f);
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

bool buffer_insert_at(UTFChar ch, u32 index, BufferNode *n) {
  if (n->buffer_len == BUFF_SIZE) {
    return false;
  }

  memcpy(&n->buffer[index + 1], &n->buffer[index],
         (n->buffer_len - index) * sizeof(UTFChar));
  
  n->buffer[index] = ch;
  n->buffer_len++;

  return true;
}
