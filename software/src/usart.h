#pragma once
#include <stm32f0xx.h>
#include <cstdint>
#include <utility>

namespace stm32 {

class Usart {
 public:
  Usart(){};
  Usart(int index, USART_TypeDef *usart, GPIO_TypeDef *gpio, uint32_t rcc,
        uint32_t gpio_rcc, uint16_t pin_rx, uint16_t pin_tx, uint8_t pin_af,
        uint8_t pin_rx_source, uint8_t pin_tx_source, IRQn_Type irq);

  void Start();

  void Read(uint8_t *data, int n);
  void Write(uint8_t *data, int n);

  std::pair<GPIO_TypeDef *, uint16_t> TxPin() {
    return std::make_pair(gpio_, pin_tx_);
  }
  std::pair<GPIO_TypeDef *, uint16_t> RxPin() {
    return std::make_pair(gpio_, pin_rx_);
  }

 private:
  int index_;
  USART_TypeDef *usart_;
  GPIO_TypeDef *gpio_;
  uint32_t rcc_;
  uint32_t gpio_rcc_;
  uint16_t pin_rx_;
  uint16_t pin_tx_;
  uint16_t pin_af_;
  uint8_t pin_rx_source_;
  uint8_t pin_tx_source_;
  IRQn_Type irq_;
};

extern Usart Usart1;
extern Usart Usart2;
extern Usart Usart3;
extern Usart Usart4;
extern Usart Usart5;
extern Usart Usart6;
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_6_IRQHandler(void);

}  // namespace stm32
