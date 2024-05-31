/*
 * File: input.h
 * Author: Lucas Berganton
 * Created: 05/21/2024
 */
#pragma once

#include "section.h"
#include <wchar.h>

#define TAB_SIZE 4
#define AUTO_PAIRS true

void insert_tab(Section *s);
void insert_char(Section *s, wchar_t ch);
void delete_char(Section *s);
void backspace_char(Section *s);
void insert_pairs(Section *s, wchar_t ch);
void insert_new_line(Section *s);
bool merge_line(Section *s, BufferNode *dest, BufferNode *src);
