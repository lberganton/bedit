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
  if (stack->top == node) {
    stack->top = stack->top->prev;
  }
  if (stack->nodes == 1) {
    stack->bot = NULL;
  }

  if (node->next) {
    node->next = node->prev;
  }
  if (node->prev) {
    node->prev = node->next;
  }

  free(node->state);
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

  new->next = NULL;
  new->prev = stack->top;

  if (stack->nodes == 0) {
    stack->bot = new;
  } else {
    stack->top->next = new;
  }
  stack->top = new;

  new->type = type;

  new->ptr_target = target;
  new->ptr_aux = aux;

  new->row = row;
  new->col = col;

  BufferNode *node = type == UNDO_ROW ? target : aux;
  new->state = (wchar_t *)malloc(sizeof(wchar_t) * node->string_length);
  memcpy(new->state, node->vector, node->string_length * sizeof(wchar_t));

  new->length = node->string_length;

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
  memcpy(node->vector, top->state, top->length * sizeof(wchar_t));
  node->string_length = top->length;

  UndoNode ret = *top;
  undo_node_free(stack, top);

  stack->nodes--;

  return ret;
}
