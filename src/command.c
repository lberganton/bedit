/*
 * File: command.c
 * Author: Lucas Berganton
 * Created: 05/20/2024
 */
#include "command.h"
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

  paint_background(s->window_command, COLOR_PAIR(PAIR_TEXT));
  wattrset(s->window_command, COLOR_PAIR(PAIR_TEXT));
  mvwaddch(s->window_command, 0, 0, ':');

  wmove(s->window_command, 0, 1);

  u32 maxx = getmaxx(s->window_command) - 1;
  int key;

  while ((key = wgetch(s->window_command)) != '\n') {
    if (key == KEY_BACKSPACE) {
      if (pos == 0) {
        continue;
      }
      mvwaddch(s->window_command, 0, pos, ' ');
      wmove(s->window_command, 0, pos);
      pos--;
      continue;
    }

    if (pos == maxx - 1) {
      continue;
    }

    if (key == KEY_ESC) {
      section_set_msg(s, "");
      return;
    }

    if (isgraph(key) || key == ' ') {
      buffer[pos] = key;
      mvwaddch(s->window_command, 0, pos + 1, buffer[pos]);
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
  // Write and Write and Quite.
  if (strncmp(c.token[0], "w", BUFF_STR) == 0 ||
      strncmp(c.token[0], "wq", BUFF_STR) == 0) {
    if (c.amount == 1) {
      if (s->unamed) {
        section_set_msg(s, "Erro: Nenhum nome de arquivo.");
        return;
      }
      command_write(s);

      if (c.token[0][1] == 'q') {
        exit(0);
      }

      return;
    }

    if (c.amount > 2) {
      section_set_msg(s, "Erro: Forneça apenas um nome para o arquivo.");
      return;
    }

    char *ptr = c.token[1];

    if (strchr(ptr, '\"')) {
      u32 i = strlen(ptr) - 1;

      if (ptr[0] == '\"' && ptr[i] == '\"') {
        ptr[i] = '\0';
        ptr++;
      } else {
        section_set_msg(s, "Erro: Feche as aspas para dar o caminho completo.");
        return;
      }
    }

    file_get_input(ptr, s->file_full);
    file_get_name(s->file_full, s->file_name);
    file_get_directory(s->file_full, s->file_path);
    s->file_extension = file_get_extension(s->file_full);
    s->unamed = false;

    command_write(s);

    if (c.token[0][1] == 'q') {
      exit(0);
    }

    return;
  }

  // Quit.
  if (strncmp(c.token[0], "q", BUFF_STR) == 0) {
    if (c.amount > 1) {
      section_set_msg(s, "Erro: O comando sair não exige argumentos.");
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
      section_set_msg(s, "Erro: O comando sair não exige argumentos.");
      return;
    }

    exit(0);
  }

  // Debug mode.
  if (strncmp(c.token[0], "debug", BUFF_STR) == 0) {
    s->debug = !s->debug;
    return;
  }

  section_set_msg(s, "Erro: Comando inválido.");
}

void command_write(Section *s) {
  if (!file_write(s->file_full, s->buffer)) {
    section_set_msg(s, "Erro: Falha ao escrever no arquivo.");
    return;
  }

  section_set_msg(s, "\"%s\" %" PRIu32 "L %" PRIu32 "B (Gravados)",
                  s->file_name, s->rows, file_get_size(s->file_full));

  s->dirty = false;
}
