/*
 * File: section.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "section.h"
#include "command.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void section_set_msg(Section *s, const char *msg) {
  strncpy(s->msg, msg, BUFF_STR);
}

void text_up(Section *s) {
  s->beg_row--;
  s->buffer->top = s->buffer->top->prev;
}

void text_down(Section *s) {
  s->beg_row++;
  s->buffer->top = s->buffer->top->next;
}

void push_undo(Section *s, UndoType type) {
  if (s->undo->nodes == UNDO_MAX_STACK) {
    undo_bottom_free(s->undo);
  }
  undo_node_push(s->undo, type, s->buffer->current);
}

void pop_undo(Section *s) {
  UndoStack *stack = s->undo;

  if (stack->nodes == 0) {
    section_set_msg(s, "Já está na alteração mais antiga.");
    return;
  }

  UndoNode *pop = undo_node_pop(s->undo);
  BufferNode *row = pop->row;

  switch (pop->type) {
  case UNDO_ROW:
    memcpy(row->buffer, pop->state, sizeof(wchar_t) * pop->length);
    row->buffer_len = pop->length;
    break;
  case UNDO_NEW_ROW:
    buffer_remove_node(s->buffer, row);
    free(pop);
    pop_undo(s);
    break;
  case UNDO_REMOVE_ROW:
    row->activated = true;
    free(pop);
    pop_undo(s);
    break;
  }

  free(pop->state);
  free(pop);
}
