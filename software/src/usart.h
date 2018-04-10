#pragma once
#include <cstdint>
#include "stm32f0xx.h"

namespace stm32 {
enum class USART : int {
  Usart1 = 0,
  Usart2 = 1,
  Usart3 = 2,
  Usart4 = 3,
  Usart5 = 4,
  Usart6 = 5,
};

constexpr USART_TypeDef *kUsart[] = {nullptr, USART1, USART2, USART3,
                                     USART4,  USART5, USART6};
constexpr GPIO_TypeDef *kGPIOBase[] = {nullptr, GPIOA, GPIOA, GPIOB,
                                       GPIOA,   GPIOB, GPIOA};
constexpr uint32_t kGPIO_RCC[] = {0,
                                  RCC_AHBPeriph_GPIOA,
                                  RCC_AHBPeriph_GPIOA,
                                  RCC_AHBPeriph_GPIOB,
                                  RCC_AHBPeriph_GPIOA,
                                  RCC_AHBPeriph_GPIOB,
                                  RCC_AHBPeriph_GPIOA};
constexpr uint32_t kUSART_RCC[] = {0,
                                   RCC_APB2Periph_USART1,
                                   RCC_APB1Periph_USART2,
                                   RCC_APB1Periph_USART3,
                                   RCC_APB1Periph_USART4,
                                   RCC_APB1Periph_USART5,
                                   RCC_APB2Periph_USART6};

constexpr uint16_t kPinRx[] = {0,          GPIO_Pin_10, GPIO_Pin_3, GPIO_Pin_11,
                               GPIO_Pin_1, GPIO_Pin_4,  GPIO_Pin_5};
constexpr uint16_t kPinTx[] = {0,          GPIO_Pin_9, GPIO_Pin_4, GPIO_Pin_10,
                               GPIO_Pin_0, GPIO_Pin_3, GPIO_Pin_4};
constexpr uint8_t kPinAF[] = {0,         GPIO_AF_0, GPIO_AF_0, GPIO_AF_0,
                              GPIO_AF_0, GPIO_AF_0, GPIO_AF_0};

class Usart {
 public:
  Usart(USART usart);
  Usart(){};

  void Start(uint32_t BaudRate);

 private:
  USART usart_;
};

}  // namespace stm32
