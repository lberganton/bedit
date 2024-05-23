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

static Section *section_create(void) {
  Section *new = (Section *)malloc(sizeof(Section));
  ABORT(new == NULL, "Erro: Falha ao alocar memória para seção.");

  new->undo = undo_stack_create();
  new->buffer = buffer_init();
  new->dirty = false;
  new->beg_row = 0;
  new->beg_col = 0;
  new->cy = 0;
  new->cx = 0;
  new->row = 0;
  new->col = 0;

  return new;
}

Section *section_unamed(void) {
  Section *new = section_create();

  new->unamed = true;
  new->file_extension = EXTENSION_UNKNOWN;
  new->rows = 1;

  return new;
}

Section *section_open(const char *file_name) {
  ABORT(strlen(file_name) >= BUFF_STR,
        "Erro: O caminho do arquivo é muito grande.");

  Section *new = section_create();

  new->unamed = false;

  file_get_input(file_name, new->file_input);
  file_get_name(file_name, new->file_name);
  file_get_directory(file_name, new->file_directory);
  file_read(file_name, new->buffer);
  new->file_extension = file_get_extension(file_name);

  snprintf(new->msg, BUFF_STR, "\"%s\" %" PRIu32 "L %" PRIu32 "B", file_name,
           new->buffer->nodes, file_get_size(file_name));

  new->rows = new->buffer->nodes;
  return new;
}

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
    memcpy(row->buffer, pop->state, sizeof(UTFChar) * pop->length);
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
