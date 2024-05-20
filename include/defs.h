/*
 * File: defs.h
 * Author: Lucas Berganton
 * Created: 05/11/2024
 *
 * Some basic definitions
 */
#pragma once

#include <inttypes.h>
#include <ncurses.h>
#include <stdlib.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define BUFF_SIZE 1024
#define TAB_SIZE 4

#define STATUS_BAR_FILE_NAME_LEN 32

#define ERROR_WRONG_ARGS_NUM                                                   \
  "Forneça argumentos válidos.\n"                                            \
  "bedit [arquivo]"

#define NO_NAME_FILE "[Sem nome]"

#define FILE_EXTENSION_UNKNOWN "Sem fmt"
#define FILE_EXTENSION_TXT "Texto"
#define FILE_EXTENSION_C "C"
#define FILE_EXTENSION_CPP "C++"

typedef enum Mode {
  MODE_NORMAL,
  MODE_INSERT,
  MODE_COMMAND
} Mode;

#define ABORT(condition, description)                                          \
  if (condition) {                                                             \
    if (stdscr)                                                                \
      endwin();                                                                \
    fprintf(stderr, "bedit: %s\n", description);                               \
    exit(1);                                                                   \
  }
