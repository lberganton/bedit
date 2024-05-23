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

static Section *section;

void loop(void) {
  section->window = windows_init();
  Mode mode = MODE_NORMAL;

  while (true) {
    curs_set(false);

    paint_command_bar(section->msg, COLOR_PAIR(PAIR_TEXT), section);
    paint_status_bar(mode, section);
    paint_rows(section);

    wmove(section->window->text, section->cy, section->cx);

    refresh_windows(section->window);

    curs_set(true);

    int key = getch();

    switch (key) {
    case KEY_UP:
      cursor_up(section);
      continue;
    case KEY_DOWN:
      cursor_down(section);
      continue;
    case KEY_LEFT:
      cursor_left(section);
      continue;
    case KEY_RIGHT:
      cursor_right(section);
      continue;
    case KEY_HOME:
      cursor_home(section);
      continue;
    case KEY_END:
      cursor_end(section);
      continue;
    case KEY_PPAGE:
      cursor_pgup(section);
      continue;
    case KEY_NPAGE:
      cursor_pgdown(section);
      continue;
    case KEY_DC:
      delete_char(section);
      continue;
    case KEY_ESC:
      mode = MODE_NORMAL;
      continue;
    case KEY_RESIZE:
      windows_end(section->window);
      section->window = windows_init();
      continue;
    }

    if (mode == MODE_INSERT) {
      section->dirty = true;

      switch (key) {
      case KEY_BACKSPACE:
        backspace_char(section);
        continue;
      case '\t':
        insert_tab(section);
        continue;
      case '\n':
        insert_new_line(section);
        continue;
      default:
        insert_char(section, key);
        continue;
      }
    }

    switch (key) {
    case 'k':
      cursor_up(section);
      continue;
    case 'j':
      cursor_down(section);
      continue;
    case 'h':
      cursor_left(section);
      continue;
    case 'l':
      cursor_right(section);
      continue;
    case 'i':
      mode = MODE_INSERT;
      continue;
    case ':':
      input_command(section);
      continue;
    }
  }
}

int main(int argc, char **argv) {
  setlocale(LC_CTYPE, "");

  ABORT(argc > 2, ERROR_WRONG_ARGS_NUM);

  // If no argument was provided or the file dosen't exist, start a section
  // in a 'unamed' style.
  if (argc == 1 || !file_exist(argv[1])) {
    section = section_unamed();
  } else {
    section = section_open(argv[1]);
  }

  ui_init();

  loop();

  ui_end();

  // section_end(section);

  return 0;
}
