#include "section.h"
#include <ncurses.h>

void cursor_move(WINDOW *w, u32 y, u32 x) {
  wmove(w, y, x);
}
