/*
 * File: command.h
 * Author: Lucas Berganton
 * Created: 05/20/2024
 */
#pragma once

#include "section.h"
#include <ncurses.h>

typedef struct Command {
  char *token[BUFF_TOK];
  u8 amount;
} Command;

void input_command(Section *s);
Command tokenize_command(char *input);
void set_command(Section *s, Command c);

void command_write(Section *s);
