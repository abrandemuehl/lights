.syntax unified
.cpu cortex-m0
.fpu vfp
.thumb



.section .text
.weak Reset_Handler // Reset handler should be global

Reset_Handler:
	// Set initial stack pointer 
	ldr   r0, =_estack
	mov   sp, r0

	wfi
	bl SystemInit
	bl main // Jump to main
	b infiniteloop // If main returns, infinite loop

.weak NMI_Handler
.weak HardFault_Handler
.weak SVC_Handler
.weak PendSV_Handler
.weak SysTick_Handler
.weak WWDG_IRQHandler
.weak RTC_IRQHandler
.weak FLASH_IRQHandler
.weak RCC_IRQHandler
.weak EXTI0_1_IRQHandler
.weak EXTI2_3_IRQHandler
.weak EXTI4_15_IRQHandler
.weak DMA1_Channel1_IRQHandler
.weak DMA1_Channel2_3_IRQHandler
.weak DMA1_Channel4_5_IRQHandler
.weak ADC1_IRQHandler
.weak TIM1_BRK_UP_TRG_COM_IRQHandler
.weak TIM1_CC_IRQHandler
.weak TIM3_IRQHandler
.weak TIM6_IRQHandler
.weak TIM14_IRQHandler
.weak TIM15_IRQHandler
.weak TIM16_IRQHandler
.weak TIM17_IRQHandler
.weak I2C1_IRQHandler
.weak I2C2_IRQHandler
.weak SPI1_IRQHandler
.weak SPI2_IRQHandler
.weak USART1_IRQHandler
.weak USART2_IRQHandler
.weak USART3_6_IRQHandler
.weak _exit
_exit:
.weak __end__
__end__:
infiniteloop:
	wfi
	b infiniteloop


.section .int_vector_table, "a", %progbits
int_vector_table:
	.word     _estack // Specify the end of the stack for the processor's initial SP
	// Interrupt Vector entries must have an LSB of 1 to indicate the handler is thumb code
	.word     Reset_Handler+1                  // Reset Handler
	.word     NMI_Handler+1                    // NMI Handler
	.word     HardFault_Handler+1              // Hard Fault Handler
	.word     SVC_Handler+1                    // SVCall Handler
	.word     PendSV_Handler+1                 // PendSV Handler
	.word     SysTick_Handler+1                // SysTick Handler

	// Interrupts
	.word     WWDG_IRQHandler+1                // Window Watchdog
	.word     0                                // Reserved
	.word     RTC_IRQHandler+1                 // RTC through EXTI Line
	.word     FLASH_IRQHandler+1               // FLASH
	.word     RCC_IRQHandler+1                 // RCC
	.word     EXTI0_1_IRQHandler+1             // EXTI Line 0 and 1
	.word     EXTI2_3_IRQHandler+1             // EXTI Line 2 and 3
	.word     EXTI4_15_IRQHandler+1            // EXTI Line 4 to 15
	.word     0                                // Reserved
	.word     DMA1_Channel1_IRQHandler+1       // DMA1 Channel 1
	.word     DMA1_Channel2_3_IRQHandler+1     // DMA1 Channel 2 and Channel 3
	.word     DMA1_Channel4_5_IRQHandler+1     // DMA1 Channel 4 and Channel 5
	.word     ADC1_IRQHandler+1                // ADC1
	.word     TIM1_BRK_UP_TRG_COM_IRQHandler+1 // TIM1 Break, Update, Trigger and Commutation
	.word     TIM1_CC_IRQHandler+1             // TIM1 Capture Compare
	.word     0                                // Reserved
	.word     TIM3_IRQHandler+1                // TIM3
	.word     TIM6_IRQHandler+1                // TIM6
	.word     0							       // Reserved
	.word     0							       // Reserved
	.word     TIM14_IRQHandler+1               // TIM14
	.word     TIM15_IRQHandler+1               // TIM15
	.word     TIM16_IRQHandler+1               // TIM16
	.word     TIM17_IRQHandler+1               // TIM17
	.word     I2C1_IRQHandler+1                // I2C1
	.word     I2C2_IRQHandler+1                // I2C2
	.word     SPI1_IRQHandler+1                // SPI1
	.word     SPI2_IRQHandler+1                // SPI2
	.word     USART1_IRQHandler+1              // USART1
	.word     USART2_IRQHandler+1              // USART2
	.word     USART3_6_IRQHandler+1            // USART3, USART4, USART5, USART6
