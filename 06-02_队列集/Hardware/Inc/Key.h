#ifndef __KEY_H
#define __KEY_H

#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"

#include "LED.h"

enum Key_num{KEY0 = 1,KEY1 = 2};

void Key_Init(void);
uint8_t Key_return(void);

void Key_EXTI_Init(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);

#endif
