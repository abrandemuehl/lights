#include "usart.h"

namespace stm32 {

Usart Usart1(1, USART1, GPIOA, RCC_AHBPeriph_GPIOA, RCC_APB2Periph_USART1, GPIO_Pin_10,
             GPIO_Pin_9, GPIO_AF_0);
Usart Usart2(2, USART2, GPIOA, RCC_AHBPeriph_GPIOA, RCC_APB1Periph_USART2, GPIO_Pin_3,
             GPIO_Pin_4, GPIO_AF_0);
Usart Usart3(3, USART3, GPIOB, RCC_AHBPeriph_GPIOB, RCC_APB1Periph_USART3, GPIO_Pin_11,
             GPIO_Pin_10, GPIO_AF_0);
Usart Usart4(4, USART4, GPIOA, RCC_AHBPeriph_GPIOA, RCC_APB1Periph_USART4, GPIO_Pin_1,
             GPIO_Pin_0, GPIO_AF_0);
Usart Usart5(5, USART5, GPIOB, RCC_AHBPeriph_GPIOB, RCC_APB1Periph_USART5, GPIO_Pin_4,
             GPIO_Pin_3, GPIO_AF_0);
Usart Usart6(6, USART6, GPIOA, RCC_AHBPeriph_GPIOA, RCC_APB2Periph_USART6, GPIO_Pin_5,
             GPIO_Pin_4, GPIO_AF_0);

constexpr int kBufferSize = 256;

uint8_t rxBuffer[6][kBufferSize];

int rxStart[6] = { 0 };
int rxEnd[6] = { 0 };

bool rxReceived[8] = { false };

void Usart::Start(uint32_t BaudRate) {
  // Enable RCC clock for GPIO
  RCC_AHBPeriphClockCmd(gpio_rcc_, ENABLE);

  // Enable USART clock
  switch (index_) {
    case 1:
    case 6:
      RCC_APB2PeriphClockCmd(rcc_, ENABLE);
      break;
    case 2:
    case 3:
    case 4:
    case 5:
      RCC_APB1PeriphClockCmd(rcc_, ENABLE);
      break;
  }

  // Set alternate function
  GPIO_PinAFConfig(gpio_, pin_rx_, pin_af_);
  GPIO_PinAFConfig(gpio_, pin_tx_, pin_af_);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = pin_tx_;
  GPIO_Init(gpio_, &GPIO_InitStructure);

  // Don't pull the Rx Pin
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = pin_rx_;
  GPIO_Init(gpio_, &GPIO_InitStructure);

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
  USART_Init(usart_, &USART_InitStructure);

  // Enable interrupt
  USART_Cmd(usart_, ENABLE);
  USART_DMACmd(usart_, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);

  // Enable DMA Controller
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);


}

void Usart::Read(uint8_t *data, int n) {
  for(int i=0; i < n; i++) {
    data[i] = (uint8_t)USART_ReceiveData(usart_);
  }
}

void Usart::Write(uint8_t *data, int n) {
  for(int i=0; i < n; i++) {
    USART_SendData(usart_, (uint16_t)data[i]);
  }
}


void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    rxBuffer[0][rxIndex[0]++] = USART_ReceiveData(USART1);

    if(rxIndex[0] == kBufferSize)
    {
      rxReceived[0] = true;
      rxIndex[0] = 0;
    }
  }
}

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    rxBuffer[1][rxIndex[1]++] = USART_ReceiveData(USART2);

    if(rxIndex[1] == kBufferSize)
    {
      rxReceived[1] = true;
      rxIndex[1] = 0;
    }
  }
}

/**
  * @brief  This function handles USARTy global interrupt request.
  * @param  None
  * @retval None
  */
void USART3_6_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    rxBuffer[2][rxIndex[2]++] = USART_ReceiveData(USART3);

    if(rxIndex[2] == kBufferSize)
    {
      rxReceived[2] = true;
      rxIndex[2] = 0;
    }
  }
  
  if(USART_GetITStatus(USART4, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    rxBuffer[3][rxIndex[3]++] = USART_ReceiveData(USART4);

    if(rxIndex[3] == kBufferSize)
    {
      rxReceived[3] = true;
      rxIndex[3] = 0;
    }
  }
  
  if(USART_GetITStatus(USART5, USART_IT_RXNE) != RESET)
  {

    /* Read one byte from the receive data register */
    rxBuffer[4][rxIndex[4]++] = USART_ReceiveData(USART5);

    if(rxIndex[4] == kBufferSize)
    {
      rxReceived[4] = true;
      rxIndex[4] = 0;
    }
  }
  
  if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
  {

    /* Read one byte from the receive data register */
    rxBuffer[5][rxIndex[5]++] = USART_ReceiveData(USART6);

    if(rxIndex[5] == kBufferSize)
    {
      rxReceived[5] = true;
      rxIndex[5] = 0;
    }
  }
}

}  // namespace stm32
