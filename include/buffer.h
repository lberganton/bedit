/*
 * File: buffer.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * Buffer-related things.
 */
#pragma once

#include "defs.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct BufferNode {
  bool activated;
  wchar_t *vector;
  u32 string_length;
  u32 vector_length;
  struct BufferNode *prev;
  struct BufferNode *next;
} BufferNode;

typedef struct Buffer {
  BufferNode *begin;
  BufferNode *end;
  BufferNode *current;
  BufferNode *top;
  u32 nodes;
} Buffer;

Buffer *buffer_init(void);
BufferNode *buffer_insert_prev(Buffer *b, BufferNode *n);
BufferNode *buffer_insert_next(Buffer *b, BufferNode *n);
void buffer_increase_vector(BufferNode *node, size_t length);
void buffer_decrease_vector(BufferNode *node, size_t length);
void buffer_remove_node(Buffer *b, BufferNode *n);
void buffer_free(Buffer *b);

BufferNode *buffer_valid_prev(BufferNode *n);
BufferNode *buffer_valid_next(BufferNode *n);

bool buffer_insert_char(wchar_t ch, u32 index, BufferNode *n);
bool buffer_delete_char(u32 index, BufferNode *n);
