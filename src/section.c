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
#include <stdarg.h>

void section_set_msg(Section *s, const char *msg, ...) {
  va_list args;
  va_start(args, msg);

  vsnprintf(s->msg, BUFF_STR, msg, args);

  va_end(args);
}

void text_up(Section *s) {
  s->beg_row--;
  s->buffer->top = s->buffer->top->prev;
}

void text_down(Section *s) {
  s->beg_row++;
  s->buffer->top = s->buffer->top->next;
}

void push_undo(Section *s, UndoType type, BufferNode *target, BufferNode *aux) {
  undo_node_push(s->undo, type, target, aux, s->row, s->col);
}

static void get_time_difference(struct tm *node_time, char *buff, u32 node) {
  time_t time_seed = time(NULL);
  struct tm current_time = *localtime(&time_seed);

  int diff;
  char diff_buff[16];

  if (current_time.tm_year != node_time->tm_year) {
    diff = current_time.tm_year - node_time->tm_year;
    strncpy(diff_buff, diff > 1 ? "anos" : "ano", 16);
  } else if (current_time.tm_mon != node_time->tm_mon) {
    diff = current_time.tm_mon - node_time->tm_mon;
    strncpy(diff_buff, diff > 1 ? "meses" : "mês", 16);
  } else if (current_time.tm_yday != node_time->tm_yday) {
    diff = current_time.tm_yday - node_time->tm_yday;
    strncpy(diff_buff, diff > 1 ? "dias" : "dia", 16);
  } else if (current_time.tm_min != node_time->tm_min) {
    diff = current_time.tm_min - node_time->tm_min;
    strncpy(diff_buff, diff > 1 ? "minutos" : "minuto", 16);
  } else {
    diff = current_time.tm_sec - node_time->tm_sec;
    strncpy(diff_buff, diff > 1 ? "segundos" : "segundo", 16);
  }

  snprintf(buff, BUFF_STR, "Retornado a alteração %d. Há %d %s", node, diff,
           diff_buff);
}

void pop_undo(Section *s) {
  UndoStack *stack = s->undo;

  if (stack->nodes == 0) {
    section_set_msg(s, "Já está na alteração mais antiga.");
    return;
  }

  stack->dirty = false;

  UndoNode pop = undo_node_pop(s->undo);

  if (pop.type == UNDO_NEW_ROW) {
    buffer_remove_node(s->buffer, pop.ptr_target);
    s->rows--;
  } else if (pop.type == UNDO_REMOVE_ROW) {
    pop.ptr_target->activated = true;
    s->rows++;
  }

  char buff[BUFF_STR];
  get_time_difference(&pop.time, buff, s->undo->nodes + 1);

  section_set_msg(s, buff);

  u32 maxy = getmaxy(s->window_text);

  BufferNode *destiny_node =
      pop.type != UNDO_NEW_ROW ? pop.ptr_target : pop.ptr_aux;

  if (!(pop.row >= s->beg_row && pop.row <= maxy)) {
    s->buffer->top = destiny_node;
    s->beg_row = pop.row;
  }

  s->buffer->current = destiny_node;
  s->cursor_y = pop.row - s->beg_row;
  s->row = pop.row;

  if (s->col < pop.col) {
    while (s->col < pop.col) {
      cursor_right(s);
    }
  } else {
    while (s->col > pop.col) {
      cursor_left(s);
    }
  }

  s->undo->dirty = false;
}
