/*
 * File: main.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * The entry of Beditor application.
 */
#include <ncurses.h>

int main(void) {
  initscr();

  printw("Hello, world!");
  getch();

  endwin();
  return 0;
}
