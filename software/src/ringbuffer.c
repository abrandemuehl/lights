#include "ringbuffer.h"
#include "console.h"

void bufferInit(Buffer *buf) {
  buf->start = 0;
  buf->end = 0;
}

int bufferGet(Buffer *buf, uint8_t *data, int n) {
  int read = 0;
  for(int i=0; i < n; i++) {
    if(buf->start == buf->end) {
      return read;
    }

    data[i] = buf->data[buf->start];
    buf->start = bufferIndexInc(buf->start);
    read++;
  }
  return read;
}
int bufferPut(Buffer *buf, uint8_t *data, int n) {
  int written = 0;
  for(int i=0; i < n; i++) {
    if(buf->start == bufferIndexInc(buf->end)) {
      PRINT("Put buffer full\n");
      return written;
    }

    buf->data[buf->end] = data[i];
    buf->end = bufferIndexInc(buf->end);
    written++;
  }
  return written;
}


int bufferSize(Buffer *buf) {
  if(buf->start <= buf->end) {
    return buf->end - buf->start;
  }
  return (BUFFER_SIZE - buf->start) + buf->end;
}

int bufferIndexInc(int idx) { return (idx + 1) % BUFFER_SIZE; }
