#pragma once
#include <stdint.h>
#include <stm32f0xx.h>

namespace stm32 {

class gpio {
 public:
  gpio(GPIO_TypeDef *bank, uint16_t pin);
  ~gpio();

  void Start(GPIOMode_TypeDef mode, GPIOPuPd_TypeDef pull);

  bool Read();
  void Write(bool val);

 private:
  GPIO_TypeDef *bank_;
  uint16_t pin_;

  GPIOMode_TypeDef mode_;
};

}  // namespace stm32
