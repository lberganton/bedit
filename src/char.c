/*
 * File: char.c
 * Author: Lucas Berganton
 * Created: 05/18/2024
 */
#include "char.h"

u8 get_encoding(char *ch) {
  ABORT(ch == NULL, "Erro: Falha em descobrir o encoding de um caractere.");

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

UTFChar get_utfchar(char *in) {
    UTFChar ch;
    ch.size = get_encoding(in);

    for (u8 i = 0; i < ch.size; i++) {
      ch.data[i] = in[i];
    }

    return ch;
}
