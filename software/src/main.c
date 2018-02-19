#include <stdint.h>
#include "stm32f0xx.h"
#include <stdio.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

static void USART_Config(void);

int main() {

  USART_Config();

  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");

  /* Loop until the end of transmission */
  /* The software must wait until TC=1. The TC flag remains cleared during all data
     transfers and it is set by hardware at the last frame's end of transmission*/
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  while(1);

  /*
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  while(1) {

    // Set PA4
    GPIOA->BSRR = 0x000F << 2;
    // Reset PA4
    GPIOA->BRR = 0x000F << 2;
  }
  return 0;
  */
}

/**
  * @brief Configure the USART Device
  * @param  None
  * @retval None
  */
static void USART_Config(void)
{ 
  USART_InitTypeDef USART_InitStructure;
  
  /* USARTx configured as follow:
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - Stop Bit = 1 Stop Bit
  - Parity = No Parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Enable USART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  

  /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(GPIOA, GPIO_Pin_9, GPIO_AF_1);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(GPIOA, GPIO_Pin_10, GPIO_AF_1);
  
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(USART1, &USART_InitStructure);
    
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {}

  return ch;
}
