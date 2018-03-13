#pragma once
#include "stm32f0xx.h"
#include <cstdint>

namespace stm32 {
enum class USART : int {
  Usart1=0,
  Usart2=1,
  Usart3=2,
  Usart4=3,
  Usart5=4,
  Usart6=5,
};

class Usart {
  public:

    Usart(USART usart);

    void Start(uint32_t BaudRate);

  private:
    USART usart_;
};

} // namespace stm32
