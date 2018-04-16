#include "stm32f0xx.h"
#include <cstdint>
#include <cstdio>
#include "usart.h"
#include "timer.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

extern uint32_t SystemCoreClock;

using namespace stm32;

int main() {

  Usart1.Start(115200);
  Usart3.Start(115200);
  Usart4.Start(115200);
  Usart5.Start(115200);
  Usart6.Start(115200);

  // 1 ms period
  Timer1.Start(1000);
  Timer1.EnableOutputChannel(1);

  /*
  while(1) {
    printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");

    // Loop until the end of transmission
    // The software must wait until TC=1. The TC flag remains cleared during all data
    // transfers and it is set by hardware at the last frame's end of transmission
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}

  }

  while(1);
  */
  return 0;
}



/*
PUTCHAR_PROTOTYPE
{
  USART_SendData(USART1, (uint8_t) ch);

  // Loop until transmit data register is empty
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {}

  return ch;
}
*/
