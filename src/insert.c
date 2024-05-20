#include "buffer.h"
#include <string.h>

bool insert_char_at(UTFChar ch, u32 index, BufferNode *n) {
  if (n->buffer_len == BUFF_SIZE) {
    return false;
  }

  memcpy(&n->buffer[index + 1], &n->buffer[index],
         (n->buffer_len - index) * sizeof(UTFChar));
  
  n->buffer[index] = ch;
  n->buffer_len++;
}
