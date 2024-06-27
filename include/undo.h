/*
 * File: undo.h
 * Author: Lucas Berganton
 * Created: 05/23/2024
 */
#pragma once

#include "buffer.h"
#include "defs.h"
#include <time.h>

#define UNDO_MAX_STACK 100

typedef enum UndoType { UNDO_ROW, UNDO_NEW_ROW, UNDO_REMOVE_ROW } UndoType;

typedef struct UndoChange {
  UndoType type;
  BufferNode *target;

  wchar_t *state;
  u32 length;

  struct UndoChange *next;
} UndoChange;

typedef struct UndoNode {
  struct tm time;

  BufferNode *current;
  u32 row;
  u32 col;

  UndoChange *begin;
  UndoChange *end;

  struct UndoNode *prev;
  struct UndoNode *next;
} UndoNode;

typedef struct UndoStack {
  UndoNode *top;
  UndoNode *bot;
  u32 nodes;
  bool dirty;
} UndoStack;

UndoStack *undo_stack_create(void);
void undo_stack_free(UndoStack *stack);

void undo_node_push(UndoStack *stack, BufferNode *current, u32 row, u32 col);
UndoNode undo_node_pop(UndoStack *stack, Buffer *buffer, u32 *rows);
void undo_node_free(UndoStack *stack, UndoNode *node);

void undo_node_insert(UndoStack *stack, UndoType type, BufferNode *target);
void undo_node_remove(UndoNode *node);
