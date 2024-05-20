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

Section *section_open(char *file_name) {
  Section *new = section_create();

  new->unamed = false;

  file_get_name(file_name, new->file_name);
  file_get_directory(file_name, new->file_directory);
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

  while (true) {
    curs_set(false);

    paint_status_bar(MODE_NORMAL, s, windows->status);
    paint_rows(s, windows->rows, windows->text);

    wmove(windows->text, s->cy, s->cx);

    refresh_windows(windows);

    curs_set(true);

    int key = getch();

    switch (key) {
    case KEY_UP:
      cursor_up(s, windows->text);
      break;
    case KEY_DOWN:
      cursor_down(s, windows->text);
      break;
    case KEY_LEFT:
      cursor_left(s, windows->text);
      break;
    case KEY_RIGHT:
      cursor_right(s, windows->text);
      break;
    case KEY_HOME:
      cursor_home(s, windows->text);
      break;
    case KEY_END: 
      cursor_end(s, windows->text);
      break;
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
