#include "timer.h"

namespace stm32 {

constexpr TIM_TypeDef *kTIMBase[] = {TIM1};
constexpr GPIO_TypeDef *kChanGPIOBase[][] = {
    {GPIOA, GPIOA, GPIOA, GPIOA},  // Tim1
};

constexpr GPIO_TypeDef *kChanGPIOPin[][4] = {
  {GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11},
};

constexpr uint32_t kChanGPIOAF[][4] = {
  {GPIO_AF_2, GPIO_AF_2, GPIO_AF_2, GPIO_AF_2},
};
static uint32_t GPIOToRCC(GPIO_TypeDef *gpio) {
  switch((long)gpio) {
    case (long)GPIOA:
      return RCC_AHBPeriph_GPIOA;
    case (long)GPIOB:
      return RCC_AHBPeriph_GPIOB;
  }
  // TODO: Throw error
  return -1;
}

Timer::Timer(uint8_t timer, uint16_t period_us)
    : timer_(timer), period_(period_us) {

  uint16_t freq = 1000000 / period_us;
  uint16_t period = (SystemCoreClock / freq) - 1;

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = period;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
}

void Timer::EnableOutputChannel(uint8_t channel) {
  // Enable GPIO Bank for output pin
  RCC_AHBPeriphClockCmd(GPIOToRCC(kChanGPIOBase[timer_][channel]), ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = kChanGPIOPin[timer_][channel];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, kChanGPIOPin[timer_][channel], kChanGPIOAF[timer_][channel]);


  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init(kTIMBase[timer_], &TIM_OCInitStructure);

  TIM_Cmd(kTIMBase[timer_], ENABLE);
  TIM_CtrlPWMOutputs(kTIMBase[timer_], ENABLE);
}

}  // namespace stm32
