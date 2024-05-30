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

u8 get_utf_len(const char *ch);
UTFChar get_utfchar(const char *ch);
int utfchar_to_int(UTFChar ch);
bool compare_utfchar(UTFChar ch1, UTFChar ch2);

void paint_char(WINDOW *w, u32 y, u32 x, attr_t attr, const char *ch);
void paint_utfchar(WINDOW *w, u32 y, u32 x, attr_t attr, UTFChar ch);

void paint_string(WINDOW *w, u32 y, u32 x, attr_t attr, size_t len,
                  const char *str);
void paint_utfstring(WINDOW *w, u32 y, u32 x, attr_t attr, size_t len,
                     const char *str);
