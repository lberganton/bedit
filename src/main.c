/*
 * File: main.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * The entry of Beditor application.
 */
#include "section.h"
#include <ncurses.h>

static Section *section;

int main(int argc, char **argv) {
  ABORT(argc > 2, ERROR_WRONG_ARGS_NUM);

  // If no argument was provided or the file dosen't exist, start a section
  // in a 'unamed' style.
  if (argc == 1 || !file_exist(argv[1])) {
    section = section_unamed();
  }
  else {
    section = section_open(argv[1]);
  }

  mode_normal(section);
  
  section_end(section);
  
  endwin();
  return 0;
}
