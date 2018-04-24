#include "usart.h"
#include <stm32f0xx.h>
#include "ringbuffer.h"


Buffer usart1_buf;
Buffer usart3_buf;
Buffer usart4_buf;
Buffer usart5_buf;
Buffer usart6_buf;

void initUsarts() {

  bufferInit(&usart1_buf);
  bufferInit(&usart3_buf);
  bufferInit(&usart4_buf);
  bufferInit(&usart5_buf);
  bufferInit(&usart6_buf);

  // Enable RCC clock for GPIO
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  // Enable USART clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART4, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART5, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

  // Set alternate function
  // Usart 1
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

  // Usart 3
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_0);

  // Usart 4
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_0);

  // Usart 5
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_0);

  // Usart 6
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

  // Usart 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Usart 3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Usart 4
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Usart 5
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Usart 6
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Don't pull the Rx Pin
  /* GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; */
  /* GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; */
  /* GPIO_Init(GPIOA, &GPIO_InitStructure); */

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


  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART3_8_IRQn;
  NVIC_Init(&NVIC_InitStructure);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);   
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);   
  USART_ITConfig(USART4, USART_IT_RXNE, ENABLE);   
  USART_ITConfig(USART5, USART_IT_RXNE, ENABLE);   
  USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);   

  USART_Init(USART1, &USART_InitStructure);
  USART_Init(USART3, &USART_InitStructure);
  USART_Init(USART4, &USART_InitStructure);
  USART_Init(USART5, &USART_InitStructure);
  USART_Init(USART6, &USART_InitStructure);

  USART_Cmd(USART1, ENABLE);
  USART_Cmd(USART3, ENABLE);
  USART_Cmd(USART4, ENABLE);
  USART_Cmd(USART5, ENABLE);
  USART_Cmd(USART6, ENABLE);
  // NVIC_EnableIRQ(USART1_IRQn);
  // NVIC_EnableIRQ(USART3_8_IRQn);
}

void usartWrite(USART_TypeDef *usart, uint8_t *data, unsigned int n) {
  for(unsigned int i=0; i < n; i++) {
    USART_SendData(USART1, (uint16_t)data[i]);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  }
}

int usartRead(USART_TypeDef *usart, uint8_t *data, unsigned int n) {
  if(usart == USART1) {
    return bufferGet(&usart1_buf, data, n);
  } else if(usart == USART3) {
    return bufferGet(&usart3_buf, data, n);
  } else if(usart == USART4) {
    return bufferGet(&usart4_buf, data, n);
  } else if(usart == USART5) {
    return bufferGet(&usart5_buf, data, n);
  } else if(usart == USART6) {
    return bufferGet(&usart6_buf, data, n);
  }
  return -1;
}


void USART1_IRQHandler(void) {
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    uint8_t ch = USART_ReceiveData(USART1);
    bufferPut(&usart1_buf, &ch, 1);
  }
}

/**
 * @brief  This function handles USARTy global interrupt request.
 * @param  None
 * @retval None
 */
void USART3_6_IRQHandler(void) {
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
    uint8_t ch = USART_ReceiveData(USART3);
    bufferPut(&usart3_buf, &ch, 1);
  }

  if (USART_GetITStatus(USART4, USART_IT_RXNE) != RESET) {
    uint8_t ch = USART_ReceiveData(USART4);
    bufferPut(&usart4_buf, &ch, 1);
  }

  if (USART_GetITStatus(USART5, USART_IT_RXNE) != RESET) {
    uint8_t ch = USART_ReceiveData(USART5);
    bufferPut(&usart5_buf, &ch, 1);
  }

  if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) {
    uint8_t ch = USART_ReceiveData(USART6);
    bufferPut(&usart6_buf, &ch, 1);
  }
}

