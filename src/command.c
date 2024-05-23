#include "command.h"
#include "char.h"
#include "ui.h"
#include <ctype.h>
#include <string.h>

Command tokenize_command(char *input) {
  Command command = {0};
  char *token = strtok(input, " ");

  if (token == NULL) {
    return command;
  }

  command.token[command.amount++] = token;

  while ((token = strtok(NULL, " "))) {
    command.token[command.amount++] = token;
  }

  return command;
}

void input_command(Section *s) {
  char buffer[BUFF_STR];
  u32 pos = 0;

  paint_background(s->window->command, COLOR_PAIR(PAIR_TEXT));
  wattrset(s->window->command, COLOR_PAIR(PAIR_TEXT));
  mvwaddch(s->window->command, 0, 0, ':');

  wmove(s->window->command, 0, 1);

  u32 maxx = getmaxx(s->window->command) - 1;
  int key;

  while ((key = wgetch(s->window->command)) != '\n') {
    if (pos == maxx - 1) {
      continue;
    }

    if (key == KEY_RESIZE) {
      // Code
      continue;
    }

    if (key == KEY_ESC) {
      section_set_msg(s, "");
      return;
    }

    if (key == KEY_BACKSPACE) {
      if (pos == 0) {
        continue;
      }
      mvwaddch(s->window->command, 0, pos, ' ');
      wmove(s->window->command, 0, pos);
      pos--;
      continue;
    }

    if (isgraph(key) || key == ' ') {
      buffer[pos] = key;
      mvwaddch(s->window->command, 0, pos + 1, buffer[pos]);
      pos++;
    }
  }

  buffer[pos] = '\0';
  Command command = tokenize_command(buffer);

  if (command.amount == 0) {
    section_set_msg(s, "Erro: Comando inválido.");
  } else {
    set_command(s, command);
  }

  curs_set(false);
}

void set_command(Section *s, Command c) {
  // Write.
  if (strncmp(c.token[0], "w", BUFF_STR) == 0) {
    if (c.amount == 1) {
      if (s->unamed) {
        section_set_msg(s, "Erro: Nenhum nome de arquivo.");
        return;
      }
      command_write(s);
      return;
    }

    if (c.amount > 2) {
      section_set_msg(s, "Erro: Forneça apenas um nome para o arquivo.");
      return;
    }

    char *ptr = c.token[1];

    if (ptr[0] == '\"') {
      u32 i = strlen(ptr) - 1;

      if (ptr[i] != '\"') {
        section_set_msg(s, "Erro: Feche as aspas para dar o caminho completo.");
        return;
      }

      ptr[i] = '\0';
      ptr++;
    }

    file_get_input(ptr, s->file_input);
    file_get_name(s->file_input, s->file_name);
    file_get_directory(s->file_input, s->file_directory);

    command_write(s);
    return;
  }

  // Quit.
  if (strncmp(c.token[0], "q", BUFF_STR) == 0) {
    if (c.amount > 1) {
      section_set_msg( s, "Erro: O comando sair não exige argumentos.");
      return;
    }

    if (s->dirty) {
      section_set_msg(
          s, "Erro: Alterações não foram gravadas (Use q! para forçar).");
      return;
    }

    exit(0);
  }

  // Force quit.
  if (strncmp(c.token[0], "q!", BUFF_STR) == 0) {
    if (c.amount > 1) {
      section_set_msg( s, "Erro: O comando sair não exige argumentos.");
      return;
    }

    exit(0);
  }

  section_set_msg(s, "Erro: Comando inválido.");
}

void command_write(Section *s) {
  if (!file_write(s->file_input, s->buffer)) {
    section_set_msg(s, "Erro: Falha ao escrever no arquivo.\n");
    return;
  }

  snprintf(s->msg, BUFF_STR, "\"%s\" %" PRIu32 "L %" PRIu32 "B (Gravados)",
           s->file_name, s->rows, file_get_size(s->file_input));

  s->dirty = false;
}
