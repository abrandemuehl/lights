#include "console.h"
#include "usart.h"

void console_init() {
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  // Set alternate function
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);


  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Don't pull the Rx Pin
  /* GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; */
  /* GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; */
  /* GPIO_Init(GPIOA, &GPIO_InitStructure); */

  // USARTx configured as follow:
  // - BaudRate = 115200 baud
  // - Word Length = 8 Bits
  // - Stop Bit = 1 Stop Bit
  // - Parity = No Parity
  // - Hardware flow control disabled (RTS and CTS signals)
  // - Receive and transmit enabled
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
  NVIC_EnableIRQ(USART1_IRQn);
}


void printHex(uint32_t num) {
  PRINT("0x");

  // One hex digit is 4 bits
  uint8_t digit;
  uint8_t display;
  for(int i=0; i < 8; i++) {
    digit = ((num & 0xF0000000) >> 28) & 0x0F;
    if(digit < 10) {
      display = '0' + digit;
    } else {
      display = 'A' + (digit - 10);
    }
    usartWrite(USART1, &display, 1);
    num = num << 4;
  }
}

void printByte(uint8_t num) {
  PRINT("0x");

  // One hex digit is 4 bits
  uint8_t digit;
  uint8_t display;
  for(int i=0; i < 2; i++) {
    digit = ((num & 0xF0) >> 4) & 0x0F;
    if(digit < 10) {
      display = '0' + digit;
    } else {
      display = 'A' + (digit - 10);
    }
    usartWrite(USART1, &display, 1);
    num = num << 4;
  }
}
void printDec(int32_t num) {
  uint8_t digit;
  uint8_t display;
  if(num < 0) {
    PRINT("-");
    num *= -1;
  }
  do {
    digit = num % 10;
    display = '0' + digit;
    usartWrite(USART1, &display, 1);
    num = num / 10;
  } while(num > 0);
}

void printBin(uint32_t num) {
  PRINT("0b");
  
  for(int i=0; i < 32; i++) {
    uint8_t bit = (num & 0x80000000) != 0 ? '1' : '0';
    usartWrite(USART1, &bit, 1);
    num = num << 1;
  }
}


void printFloat(float num) {
  if(num < 0) {
    PRINT('-');
    num *= -1;
  }
  printDec((int32_t)num);

  num -= (int32_t)num;

  PRINT('.');

  // Print with three decimal places of accuracy
  num *= 1000;
  printDec((int32_t)num);
}
