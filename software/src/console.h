#pragma once 

#include <stm32f0xx.h>
#include "usart.h"

void console_init();


void printDec(int32_t num);
void printHex(uint32_t num);
void printByte(uint8_t num);
void printBin(uint32_t num);
void printFloat(float num);

#define PRINT(string) \
  do { \
    usartWrite(USART1, (uint8_t*)string, sizeof(string)); \
  } while(0)
