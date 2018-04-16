#include "protocol.h"
#include "gpio.h"


bool Neighbor::isControlChar(uint8_t data) {
  switch (data) {
    case DLE:
    case SOH:
    case ETX:
      return true;
  }
  return false;
}

Neighbor::Neighbor(stm32::Usart *usart) : usart_(usart) {}

bool Neighbor::Online() {
  std::pair<GPIO_TypeDef*, uint16_t> rxPin = usart_->RxPin();

  stm32::gpio pin(rxPin.first, rxPin.second);
  pin.Start(GPIO_Mode_IN, GPIO_PuPd_NOPULL);
  return pin.Read();
  // pin will destruct and restore the default configuration
}

bool Neighbor::ProcessBuffer() {
  static uint8_t packet[kMaxMessageSize];
  return false;
}

Comm::Comm(stm32::Usart *n, stm32::Usart *w, stm32::Usart *s, stm32::Usart *e)
    : n_(n), w_(w), s_(s), e_(e) {}
