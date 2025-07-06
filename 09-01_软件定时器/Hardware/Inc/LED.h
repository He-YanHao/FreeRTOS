#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

void LED_Init(void);

void LED0_ON(void);
void LED0_OFF(void);
void LED0_Toggle(void);

void LED1_ON(void);
void LED1_OFF(void);
void LED1_Toggle(void);

void LED_ON(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void LED_OFF(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void LED_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
