#include "usart.h"
#include "ringbuffer.h"

#include "console.h"
namespace stm32 {


Usart Usart1;
Usart Usart2;
Usart Usart3;
Usart Usart4;
Usart Usart5;
Usart Usart6;

  /* Usart1 = Usart(1, USART1, GPIOA, RCC_APB2Periph_USART1, RCC_AHBPeriph_GPIOA, */
  /*     GPIO_Pin_10, GPIO_Pin_9, GPIO_AF_1, GPIO_PinSource10, GPIO_PinSource9, */
  /*     USART1_IRQn); */
  /* Usart2 = Usart(2, USART2, GPIOA, RCC_APB1Periph_USART2, RCC_AHBPeriph_GPIOA, */
  /*     GPIO_Pin_3, GPIO_Pin_4, GPIO_AF_0, GPIO_PinSource3, GPIO_PinSource4, */
  /*     USART2_IRQn); */
  /* Usart3 = Usart(3, USART3, GPIOB, RCC_APB1Periph_USART3, RCC_AHBPeriph_GPIOB, */
  /*     GPIO_Pin_11, GPIO_Pin_10, GPIO_AF_0, GPIO_PinSource11, */
  /*     GPIO_PinSource10, USART3_8_IRQn); */
  /* Usart4 = Usart(4, USART4, GPIOA, RCC_APB1Periph_USART4, RCC_AHBPeriph_GPIOA, */
  /*     GPIO_Pin_1, GPIO_Pin_0, GPIO_AF_0, GPIO_PinSource1, GPIO_PinSource0, */
  /*     USART3_8_IRQn); */
  /* Usart5 = Usart(5, USART5, GPIOB, RCC_APB1Periph_USART5, RCC_AHBPeriph_GPIOB, */
  /*     GPIO_Pin_4, GPIO_Pin_3, GPIO_AF_0, GPIO_PinSource4, GPIO_PinSource3, */
  /*     USART3_8_IRQn); */
  /* Usart6 = Usart(6, USART6, GPIOA, RCC_APB2Periph_USART6, RCC_AHBPeriph_GPIOA, */
  /*     GPIO_Pin_5, GPIO_Pin_4, GPIO_AF_0, GPIO_PinSource5, GPIO_PinSource4, */
  /*     USART3_8_IRQn); */

constexpr int kBufferSize = 256;

RingBuffer<uint8_t, kBufferSize> rxBuffer[6];

bool rxReceived[8] = {false};

Usart::Usart(int index, USART_TypeDef *usart, GPIO_TypeDef *gpio, uint32_t rcc,
             uint32_t gpio_rcc, uint16_t pin_rx, uint16_t pin_tx,
             uint8_t pin_af, uint8_t pin_rx_source, uint8_t pin_tx_source,
             IRQn_Type irq)
    : index_(index),
      usart_(usart),
      gpio_(gpio),
      rcc_(rcc),
      gpio_rcc_(gpio_rcc),
      pin_rx_(pin_rx),
      pin_tx_(pin_tx),
      pin_af_(pin_af),
      pin_rx_source_(pin_rx_source),
      pin_tx_source_(pin_tx_source),
      irq_(irq) {
        /* console_init(); */
        /* Write((uint8_t *)"Init usart", 14); */
        /* switch(index) { */
        /*   case 1: */
        /*     Write((uint8_t *)"1", 1); */
        /*     break; */
        /*   case 2: */
        /*     Write((uint8_t *)"2", 1); */
        /*     break; */
        /*   case 3: */
        /*     Write((uint8_t *)"3", 1); */
        /*     break; */
        /*   case 4: */
        /*     Write((uint8_t *)"4", 1); */
        /*     break; */
        /*   case 5: */
        /*     Write((uint8_t *)"5", 1); */
        /*     break; */
        /*   case 6: */
        /*     Write((uint8_t *)"6", 1); */
        /*     break; */
        /* } */
      }

void Usart::Start() {
  switch(gpio_rcc_) {
    case RCC_AHBPeriph_GPIOA:
      Write((uint8_t *)"good rcc_gpio\n", 14);
      break;
    default: 
      Write((uint8_t *)"bad rcc_gpio\n", 13);
      break;
  }

  switch(rcc_) {
    case RCC_APB2Periph_USART1:
      Write((uint8_t *)"good rcc\n", 10);
      break;
    default: 
      Write((uint8_t *)"bad rcc\n", 10);
      break;
  }

  if(usart_ != USART1) {
      Write((uint8_t *)"good usart\n", 11);
  } else {
      Write((uint8_t *)"bad usart\n", 10);
  }


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

  RCC_AHBPeriphClockCmd(gpio_rcc_, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  // Set alternate function
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);


  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
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

  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
  NVIC_EnableIRQ(USART1_IRQn);
}

void Usart::Read(uint8_t *data, int n) {
  for (int i = 0; i < n; i++) {
    data[i] = (uint8_t)USART_ReceiveData(usart_);
  }
}

void Usart::Write(uint8_t *data, int n) {
  for (int i = 0; i < n; i++) {
    USART_SendData(USART1, (uint16_t)data[i]);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
    }
  }
}

void USART1_IRQHandler(void) {
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    /* Read one byte from the receive data register */
    rxReceived[0] = rxBuffer[0].Put(USART_ReceiveData(USART1));
  }
  GPIOB->BSRR = 0x0080;
  for (int i = 0; i < 100000; i++)
    ;
  GPIOB->BRR = 0x0080;
  for (int i = 0; i < 100000; i++)
    ;
}

void USART2_IRQHandler(void) {
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
    /* Read one byte from the receive data register */
    rxReceived[1] = rxBuffer[1].Put(USART_ReceiveData(USART2));
  }
}

/**
 * @brief  This function handles USARTy global interrupt request.
 * @param  None
 * @retval None
 */
void USART3_6_IRQHandler(void) {
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
    /* Read one byte from the receive data register */
    rxReceived[2] = rxBuffer[2].Put(USART_ReceiveData(USART3));
  }

  if (USART_GetITStatus(USART4, USART_IT_RXNE) != RESET) {
    /* Read one byte from the receive data register */
    rxReceived[3] = rxBuffer[3].Put(USART_ReceiveData(USART4));
  }

  if (USART_GetITStatus(USART5, USART_IT_RXNE) != RESET) {
    /* Read one byte from the receive data register */
    rxReceived[4] = rxBuffer[4].Put(USART_ReceiveData(USART5));
  }

  if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) {
    /* Read one byte from the receive data register */
    rxReceived[5] = rxBuffer[5].Put(USART_ReceiveData(USART6));
  }
}

}  // namespace stm32
