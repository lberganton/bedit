/*
 * File: buffer.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * Buffer-related things.
 */
#pragma once

#include "char.h"
#include "defs.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct BufferNode {
  UTFChar buffer[BUFF_SIZE];
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
void buffer_read_file(const char *file_name, Buffer *b);
BufferNode *buffer_insert_begin(Buffer *b);
BufferNode *buffer_insert_end(Buffer *b);
BufferNode *buffer_insert_next(Buffer *b, BufferNode *n);

bool buffer_insert_char(UTFChar ch, u32 index, BufferNode *n);
bool buffer_delete_char(u32 index, BufferNode *n);
