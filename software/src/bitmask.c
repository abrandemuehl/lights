#include "bitmask.h"
#include <string.h>
#include "console.h"

uint8_t bitmaskGet(Bitmask mask, int x, int y) {
  int offset = (PATTERN_DIAMETER/2) + 1;
  x = x + offset;
  y = y + offset;
  int shift = x % 8;
  int idx = x / 8;
  return ((mask[y][idx]) >> shift) & 0x01;
}
void bitmaskWrite(Bitmask mask, int x, int y, int value) {
  int offset = (PATTERN_DIAMETER/2) + 1;
  x = x + offset;
  y = y + offset;
  int shift = x % 8;
  int idx = x / 8;
  if(value) {
    mask[y][idx] |= 1 << shift;
  } else {
    mask[y][idx] &= ~(1 << shift);
  }
}

int bitmaskSerialize(Bitmask mask, uint8_t *buf) {
  memcpy(buf, mask, sizeof(Bitmask));
  return sizeof(Bitmask);
}


void bitmaskPrint(Bitmask mask) {
  for(int i=0; i < PATTERN_DIAMETER; i++) {
    for(int j = 0; j < (PATTERN_DIAMETER /8); j++) {
      printByteBin(mask[i][j]);
      PRINT("\n");
    }
  }
}
