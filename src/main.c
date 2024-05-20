/*
 * File: main.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * The entry of Beditor application.
 */
#include "command.h"
#include "section.h"
#include "ui.h"
#include <locale.h>

static Section *section;

void loop(void) {
  Windows *windows = windows_init();
  Mode mode = MODE_NORMAL;

  if (!section->unamed) {
    paint_command_bar_file_info(section, windows->command);
  }

  while (true) {
    curs_set(false);

    paint_status_bar(mode, section, windows->status);
    paint_rows(section, windows->rows, windows->text);

    wmove(windows->text, section->cy, section->cx);

    refresh_windows(windows);

    curs_set(true);

    int key = getch();

    switch (key) {
    case KEY_UP:
      cursor_up(section, windows->text);
      continue;
    case KEY_DOWN:
      cursor_down(section, windows->text);
      continue;
    case KEY_LEFT:
      cursor_left(section, windows->text);
      continue;
    case KEY_RIGHT:
      cursor_right(section, windows->text);
      continue;
    case KEY_HOME:
      cursor_home(section, windows->text);
      continue;
    case KEY_END:
      cursor_end(section, windows->text);
      continue;
    case KEY_PPAGE:
      cursor_pgup(section, windows->text);
      continue;
    case KEY_NPAGE:
      cursor_pgdown(section, windows->text);
      continue;
    case KEY_ESC:
      mode = MODE_NORMAL;
      continue;
    }

    if (mode == MODE_INSERT) {
      UTFChar ch = get_utfchar((char *)&key);

      if (!insert_at(section, ch, section->col)) {
        paint_command_bar("Erro: Falha ao inserir, buffer de linha cheio",
                          COLOR_PAIR(PAIR_TEXT), windows->command);
      }

      continue;
    }

    switch (key) {
    case 'k':
      cursor_up(section, windows->text);
      continue;
    case 'j':
      cursor_down(section, windows->text);
      continue;
    case 'h':
      cursor_left(section, windows->text);
      continue;
    case 'l':
      cursor_right(section, windows->text);
      continue;
    case 'i':
      mode = MODE_INSERT;
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
