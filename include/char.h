/*
 * File: char.h
 * Author: Lucas Berganton
 * Created: 05/18/2024
 */
#pragma once

#include "defs.h"

typedef struct Char {
    char data[4];
    u8 size;
} Char;

u8 get_encoding(char ch);
char get_char(Char *ch);
void set_char(char in, Char *ch);
Char to_special_char(char ch);
