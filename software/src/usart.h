#pragma once
#include <stm32f0xx.h>
#include <stdint.h>


void initUsarts();

void usartWrite(USART_TypeDef *usart, uint8_t *data, unsigned int n);

int usartRead(USART_TypeDef *usart, uint8_t *data, unsigned int n);

int usartAvailable(USART_TypeDef *usart);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_6_IRQHandler(void);
