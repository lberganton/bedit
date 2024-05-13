#include "section.h"
#include <ncurses.h>

void cursor_set(Section *s, WINDOW *w, u32 y, u32 x) {
  s->cx = x;
  s->cy = y;
  wmove(w, y, x);
}
