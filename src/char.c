/*
 * File: char.c
 * Author: Lucas Berganton
 * Created: 05/18/2024
 */
#include "char.h"
#include <ncurses.h>

// Gets the utf-8 length of the char. Code adapted from an answer of a
// Stack Overflow's user named Christoph.
// https://stackoverflow.com/questions/1031645/how-to-detect-utf-8-in-plain-c
u8 get_utf_len(const char *ch) {
  ASSERT(ch == NULL, "Erro: Falha em descobrir o encoding de um caractere.");

  unsigned char *bytes = (unsigned char *)ch;

  if (bytes[0] == 0x09 || bytes[0] == 0x0A || bytes[0] == 0x0D ||
      (0x20 <= bytes[0] && bytes[0] <= 0x7E)) {
    return 1;
  }

  if ((0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
      (0x80 <= bytes[1] && bytes[1] <= 0xBF)) {
    return 2;
  }

  if ((bytes[0] == 0xE0 && (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF)) ||
      (((0xE1 <= bytes[0] && bytes[0] <= 0xEC) || bytes[0] == 0xEE ||
        bytes[0] == 0xEF) &&
       (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF)) ||
      (bytes[0] == 0xED && (0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF))) {
    return 3;
  }

  if ((bytes[0] == 0xF0 && (0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
       (0x80 <= bytes[3] && bytes[3] <= 0xBF)) ||
      ((0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
       (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
       (0x80 <= bytes[3] && bytes[3] <= 0xBF)) ||
      (bytes[0] == 0xF4 && (0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
       (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
       (0x80 <= bytes[3] && bytes[3] <= 0xBF))) {
    return 4;
  }

  return 0;
}

UTFChar get_utfchar(const char *in) {
  UTFChar ch;
  ch.size = get_utf_len(in);

  // Set the array's data based on the utf-8 length.
  for (u8 i = 0; i < ch.size; i++) {
    ch.data[i] = in[i];
  }

  return ch;
}

int utfchar_to_int(UTFChar ch) {
  if (ch.size > 1) {
    return 0;
  }

  return ch.data[0];
}

bool compare_utfchar(UTFChar ch1, UTFChar ch2) {
  if (ch1.size != ch2.size) {
    return false;
  }
  
  for (u32 i = 0; i < ch1.size; i++) {
    if (ch1.data[i] != ch2.data[i]) {
      return false;
    }
  }

  return true;
}

void paint_char(WINDOW *w, u32 y, u32 x, attr_t attr, const char *ch) {
  u8 encoding = get_utf_len(ch);

  attr_t temp;
  wattr_get(w, &temp, NULL, NULL);

  wattrset(w, attr);
  mvwprintw(w, y, x, "%.*s", encoding, ch);

  wattrset(w, temp);
}

void paint_string(WINDOW *w, u32 y, u32 x, attr_t attr, size_t len,
                  const char *str) {
  for (size_t i = 0; str[i] && i < len; i++) {
    paint_char(w, y, x++, attr, &str[i]);
  }
}

void paint_utfstring(WINDOW *w, u32 y, u32 x, attr_t attr, size_t len,
                     const char *str) {
  size_t i = 0;
  size_t times = 0;

  while (str[i] && times < len) {
    u8 size = get_utf_len(&str[i]);
    UTFChar ch = get_utfchar(&str[i]);
    paint_utfchar(w, y, x++, attr, ch);
    i += size;
    times++;
  }
}

void paint_utfchar(WINDOW *w, u32 y, u32 x, attr_t attr, UTFChar ch) {
  attr_t temp;
  wattr_get(w, &temp, NULL, NULL);

  wattrset(w, attr);
  mvwprintw(w, y, x, "%.*s", ch.size, &ch.data[0]);

  wattrset(w, temp);
}
