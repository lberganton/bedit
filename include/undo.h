/*
 * File: undo.h
 * Author: Lucas Berganton
 * Created: 05/23/2024
 */
#pragma once

#include "buffer.h"
#include "defs.h"

#define UNDO_MAX_STACK 100

typedef enum UndoType { UNDO_ROW, UNDO_NEW_ROW, UNDO_REMOVE_ROW } UndoType;

typedef struct UndoNode {
  UndoType type;
  UTFChar *state;
  struct UndoNode *next;
  struct UndoNode *prev;
} UndoNode;

typedef struct UndoStack {
  UndoNode *top;
  UndoNode *bot;
  u32 nodes;
  bool dirty;
} UndoStack;

UndoStack *undo_stack_create(void);
void undo_stack_free(UndoStack *s);

UndoNode *undo_node_create(UndoStack *s, UndoType t, BufferNode *b);
