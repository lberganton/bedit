/*
 * File: char.h
 * Author: Lucas Berganton
 * Created: 05/18/2024
 */
#pragma once

#include "defs.h"

// Struct to deal with utf-8 characters.
typedef struct UTFChar {
  char data[4];
  u8 size;
} UTFChar;

u8 get_utf_len(char *ch);
UTFChar get_utfchar(char *ch);
UTFChar set_utfchar(char *in, UTFChar *ch);
void paint_char(WINDOW *w, u32 y, u32 x, attr_t attr, char *ch);
void paint_string(WINDOW *w, u32 y, u32 x, attr_t attr, size_t len, char *str);
void paint_utfchar(WINDOW *w, u32 y, u32 x, attr_t attr, UTFChar ch);
void paint_utfstring(WINDOW *w, u32 y, u32 x, attr_t attr, size_t len,
                     char *str);
