/* #include "stm32f0xx.h" */
#include <cstdint>
#include <cstdio>
/* #include "usart.h" */
/* #include "timer.h" */
#include <stm32f0xx_hal.h>
#include <Legacy/stm32_hal_legacy.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

extern uint32_t SystemCoreClock;

/* using namespace stm32; */

/* static Usart Usart1(1, USART1, GPIOA, RCC_APB2Periph_USART1, RCC_AHBPeriph_GPIOA, GPIO_Pin_10, */
/*     GPIO_Pin_9, GPIO_AF_1); */
int main() {
  HAL_Init();
  __USART1_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStructure;
  // TX Pin
  GPIO_InitStructure.Pin = GPIO_PIN_9;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Alternate = GPIO_AF1_USART1;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  // RX pin
  GPIO_InitStructure.Pin = GPIO_PIN_10;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  s_UARTHandle.Instance        = USART1;
  s_UARTHandle.Init.BaudRate   = 115200;
  s_UARTHandle.Init.WordLength = UART_WORDLENGTH_8B;
  s_UARTHandle.Init.StopBits   = UART_STOPBITS_1;
  s_UARTHandle.Init.Parity     = UART_PARITY_NONE;
  s_UARTHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  s_UARTHandle.Init.Mode       = UART_MODE_TX_RX;
    
  if (HAL_UART_Init(&s_UARTHandle) != HAL_OK) {
    while(1);
  }
  for (;;)
  {
    uint8_t buffer[4] = {'h', 'i', 0, 0};
    /* HAL_UART_Receive(&s_UARTHandle, buffer, sizeof(buffer), HAL_MAX_DELAY); */
    HAL_UART_Transmit(&s_UARTHandle, buffer, sizeof(buffer), HAL_MAX_DELAY);
  }
  // 1 ms period
  /* Timer1.Start(1000); */
  // PA8
  /* Timer1.EnableOutputChannel(1); */
  // Successful send
  uint8_t data = 'c';
  while(1) {
  }
  /* Usart1.Write(&data, 1); */
  /* while(1) { */
  /*   __WFI(); */
  /* } */


  while(1) {
    /* printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r"); */

    // Loop until the end of transmission
    // The software must wait until TC=1. The TC flag remains cleared during all data
    // transfers and it is set by hardware at the last frame's end of transmission
    /* while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) */
    /* {} */

  }

  while(1);
  return 0;
}



PUTCHAR_PROTOTYPE
{
  /* uint8_t data = (uint8_t)ch; */
  /* Usart1.Write(&data, 1); */
  /* USART_SendData(USART1, (uint16_t)ch); */
  /* while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {} */
  return ch;
}
