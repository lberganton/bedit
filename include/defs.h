/*
 * File: defs.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * Some basic definitions
 */
#pragma once

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define ERROR_WRONG_ARGS_NUM                                                   \
  "Erro: Forneça argumentos válidos.\n\n"                                    \
  "bedit [arquivo]"

#define ABORT(condition, description)                                          \
  if (condition) {                                                             \
    if (stdscr)                                                                \
      endwin();                                                                \
    fprintf(stderr, "bedit: %s\n", description);                             \
  }\
