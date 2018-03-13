#include "stm32f0xx.h"
#include <cstdint>
#include <cstdio>
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

  auto usart1 = Usart(USART::Usart1);
  auto usart3 = Usart(USART::Usart3);
  auto usart4 = Usart(USART::Usart4);
  auto usart5 = Usart(USART::Usart5);
  auto usart6 = Usart(USART::Usart6);

  usart1.Start(115200);
  usart3.Start(115200);
  usart4.Start(115200);
  usart5.Start(115200);
  usart6.Start(115200);

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
  // Enable GPIOA Bank
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  // Enable GPIO Pin 8 for timer
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  uint16_t freq = 1000;
  uint16_t period = (SystemCoreClock / freq ) - 1;
  /* Time Base configuration */
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = period;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  float duty = 0.25;
  uint16_t channelPulse = (uint16_t) ((uint32_t) (duty * (period - 1)));

  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = channelPulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);


  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
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
