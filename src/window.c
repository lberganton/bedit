/*
 * File: window.c
 * Author: Lucas Berganton
 * Created: 05/11/2024
 */
#include "window.h"
#include "defs.h"
#include <stdlib.h>

Windows *window_init(void) {
  Windows *new = (Windows *)malloc(sizeof(Windows));
  ABORT(new == NULL, "Erro: Falha ao alocar memória para janelas.");
  return new;
}
