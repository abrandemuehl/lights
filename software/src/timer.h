#pragma once
#include <stm32f0xx.h>


void initTimer(uint32_t period_us);
void timerSetDuty(float duty);
