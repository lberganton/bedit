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
  ASSERT(new == NULL, "Erro: Falha ao alocar memória para buffer.");

  new->activated = true;
  new->prev = prev;
  new->next = next;
  new->vector = NULL;
  new->vector_length = 0;
  new->string_length = 0;

  if (prev) {
    prev->next = new;
  }
  if (next) {
    next->prev = new;
  }

  return new;
}

static void buffer_increase_vector(BufferNode *node) {
  ASSERT(node->vector_length >= BUFF_COL, "Erro: O vetor já está no limite.");

  node->vector_length +=
      node->string_length + 50 < BUFF_COL ? 50 : BUFF_COL - node->string_length;
  
  wchar_t *new_mem = realloc(node->vector, node->vector_length * sizeof(wchar_t));

  ASSERT(new_mem == NULL,
         "Erro: Falha ao relocar vetor.");
  
  node->vector = new_mem;
}

Buffer *buffer_init(void) {
  Buffer *new = (Buffer *)malloc(sizeof(Buffer));
  ASSERT(new == NULL, "Erro: Falha ao alocar memória para buffer.");

  new->begin = node_create(NULL, NULL);
  new->end = new->begin;
  new->current = new->begin;
  new->top = new->begin;
  new->nodes = 1;

  return new;
}

BufferNode *buffer_insert_next(Buffer *b, BufferNode *n) {
  ASSERT(n == NULL, "Erro: Tentativa de encadeamento com um buffer vazio.");
  BufferNode *new = node_create(n, n->next);

  b->nodes++;

  if (b->end == n) {
    b->end = new;
  }

  return new;
}

void buffer_remove_node(Buffer *b, BufferNode *n) {
  ASSERT(n == NULL, "Erro: Tentativa de remover de um buffer vazio.");

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

  if (n->vector) {
    free(n->vector);
  }

  free(n);
}

bool buffer_insert_char(wchar_t ch, u32 index, BufferNode *n) {
  // Make sure the column buffer is at maximum and can insert a new character.
  if (n->string_length == BUFF_COL) {
    return false;
  }

  if (n->string_length >= n->vector_length) {
    buffer_increase_vector(n);
  }

  // Move all characters from the index forward.
  memcpy(&n->vector[index + 1], &n->vector[index],
         (n->string_length - index) * sizeof(wchar_t));

  // Insert the new character.
  n->vector[index] = ch;
  n->string_length++;

  return true;
}

bool buffer_delete_char(u32 index, BufferNode *n) {
  // Make sure the index isn't out of bounds and there are characters to remove.
  if (index >= n->string_length || n->string_length == 0) {
    return false;
  }

  // Move all characters in front to the position of the character to be
  // deleted.
  memcpy(&n->vector[index], &n->vector[index + 1],
         (n->string_length - index) * sizeof(wchar_t));

  n->string_length--;

  return true;
}

void buffer_free(Buffer *b) {
  BufferNode *node = b->begin;

  while (node) {
    BufferNode *temp = node->next;
    if (node->vector) {
      free(node->vector);
    }
    free(node);
    node = temp;
  }

  free(b);
}

BufferNode *buffer_valid_prev(BufferNode *n) {
  n = n->prev;
  while (n && !n->activated) {
    n = n->prev;
  }
  return n;
}

BufferNode *buffer_valid_next(BufferNode *n) {
  n = n->next;
  while (n && !n->activated) {
    n = n->next;
  }
  return n;
}
