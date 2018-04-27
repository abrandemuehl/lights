#include "pattern.h"

void makeDefaultPattern(Bitmask *bitmask) {
  bitmaskWrite(bitmask[0], -2, 0, 0);
  bitmaskWrite(bitmask[0], -1, 0, 0);
  bitmaskWrite(bitmask[0],  0, 0, 1);
  bitmaskWrite(bitmask[0],  1, 0, 0);
  bitmaskWrite(bitmask[0],  2, 0, 0);

  bitmaskWrite(bitmask[1], -2, 0, 0);
  bitmaskWrite(bitmask[1], -1, 0, 1);
  bitmaskWrite(bitmask[1],  0, 0, 0);
  bitmaskWrite(bitmask[1],  1, 0, 1);
  bitmaskWrite(bitmask[1],  2, 0, 0);

  bitmaskWrite(bitmask[2], -2, 0, 1);
  bitmaskWrite(bitmask[2], -1, 0, 0);
  bitmaskWrite(bitmask[2],  0, 0, 0);
  bitmaskWrite(bitmask[2],  1, 0, 0);
  bitmaskWrite(bitmask[2],  2, 0, 1);
}
