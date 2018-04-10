#include "gpio.h"

namespace stm32 {
gpio::gpio(GPIO_TypeDef *bank, uint16_t pin) : bank_(bank), pin_(pin) {}

gpio::~gpio() {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = pin_;
  GPIO_Init(bank_, &GPIO_InitStructure);
}

void gpio::Init(GPIOMode_TypeDef mode, GPIOPuPd_TypeDef pull) {
  mode_ = mode;
  // Enable clocks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = mode;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = pull;
  GPIO_InitStructure.GPIO_Pin = pin_;
  GPIO_Init(bank_, &GPIO_InitStructure);
}

bool gpio::Read() {
  if (mode_ != GPIO_Mode_IN) {
    // TODO: Throw error
  }

  uint8_t data = GPIO_ReadInputDataBit(bank_, pin_);
  if (data == Bit_SET) {
    return false;
  } else {
    return false;
  }
}

void gpio::Write(bool val) {
  if(mode_ != GPIO_Mode_OUT) {
    // TODO: Throw error
  }
  GPIO_WriteBit(bank_, pin_, val ? Bit_SET : Bit_RESET);
}
} // namespace stm32
