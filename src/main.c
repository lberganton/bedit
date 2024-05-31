/*
 * File: main.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * The entry of Beditor application.
 */
#include "command.h"
#include "input.h"
#include "section.h"
#include "ui.h"
#include <locale.h>
#include <string.h>

static Section *section;
static Mode mode = MODE_NORMAL;

void exit_bedit(void) {
  free_windows(section);
  buffer_free(section->buffer);

  if (stdscr) {
    endwin();
  }
}

void handle_with_input(int key) {
  switch (key) {
  case KEY_UP:
    cursor_up(section);
    return;
  case KEY_DOWN:
    cursor_down(section);
    return;
  case KEY_LEFT:
    cursor_left(section);
    return;
  case KEY_RIGHT:
    cursor_right(section);
    return;
  case KEY_HOME:
    cursor_home(section);
    return;
  case KEY_END:
    cursor_end(section);
    return;
  case KEY_PPAGE:
    cursor_pgup(section);
    return;
  case KEY_NPAGE:
    cursor_pgdown(section);
    return;
  case KEY_DC:
    delete_char(section);
    return;
  case KEY_ESC:
    mode = MODE_NORMAL;
    return;
  case KEY_RESIZE:
    free_windows(section);
    initialize_windows(section);
    return;
  }

  if (mode == MODE_INSERT) {
    section->dirty = true;

    switch (key) {
    case KEY_BACKSPACE:
      backspace_char(section);
      return;
    case '\t':
      insert_tab(section);
      return;
    case '\n':
      insert_new_line(section);
      return;
    default:
      insert_char(section, key);
      return;
    }
  }

  switch (key) {
  case 'k':
    cursor_up(section);
    return;
  case 'j':
    cursor_down(section);
    return;
  case 'h':
    cursor_left(section);
    return;
  case 'l':
    cursor_right(section);
    return;
  case 'i':
    mode = MODE_INSERT;
    return;
  case ':':
    input_command(section);
    return;
  case 'u':
    pop_undo(section);
    return;
  }
}

int main(int argc, char **argv) {
  setlocale(LC_CTYPE, "");

  // Exit the program if more than one file was provided.
  ASSERT(argc > 2, ERROR_WRONG_ARGS_NUM);

  char *file_path = argv[1];

  // Allocate memory for a section.
  section = (Section *)malloc(sizeof(Section));
  ASSERT(section == NULL, "Erro: Falha ao alocar memória para seção.");

  section->undo = undo_stack_create();
  section->buffer = buffer_init();
  section->dirty = false;

  section->row = 0;
  section->col = 0;
  section->beg_row = 0;
  section->beg_col = 0;
  section->cy = 0;
  section->cx = 0;

  if (file_path == NULL) {
    // If no file was provides as a argument.
    section->unamed = true;
    section->file_extension = EXTENSION_UNKNOWN;
    section->rows = 1;
  } else {
    section->unamed = false;

    ASSERT(strlen(file_path) >= BUFF_STR,
           "Erro: O caminho do arquivo é muito grande.");

    file_get_input(file_path, section->file_input);
    file_get_name(file_path, section->file_name);
    file_get_directory(file_path, section->file_directory);

    // Read the file to the buffer.
    file_read(file_path, section->buffer);

    // Get the file extension.
    section->file_extension = file_get_extension(file_path);

    // Set the section message to show the file path, rows and the file size.
    snprintf(section->msg, BUFF_STR, "\"%s\" %" PRIu32 "L %" PRIu32 "B",
             file_path, section->buffer->nodes, file_get_size(file_path));

    section->rows = section->buffer->nodes;
  }

  // Set a function to execute when 'exit' is called.
  atexit(exit_bedit);

  // Initiliaze the NCurses.
  initscr();

  raw();
  noecho();
  keypad(stdscr, true);

  // Set the RGB colors and color pairs.
  initialize_colors();
  
  // Refresh to set the main window.
  refresh();

  // Initialize NCurses windows.
  initialize_windows(section);
  int key;

  bool quit = false;

  while (!quit) {
    curs_set(false);

    // Paint all the NCurses windows.
    paint_command_bar(section->msg, COLOR_PAIR(PAIR_TEXT), section);
    paint_status_bar(mode, section);
    paint_rows(section);

    // Set the cursor to the current user position.
    wmove(section->window_text, section->cy, section->cx);
    refresh_windows(section);
    curs_set(true);

    get_wch(&key);

    handle_with_input(key);
  }

  exit_bedit();

  return 0;
}
