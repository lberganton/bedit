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
  undo_node_push(s->undo, type, s->buffer->current, s->row, s->col);
}

void pop_undo(Section *s) {
  UndoStack *stack = s->undo;

  if (stack->nodes == 0) {
    section_set_msg(s, "Já está na alteração mais antiga.");
    return;
  }

  UndoNode *pop = undo_node_pop(s->undo);
  BufferNode *row = pop->ptr;

  switch (pop->type) {
  case UNDO_ROW: {
    WRITE_LOG("Retirado da pilha de 'desfazer' um 'UNDO_ROW'.");

    time_t t = time(NULL);
    struct tm cur = *localtime(&t);

    int diff;
    char buff[16];

    if (cur.tm_year != pop->time.tm_year) {
      diff = cur.tm_year - pop->time.tm_year;
      strncpy(buff, diff > 1 ? "anos" : "ano", 16);
    } else if (cur.tm_mon != pop->time.tm_mon) {
      diff = cur.tm_mon - pop->time.tm_mon;
      strncpy(buff, diff > 1 ? "meses" : "mês", 16);
    } else if (cur.tm_yday != pop->time.tm_yday) {
      diff = cur.tm_yday - pop->time.tm_yday;
      strncpy(buff, diff > 1 ? "dias" : "dia", 16);
    } else if (cur.tm_min != pop->time.tm_min) {
      diff = cur.tm_min - pop->time.tm_min;
      strncpy(buff, diff > 1 ? "minutos" : "minuto", 16);
    } else {
      diff = cur.tm_sec - pop->time.tm_sec;
      strncpy(buff, diff > 1 ? "segundos" : "segundo", 16);
    }

    snprintf(s->msg, BUFF_STR, "Retornado a alteração #%d. Há %d %s.",
             s->undo->nodes + 1, diff, buff);

    memcpy(row->buffer, pop->state, sizeof(wchar_t) * pop->length);
    row->buffer_len = pop->length;

    u32 maxy = getmaxy(s->window_text);

    if (!(pop->row >= s->beg_row && pop->row <= maxy)) {
      s->buffer->top = pop->ptr;
      s->beg_row = pop->row;
    }

    s->buffer->current = pop->ptr;
    s->cursor_y = pop->row - s->beg_row;
    s->row = pop->row;
    s->cursor_x = 0;
    s->col = 0;

    while (s->col < pop->col) {
      cursor_right(s);
    }

    s->undo->dirty = false;

    free(pop->state);
    free(pop);
  } break;
  case UNDO_NEW_ROW:
    WRITE_LOG("Retirado da pilha de 'desfazer' um 'UNDO_NEW_ROW'.");

    buffer_remove_node(s->buffer, row);
    free(pop);

    s->rows--;
    
    pop_undo(s);
    break;
  case UNDO_REMOVE_ROW:
    WRITE_LOG("Retirado da pilha de 'desfazer' um 'UNDO_REMOVE_ROW'.");

    row->activated = true;
    free(pop);
    pop_undo(s);
    break;
  }
}
