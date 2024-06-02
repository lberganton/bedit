#include "undo.h"
#include <stdlib.h>
#include <string.h>

UndoStack *undo_stack_create(void) {
  UndoStack *new = (UndoStack *)malloc(sizeof(UndoStack));
  ASSERT(new == NULL,
         "Erro: Falha ao alocar memória para pilha de 'desfazer'.");

  new->top = NULL;
  new->bot = NULL;
  new->dirty = false;
  new->nodes = 0;

  return new;
}

void undo_stack_free(UndoStack *s) {
  UndoNode *node = s->top;

  while (node) {
    UndoNode *temp = node->next;
    free(node);
    node = temp;
  }

  free(s);
}

UndoNode *undo_node_push(UndoStack *s, UndoType t, BufferNode *b, u32 row,
                         u32 col) {
  UndoNode *new = (UndoNode *)malloc(sizeof(UndoNode));
  ASSERT(new == NULL,
         "Erro: Falha ao alocar memória para nó da pilha de 'desfazer'.");
  
  time_t seed = time(NULL);
  new->time = *localtime(&seed);

  new->prev = NULL;
  new->next = s->top;
  new->type = t;
  new->ptr = b;
  new->row = row;
  new->col = col;

  s->top = new;

  if (s->bot == NULL) {
    s->bot = new;
  }

  switch (t) {
  case UNDO_ROW:
    new->state = (wchar_t *)malloc(sizeof(wchar_t) * b->buffer_len);
    memcpy(new->state, b->buffer, sizeof(wchar_t) * b->buffer_len);
    new->length = b->buffer_len;
    break;
  case UNDO_REMOVE_ROW:
    b->activated = false;
    break;
  }

  s->nodes++;
  return new;
}

void undo_bottom_free(UndoStack *s) {
  if (s->nodes == 0) {
    s->bot->prev = NULL;
  }

  UndoNode *temp = s->bot->prev;
  free(s->bot);

  s->bot = temp;
}

UndoNode *undo_node_pop(UndoStack *s) {
  UndoNode *node = s->top;

  if (s->nodes == 1) {
    s->bot = NULL;
  }

  s->top = s->top->next;
  s->nodes--;

  return node;
}
