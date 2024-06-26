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

void undo_stack_free(UndoStack *stack) {
  while (stack->top) {
    undo_node_free(stack, stack->top);
  }

  free(stack);
}

void undo_node_free(UndoStack *stack, UndoNode *node) {
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

  free(node);
}

void undo_node_push(UndoStack *stack, BufferNode *current, u32 row, u32 col) {
  UndoNode *new = (UndoNode *)malloc(sizeof(UndoNode));
  ASSERT(new == NULL,
         "Erro: Falha ao alocar memória para nó na pilha de 'desfazer'.");

  new->next = NULL;
  new->prev = stack->top;

  if (stack->bot == NULL) {
    stack->bot = new;
  } else {
    stack->top->next = new;
  }

  stack->top = new;

  new->row = row;
  new->col = col;
  new->current = current;

  new->begin = NULL;
  new->end = NULL;

  time_t time_seed = time(NULL);
  new->time = *localtime(&time_seed);

  if (stack->nodes >= UNDO_MAX_STACK) {
    undo_node_free(stack, stack->bot);
  } else {
    stack->nodes++;
  }
}

void undo_node_insert(UndoStack *stack, UndoType type, BufferNode *target) {
  UndoChange *new = (UndoChange *)malloc(sizeof(UndoChange));
  ASSERT(
      new == NULL,
      "Erro: Falha ao alocar memória para nó na lista da pilha de 'desfazer'.");

  new->next = NULL;

  if (stack->top->begin == NULL) {
    stack->top->begin = new;
  } else {
    stack->top->end->next = new;
  }

  stack->top->end = new;

  new->type = type;
  new->target = target;

  if (type != UNDO_ROW) {
    new->state = NULL;
    new->length = 0;
    return;
  }

  new->state = (wchar_t *)malloc(target->string_length * sizeof(wchar_t));
  ASSERT(new->state == NULL, "Erro: Falha ao alocar memória para vetor na "
                             "lista da pilha de 'desfazer'");

  memcpy(new->state, target->vector, target->string_length * sizeof(wchar_t));
  new->length = target->string_length;
}

UndoNode undo_node_pop(UndoStack *stack, Buffer *buffer, u32 *rows) {
  ASSERT(stack == NULL || stack->nodes == 0,
         "Erro: Falha ao remover item da pilha de 'desfazer'.");

  UndoChange *change = stack->top->begin;

  while (change) {
    if (change->type == UNDO_NEW_ROW) {
      buffer_remove_node(buffer, change->target);
      (*rows)--;
    } else if (change->type == UNDO_REMOVE_ROW) {
      change->target->activated = true;
      (*rows)++;
    } else {
      bool increase = false;

      if (change->target->vector_length < change->length) {
        buffer_increase_vector(change->target,
                               change->length - change->target->string_length);

        increase = true;
      }

      memcpy(change->target->vector, change->state, change->length * sizeof(wchar_t));

      if (!increase && change->target->vector_length && change->length <= change->target->vector_length - 50) {
        buffer_decrease_vector(change->target,
                               change->target->string_length - change->length);
      }

      change->target->string_length = change->length;
      free(change->state);
    }

    UndoChange *temp = change->next;
    free(change);
    change = temp;
  }

  UndoNode node_to_return = *stack->top;
  undo_node_free(stack, stack->top);

  stack->nodes--;
  return node_to_return;
}
