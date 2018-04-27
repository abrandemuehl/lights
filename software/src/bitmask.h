#pragma once
#include <stdint.h>

#define PATTERN_DIAMETER (8)
typedef uint8_t Bitmask[PATTERN_DIAMETER][PATTERN_DIAMETER/8];

uint8_t bitmaskGet(Bitmask mask, int x, int y);
void bitmaskWrite(Bitmask mask, int x, int y, int value);
int bitmaskSerialize(Bitmask mask, uint8_t *buf);

void bitmaskPrint(Bitmask mask);
