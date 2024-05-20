#include "command.h"
#include "char.h"
#include "ui.h"

void paint_command_bar_file_info(Section *s) {
  char buffer[BUFF_SIZE];

  sprintf(buffer, "\"%s\" %" PRIu32 "L %" PRIu32 "B", s->file_input,
          s->buffer->nodes, file_get_size(s->file_input));

  paint_command_bar(buffer, COLOR_PAIR(PAIR_TEXT), s);
}
