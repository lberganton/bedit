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

static Section *section_create(void) {
  Section *new = (Section *)malloc(sizeof(Section));
  ABORT(new == NULL, "Erro: Falha ao alocar memória para seção.");

  new->file_input = (char *)malloc(sizeof(char) * BUFF_SIZE);
  new->file_name = (char *)malloc(sizeof(char) * BUFF_SIZE);
  new->file_directory = (char *)malloc(sizeof(char) * BUFF_SIZE);
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

  new->file_name = NO_NAME_FILE;
  new->unamed = true;
  new->file_extension = EXTENSION_UNKNOWN;

  return new;
}

Section *section_open(const char *file_name) {
  Section *new = section_create();

  new->unamed = false;

  file_get_input(file_name, new->file_input);
  file_get_name(file_name, new->file_name);
  file_get_directory(file_name, new->file_directory);
  new->file_extension = file_get_extension(file_name);

  file_load(file_name, new->buffer);

  return new;
}

u32 get_rows(Section *s) { return s->buffer->nodes; }

void text_up(Section *s) {
  s->beg_row--;
  s->buffer->top = s->buffer->top->prev;
}

void text_down(Section *s) {
  s->beg_row++;
  s->buffer->top = s->buffer->top->next;
}
