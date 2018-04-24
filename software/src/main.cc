/* #include "stm32f0xx.h" */
#include <stm32f0xx.h>
#include <cstdint>
#include "console.h"
#include "timer.h"
#include "usart.h"

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
  console_init();
  // 1 ms period
  /* Timer1.Start(1000); */
  // PA8
  /* Timer1.EnableOutputChannel(1); */
  // Successful send

  /* Usart1.Start(); */


  /* Usart1.Start(); */

  /* while (1) { */
  /*   /1* puts("hi\n"); *1/ */
  /*   uint8_t data = 'h'; */
  /*   USART_SendData(USART1, (uint16_t)data); */
  /*   while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) { */
  /*   } */
  /*   data = '2'; */
  /*   Usart1.Write(&data, 1); */
  /* } */

  GPIO_InitTypeDef GPIO_InitStructure;
  // Show that I actually got somewhere
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* GPIO_InitTypeDef GPIO_InitStructure; */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  while (1) {
    GPIOB->BSRR = 0x0080;
    for (int i = 0; i < 100000; i++)
      ;
    GPIOB->BRR = 0x0080;
    for (int i = 0; i < 100000; i++)
      ;
    /* printf("\n\rUSART Printf Example: retarget the C library printf function
     * to the USART\n\r"); */

    // Loop until the end of transmission
    // The software must wait until TC=1. The TC flag remains cleared during all
    // data transfers and it is set by hardware at the last frame's end of
    // transmission
    /* while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) */
    /* {} */
  }

  while (1)
    ;
  return 0;
}

PUTCHAR_PROTOTYPE {
  USART_SendData(USART1, (uint16_t)'1');
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
  }
  USART_SendData(USART1, (uint16_t)ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
  }
  USART_SendData(USART1, (uint16_t)'2');
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
  }
  return ch;
}
