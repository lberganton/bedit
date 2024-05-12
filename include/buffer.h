/*
 * File: buffer.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * Buffer-related things.
 */
#pragma once

#include <stdbool.h>
#include <stdio.h>

#define BUFF_SIZE 1024

typedef struct BufferNode {
  char buffer[BUFF_SIZE];
  size_t buffer_len;
  struct BufferNode *prev;
  struct BufferNode *next;
} BufferNode;

typedef struct Buffer {
  BufferNode *begin;
  BufferNode *end;
  size_t nodes;
} Buffer;

Buffer *buffer_init(void);
void buffer_read_file(Buffer *b, FILE *f);
void buffer_print(Buffer *b);
bool is_utf(char ch);
