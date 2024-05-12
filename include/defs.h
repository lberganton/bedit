/*
 * File: main.c
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

#define ABORT(condition, description)               \
  if (condition) {                                  \
    if (stdscr)                                     \
      endwin();                                     \
    fprintf(stderr, "Beditor: %s\n", description);  \
  }                                                 \
