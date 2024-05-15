/*
 * File: section.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "section.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>

static Section *section_create(void) {
  Section *new = (Section *)malloc(sizeof(Section));
  ABORT(new == NULL, "Erro: Falha ao alocar memória para seção.");

  new->buffer = buffer_init();
  new->dirty = false;
  new->top_row = 1;
  new->cy = 0;
  new->cx = 0;
  new->row = 1;
  new->col = 1;

  return new;
}

Section *section_unamed(void) {
  Section *new = section_create();

  new->file_name = NO_NAME_FILE;
  new->unamed = true;
  new->file_extension = EXTENSION_UNKNOWN;

  return new;
}

Section *section_open(char *file_name) {
  Section *new = section_create();

  new->file_name = file_name;
  new->unamed = false;
  new->file_extension = file_get_extension(file_name);

  FILE *file = fopen(file_name, "r");
  ABORT(file == NULL, "Erro: Falha ao abrir o arquivo.");

  buffer_read_file(new->buffer, file);

  return new;
}

u32 get_rows(Section *s) { return s->buffer->nodes; }

void mode_normal(Section *s) {
  Windows *windows = windows_init();

  paint_command_bar("Teste", COLOR_PAIR(PAIR_TEXT), windows->command);
  paint_status_bar(MODE_NORMAL, s, windows->status);
  paint_rows(s, windows->rows, windows->text);

  while (true) {
    cursor_set(s, windows->text, s->cy, s->cx);
    
    refresh_windows(windows);

    int key = getch();

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
