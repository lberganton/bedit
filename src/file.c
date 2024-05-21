/*
 * File: file.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "file.h"
#include "defs.h"
#include "unistd.h"
#include <string.h>

bool file_exist(const char *name) { return !access(name, F_OK); }

bool file_can_read(const char *name) { return !access(name, R_OK); }

bool file_can_write(const char *name) { return !access(name, W_OK); }

void file_get_input(const char *input, char *buff) { strcpy(buff, input); }

void file_get_name(const char *input, char *buff) {
  const char *slash = strrchr(input, '/');
  slash = slash == NULL ? input : slash;
  strncpy(buff, slash + 1, BUFF_STR);
}

void file_get_directory(const char *name, char *buff) {
  const char *slash = strrchr(name, '/');
  u32 index = (u32)(slash - name);
  strncpy(buff, name, index);
}

FileExtension file_get_extension(const char *name) {
  const char *dot = strrchr(name, '.');

  if (strcmp(dot, ".txt") == 0) {
    return EXTENSION_TXT;
  }
  if (strcmp(dot, ".c") == 0) {
    return EXTENSION_C;
  }
  if (strcmp(dot, ".cpp") == 0) {
    return EXTENSION_CPP;
  }

  return EXTENSION_UNKNOWN;
}

u32 file_get_size(const char *input) {
  FILE *f = fopen(input, "r");
  ABORT(f == NULL, "Erro: Falha ao determinar tamanho de arquivo.\n");

  fseek(f, 0, SEEK_END);
  return (u32)ftell(f);
}

void file_load(const char *input, Buffer *b) {
  // Open the file.
  FILE *f = fopen(input, "r");
  ABORT(f == NULL, "Erro: Falha ao abrir o arquivo.");

  // Return the function if the file is empty.
  if (fgetc(f) == EOF) {
    fclose(f);
    return;
  }

  // Moves the file cursor to start.
  fseek(f, 0, SEEK_SET);

  BufferNode *aux = b->begin;
  char buffer[BUFF_COL];
  u32 pos = 0;
  int ch;

  // Loops until the file reaches the end.
  while ((ch = fgetc(f)) != EOF) {
    ABORT(pos >= BUFF_COL, "Erro: Estouro no buffer de linha.");

    // If the character read isn't a new line, put it in the buffer and go to
    // the next iteration.
    if (ch != '\n') {
      buffer[pos++] = ch;

      // If the characters is a tab, fill the next indexes with spaces.
      if (ch == '\t') {
        for (u32 i = 1; i < TAB_SIZE; i++) {
          ABORT(pos >= BUFF_COL, "Erro: Estouro no buffer de linha.");
          buffer[pos++] = ' ';
        }
      }

      continue;
    }

    u32 i = 0;

    // Iterates over the buffer converting the default char type in UTFChar to
    // put it in the node (line) buffer.
    while (i < pos) {
      aux->buffer[aux->buffer_len++] = get_utfchar(&buffer[i]);
      i += get_utf_len(&buffer[i]);
    }

    // Inserts a new node at the end of the list.
    buffer_insert_end(b);
    aux = b->end;

    pos = 0;
  }

  fclose(f);
}
