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

#define BUFF_SIZE 1024

typedef struct BufferNode {
  char buffer[BUFF_SIZE];
  u32 buffer_len;
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
void buffer_read_file(Buffer *b, FILE *f);
u8 get_char_encoding(char *ch);
