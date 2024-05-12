/*
 * File: section.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "section.h"
#include <stdio.h>
#include <stdlib.h>

static Section *section_create(void) {
  Section *new = (Section *)malloc(sizeof(Section));
  ABORT(new == NULL, "Erro: Falha ao alocar memória para seção.");
  return new;
}

Section *section_unamed(void) {
  Section *new = section_create();

  new->file_name = NULL;
  new->dirty = false;
  new->unamed = true;
  new->row = 0;
  new->col = 0;

  new->windows = windows_init();
  new->buffer = buffer_init();

  return new;
}

Section *section_open(char *file_name) {
  Section *new = section_create();

  new->file_name = file_name;
  new->dirty = false;
  new->unamed = false;
  new->row = 0;
  new->col = 0;

  new->windows = windows_init();
  new->buffer = buffer_init();

  FILE *file = fopen(file_name, "r");
  ABORT(file == NULL, "Erro: Falha ao abrir o arquivo.");

  buffer_read_file(new->buffer, file);

  return new;
}

void mode_normal(Section *s) {
  while (true) {
    paint_windows(s->row, s->top_row, s->windows, s->buffer);

    int key = get_key();

    switch (key) {
    case 'i':
      mode_insert(s);
      break;
    case ':':
      mode_command(s);
      break;
    }
  }
}

void mode_insert(Section *s) {}

void mode_command(Section *s) {}
