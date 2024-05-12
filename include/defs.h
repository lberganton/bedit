/*
 * File: defs.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * Some basic definitions
 */
#pragma once

#include <ncurses.h>
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
  "Forneça argumentos válidos.\n"                                            \
  "bedit [arquivo]"

#define NO_NAME_FILE "[Sem nome]"

#define FILE_EXTENSION_UNKNOWN "Sem fmt"
#define FILE_EXTENSION_TXT     "Texto"
#define FILE_EXTENSION_C       "C"
#define FILE_EXTENSION_CPP     "C++"

#define ABORT(condition, description)                                          \
  if (condition) {                                                             \
    if (stdscr)                                                                \
      endwin();                                                                \
    fprintf(stderr, "bedit: %s\n", description);                               \
  }
