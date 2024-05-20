/*
 * File: main.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * The entry of Beditor application.
 */
#include "section.h"
#include "ui.h"
#include <locale.h>

static Section *section;

void loop(void) {
  Windows *windows = windows_init();

  if (!section->unamed) {
    paint_command_bar_file_info(section, windows->command);
  }

  while (true) {
    curs_set(false);

    paint_status_bar(MODE_NORMAL, section, windows->status);
    paint_rows(section, windows->rows, windows->text);

    wmove(windows->text, section->cy, section->cx);

    refresh_windows(windows);

    curs_set(true);

    int key = getch();

    switch (key) {
    case KEY_UP:
    case 'k':
      cursor_up(section, windows->text);
      break;
    case KEY_DOWN:
    case 'j':
      cursor_down(section, windows->text);
      break;
    case KEY_LEFT:
    case 'h':
      cursor_left(section, windows->text);
      break;
    case KEY_RIGHT:
    case 'l':
      cursor_right(section, windows->text);
      break;
    case KEY_HOME:
      cursor_home(section, windows->text);
      break;
    case KEY_END:
      cursor_end(section, windows->text);
      break;
    case KEY_PPAGE:
      cursor_pgup(section, windows->text);
      break;
    case KEY_NPAGE:
      cursor_pgdown(section, windows->text);
      break;
    case 'i':
      mode_insert(section);
      break;
    case ':':
      mode_command(section);
      break;
    default:
      insert_char_at(get_utfchar((char *) &key), section->col, section->buffer->current);
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
