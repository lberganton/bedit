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

  strcpy(new->file_name, NO_NAME_FILE);
  new->unamed = true;
  new->file_extension = EXTENSION_UNKNOWN;

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
  file_load(file_name, new->buffer);
  new->file_extension = file_get_extension(file_name);

  sprintf(new->msg, "\"%s\" %" PRIu32 "L %" PRIu32 "B", file_name,
          new->buffer->nodes, file_get_size(file_name));

  return new;
}

void section_set_msg(Section *s, const char *msg) {
  strncpy(s->msg, msg, BUFF_STR);
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
