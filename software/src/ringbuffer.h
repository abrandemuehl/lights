#pragma once
#include <stdint.h>

#define BUFFER_SIZE (256)
typedef struct {
  int start;
  int end;
  char data[BUFFER_SIZE];
} Buffer;

void bufferInit(Buffer *buf);

int bufferGet(Buffer *buf, uint8_t *data, int n);
int bufferPut(Buffer *buf, uint8_t *data, int n);

int bufferIndexInc(int idx);
