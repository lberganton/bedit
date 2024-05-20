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

  buffer_read_file(file_name, new->buffer);

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

void mode_normal(Section *s) {
  Windows *windows = windows_init();

  if (!s->unamed) {
    paint_command_bar_file_info(s, windows->command);
  }

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
    case 'k':
      cursor_up(s, windows->text);
      break;
    case KEY_DOWN:
    case 'j':
      cursor_down(s, windows->text);
      break;
    case KEY_LEFT:
    case 'h':
      cursor_left(s, windows->text);
      break;
    case KEY_RIGHT:
    case 'l':
      cursor_right(s, windows->text);
      break;
    case KEY_HOME:
      cursor_home(s, windows->text);
      break;
    case KEY_END:
      cursor_end(s, windows->text);
      break;
    case KEY_PPAGE:
      cursor_pgup(s, windows->text);
      break;
    case KEY_NPAGE:
      cursor_pgdown(s, windows->text);
      break;
    case 'i':
      mode_insert(s);
      break;
    case ':':
      mode_command(s);
      break;
    default:
      insert_char_at(get_utfchar((char *) &key), s->col, s->buffer->current);
    }
  }
}

void mode_insert(Section *s) {}

void mode_command(Section *s) {}
