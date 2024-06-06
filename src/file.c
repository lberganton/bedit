/*
 * File: file.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "file.h"
#include "defs.h"
#include "input.h"
#include "unistd.h"
#include <string.h>

bool file_exist(const char *name) { return !access(name, F_OK); }

bool file_can_read(const char *name) { return !access(name, R_OK); }

bool file_can_write(const char *name) { return !access(name, W_OK); }

void file_get_input(const char *input, char *buff) {
  strncpy(buff, input, BUFF_STR);
}

void file_get_name(const char *input, char *buff) {
  const char *slash = strrchr(input, '/');
  slash = slash == NULL ? input : slash + 1;
  strncpy(buff, slash, BUFF_STR);
}

void file_get_directory(const char *name, char *buff) {
  const char *slash = strrchr(name, '/');
  u32 index = slash == NULL ? 0 : (u32)(slash - name);
  strncpy(buff, name, index);
}

FileExtension file_get_extension(const char *name) {
  const char *dot = strrchr(name, '.');
  if (dot == NULL) {
    return EXTENSION_UNKNOWN;
  }

  if (strncmp(dot, ".txt", 5) == 0) {
    return EXTENSION_TXT;
  }
  if (strncmp(dot, ".c", 3) == 0) {
    return EXTENSION_C;
  }
  if (strncmp(dot, ".cpp", 5) == 0) {
    return EXTENSION_CPP;
  }

  return EXTENSION_UNKNOWN;
}

u32 file_get_size(const char *input) {
  FILE *f = fopen(input, "r");
  ASSERT(f == NULL, "Erro: Falha ao determinar tamanho de arquivo.\n");

  fseek(f, 0, SEEK_END);
  return (u32)ftell(f);
}

void file_read(const char *input, Buffer *b) {
  // Open the file.
  FILE *f = fopen(input, "r");
  ASSERT(f == NULL, "Erro: Falha ao abrir o arquivo.");

  wchar_t ch;

  // Return the function if the file is empty.
  if (fgetwc(f) == (wint_t)EOF) {
    fclose(f);
    return;
  }

  // Moves the file cursor to start.
  fseek(f, 0, SEEK_SET);

  BufferNode *aux = b->begin;
  wchar_t buffer[BUFF_COL];
  u32 pos = 0;

  // Loops until the file reaches the end.
  while ((ch = fgetwc(f)) != (wchar_t)EOF) {
    ASSERT(pos >= BUFF_COL, "Erro: Estouro no buffer de coluna.");

    // If the character read isn't a new line, put it in the buffer and go to
    // the next iteration.
    if (ch != '\n') {
      buffer[pos++] = ch;

      // If the characters is a tab, fill the next indexes with spaces.
      if (ch == '\t') {
        for (u32 i = 1; i < TAB_SIZE; i++) {
          ASSERT(pos >= BUFF_COL, "Erro: Estouro no buffer de coluna.");
          buffer[pos++] = ' ';
        }
      }

      continue;
    }

    ASSERT(b->nodes >= BUFF_ROW,
           "Erro: O arquivo tem um número de linhas maior que o permitido.");

    u32 i = 0;

    // Iterates over the buffer converting the default char type in UTFChar to
    // put it in the node (row) buffer.
    while (i < pos) {
      aux->buffer[aux->buffer_len++] = buffer[i++];
    }

    // Inserts a new node at the end of the list.
    aux = buffer_insert_next(b, b->end);

    pos = 0;
  }

  fclose(f);

  // Fill the last node with the last file row buffer.
  u32 i = 0;
  while (i < pos) {
    aux->buffer[aux->buffer_len++] = buffer[i++];
  }
}

bool file_write(const char *input, Buffer *b) {
  FILE *f = fopen(input, "w");
  if (f == NULL) {
    return false;
  }

  BufferNode *node = b->begin;

  while (node) {
    for (u32 i = 0; i < node->buffer_len; i++) {
      fputwc(node->buffer[i], f);
    }

    if (node->next) {
      fputwc('\n', f);
    }

    node = buffer_valid_next(node->next);
  }

  fclose(f);
  return true;
}
