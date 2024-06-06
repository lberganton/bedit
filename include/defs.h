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
#include <stdbool.h>
#include <stdlib.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define BUFF_ROW 10000
#define BUFF_COL 30
#define BUFF_STR 128
#define BUFF_TOK 16

#define STATUS_BAR_FILE_NAME_LEN 32

#define ERROR_WRONG_ARGS_NUM                                                   \
  "Forneça argumentos válidos.\n"                                            \
  "bedit [arquivo]"

#define NO_NAME_FILE "[Sem nome]"

#define FILE_EXTENSION_UNKNOWN "Sem fmt"
#define FILE_EXTENSION_TXT "Texto"
#define FILE_EXTENSION_C "C"
#define FILE_EXTENSION_CPP "C++"

typedef enum Mode { MODE_NORMAL, MODE_INSERT, MODE_COMMAND } Mode;

#define WRITE_LOG(message, ...)                                                \
  do {                                                                         \
    FILE *file = fopen("log/log", "a");                                        \
    if (file) {                                                                \
      fprintf(file, "<%s>: %d: " message "\n", __FILE__, __LINE__,             \
              ##__VA_ARGS__);                                                  \
      fclose(file);                                                            \
    }                                                                          \
  } while (false)

#define ASSERT(condition, description)                                         \
  do {                                                                         \
    if (condition) {                                                           \
      if (stdscr) {                                                            \
        endwin();                                                              \
      }                                                                        \
      fprintf(stderr, "bedit: %s\n", description);                             \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (false)
