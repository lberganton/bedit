/*
 * File: char.h
 * Author: Lucas Berganton
 * Created: 05/18/2024
 */
#pragma once

#include "defs.h"
#include <wchar.h>

void paint_char(WINDOW *w, u32 y, u32 x, attr_t attr, wchar_t ch);
void paint_string(WINDOW *w, u32 y, u32 x, attr_t attr, size_t len,
                  const char *str);
