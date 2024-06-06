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

static void undo_node_free(UndoStack *stack, UndoNode *node) {
  if (node->next) {
    node->next = node->prev;
  }
  if (node->prev) {
    node->prev = node->next;
  }

  if (stack->top == node) {
    stack->top = stack->top->prev;
  }
  if (stack->bot == node) {
    stack->bot = stack->top->prev;
  }

  free(node->type == UNDO_ROW ? node->ptr_target : node->ptr_aux);

  free(node);
}

void undo_stack_free(UndoStack *stack) {
  while (stack->top) {
    undo_node_free(stack, stack->top);
  }

  free(stack);
}

void undo_node_push(UndoStack *stack, UndoType type, BufferNode *target,
                         BufferNode *aux, u32 row, u32 col) {
  UndoNode *new = (UndoNode *)malloc(sizeof(UndoNode));
  ASSERT(new == NULL,
         "Erro: Falha ao alocar memória para nó na pilha de 'desfazer'.");

  if (stack->nodes == 0) {
    stack->bot = new;
  }
  stack->top = new;

  new->type = type;

  new->ptr_target = target;
  new->ptr_aux = aux;

  new->row = row;
  new->col = col;

  new->next = NULL;
  new->prev = stack->top;

  BufferNode *node = type == UNDO_ROW ? target : aux; 
  new->state = (wchar_t *)malloc(sizeof(wchar_t) * node->buffer_len);
  memcpy(new->state, node->buffer, node->buffer_len * sizeof(wchar_t));

  new->length = node->buffer_len;

  time_t time_seed = time(NULL);
  new->time = *localtime(&time_seed);

  if (stack->nodes >= UNDO_MAX_STACK) {
    undo_node_free(stack, stack->bot);
  } else {
    stack->nodes++;
  }
}

UndoNode undo_node_pop(UndoStack *stack) {
  UndoNode *top = stack->top;

  BufferNode *node = top->type == UNDO_ROW ? top->ptr_target : top->ptr_aux; 
  memcpy(node->buffer, top->state, top->length * sizeof(wchar_t));
  node->buffer_len = top->length;

  UndoNode ret = *top;
  undo_node_free(stack, top);

  stack->nodes--;

  return ret;
}
