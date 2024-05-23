#include "undo.h"
#include <stdlib.h>
#include <string.h>

UndoStack *undo_stack_create(void) {
  UndoStack *new = (UndoStack *)malloc(sizeof(UndoStack));
  ABORT(new == NULL, "Erro: Falha ao alocar memória para pilha de 'desfazer'.");

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

UndoNode *undo_node_create(UndoStack *s, UndoType t, BufferNode *b) {
  UndoNode *new = (UndoNode *)malloc(sizeof(UndoNode));
  ABORT(new == NULL,
        "Erro: Falha ao alocar memória para nó da pilha de 'desfazer'.");
    
  new->prev = NULL;
  new->next = s->top;
  new->type = t;
  
  if (s->bot == NULL) {
    s->bot = new;
  }

  switch (t) {
  case UNDO_ROW:
    memcpy(new->state, b->buffer, sizeof(UTFChar) * b->buffer_len);
    break;
  case UNDO_NEW_ROW:
    new->state = NULL;
    break;
  case UNDO_REMOVE_ROW:
    new->state = NULL;
    b->activated = false;
    break;
  }

  return new;
}
