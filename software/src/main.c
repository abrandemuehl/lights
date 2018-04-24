/* #include "stm32f0xx.h" */
#include <stm32f0xx.h>
#include <stdint.h>
#include "console.h"
#include "timer.h"
#include "usart.h"

void OutputMCO();
int main() {

  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* GPIO_InitTypeDef GPIO_InitStructure; */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  initUsarts();
  // console_init();
  
  for(int i=0; i < 10; i++) {
    usartWrite(USART1, "hi\n", 3);
  }

  while (1) {
    GPIOB->BSRR = 0x0080;
    for (int i = 0; i < 100000; i++)
      ;
    GPIOB->BRR = 0x0080;
    for (int i = 0; i < 100000; i++)
      ;
  }

  while (1)
    ;
  return 0;
}

void Default_Handler(void) {
  while (1);
}

void HardFault_Handler(void) {
  while (1) {
    GPIOB->BSRR = 0x0080;
    for (int i = 0; i < 1000000; i++)
      ;
    GPIOB->BRR = 0x0080;
    for (int i = 0; i < 1000000; i++)
      ;
  }
}


void OutputMCO() {
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Output clock on MCO pin ---------------------------------------------*/

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // pick one of the clocks to spew
  RCC_MCOConfig(RCC_MCOSource_SYSCLK, 1); // Put on MCO pin the: System clock selected
  // RCC_MCOConfig(RCC_MCOSource_HSE, 1); // Put on MCO pin the: freq. of external crystal
  //RCC_MCOConfig(RCC_MCOSource_PLLCLK_Div2); // Put on MCO pin the: System clock selected
}
