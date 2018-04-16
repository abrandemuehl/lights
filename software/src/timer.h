#pragma once
#include "stm32f0xx.h"

namespace stm32 {

class Timer {
 public:
  Timer(int index, TIM_TypeDef *timer, GPIO_TypeDef *gpio, uint32_t gpio_rcc, uint16_t pin1,
        uint16_t pin2, uint16_t pin3, uint16_t pin4, uint8_t pin_af);
  void Start(uint16_t period_us);
  void EnableOutputChannel(uint8_t channel);

  void SetPeriod(uint16_t us);
  void SetDuty(float duty);
  void SetPulseWidth(uint16_t us);

 private:
  int index_;
  TIM_TypeDef *timer_;
  GPIO_TypeDef *gpio_;
  uint32_t gpio_rcc_;

  uint16_t pins[4];

  uint8_t pin_af_;

  uint16_t period_;
  uint16_t channels_[4];
};
extern Timer Timer1;

}  // namespace stm32
