#include "usart.h"

namespace stm32 {

Usart::Usart(USART usart) : usart_(usart) {}

void Usart::Start(uint32_t BaudRate) {
  // Enable RCC clock for GPIO
  RCC_AHBPeriphClockCmd(kGPIO_RCC[(int)usart_], ENABLE);

  // Enable USART clock
  switch (usart_) {
    case USART::Usart1:
    case USART::Usart6:
      RCC_APB2PeriphClockCmd(kUSART_RCC[(int)usart_], ENABLE);
      break;
    case USART::Usart2:
    case USART::Usart3:
    case USART::Usart4:
    case USART::Usart5:
      RCC_APB1PeriphClockCmd(kUSART_RCC[(int)usart_], ENABLE);
      break;
  }

  // Set alternate function
  GPIO_PinAFConfig(kGPIOBase[(int)usart_], kPinRx[(int)usart_],
                   kPinAF[(int)usart_]);
  GPIO_PinAFConfig(kGPIOBase[(int)usart_], kPinTx[(int)usart_],
                   kPinAF[(int)usart_]);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = kPinTx[(int)usart_];
  GPIO_Init(kGPIOBase[(int)usart_], &GPIO_InitStructure);

  // Don't pull the Rx Pin
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = kPinRx[(int)usart_];
  GPIO_Init(kGPIOBase[(int)usart_], &GPIO_InitStructure);

  // USARTx configured as follow:
  // - BaudRate = 115200 baud
  // - Word Length = 8 Bits
  // - Stop Bit = 1 Stop Bit
  // - Parity = No Parity
  // - Hardware flow control disabled (RTS and CTS signals)
  // - Receive and transmit enabled
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl =
    USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(kUsart[(int)usart_], &USART_InitStructure);
  USART_Cmd(kUsart[(int)usart_], ENABLE);
}
}  // namespace stm32
