/*
 * File: char.h
 * Author: Lucas Berganton
 * Created: 05/18/2024
 */
#pragma once

#include "defs.h"

typedef struct UTFChar {
    char data[4];
    u8 size;
} UTFChar;

u8 get_encoding(char *ch);
UTFChar get_utfchar(char *ch);
void set_utfchar(char *in, UTFChar *ch);
