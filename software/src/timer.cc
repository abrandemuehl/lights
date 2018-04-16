#include "timer.h"

namespace stm32 {

Timer Timer1(1, TIM1, GPIOA, RCC_AHBPeriph_GPIOA, GPIO_Pin_8, GPIO_Pin_9,
             GPIO_Pin_10, GPIO_Pin_11, GPIO_AF_2);

Timer::Timer(int index, TIM_TypeDef *timer, GPIO_TypeDef *gpio,
             uint32_t gpio_rcc, uint16_t pin1, uint16_t pin2, uint16_t pin3,
             uint16_t pin4, uint8_t pin_af)
    : index_(index),
      timer_(timer),
      gpio_(gpio),
      gpio_rcc_(gpio_rcc),
      pin_af_(pin_af) {
  pins[0] = pin1;
  pins[1] = pin2;
  pins[2] = pin3;
  pins[3] = pin4;
}

void Timer::Start(uint16_t period_us) {
  period_ = period_us;

  uint16_t freq = 1000000 / period_us;
  uint16_t period = (SystemCoreClock / freq) - 1;

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = period;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(timer_, &TIM_TimeBaseStructure);
  TIM_Cmd(timer_, ENABLE);
  TIM_CtrlPWMOutputs(timer_, ENABLE);
}

void Timer::EnableOutputChannel(uint8_t channel) {
  if (channel >= 4) {
    // TODO: Throw error
    return;
  }
  TIM_Cmd(timer_, DISABLE);
  TIM_CtrlPWMOutputs(timer_, DISABLE);

  // Enable GPIO Bank for output pin
  RCC_AHBPeriphClockCmd(gpio_rcc_, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = pins[channel - 1];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, pins[channel - 1], pin_af_);

  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init(timer_, &TIM_OCInitStructure);

  TIM_Cmd(timer_, ENABLE);
  TIM_CtrlPWMOutputs(timer_, ENABLE);
}

}  // namespace stm32
