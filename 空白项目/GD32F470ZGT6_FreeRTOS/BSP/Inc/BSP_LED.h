#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "gd32f4xx.h"

void BSP_LED1_Init(void);
void BSP_LED2_Init(void);
void BSP_LED3_Init(void);
void BSP_LED4_Init(void);

void BSP_LED_ALL_Init(void);

void BSP_LED1_ON(void);
void BSP_LED1_OFF(void);
void BSP_LED1_TOGGLE(void);

void BSP_LED2_ON(void);
void BSP_LED2_OFF(void);
void BSP_LED2_TOGGLE(void);

void BSP_LED3_ON(void);
void BSP_LED3_OFF(void);
void BSP_LED3_TOGGLE(void);

void BSP_LED4_ON(void);
void BSP_LED4_OFF(void);
void BSP_LED4_TOGGLE(void);

void BSP_LED_ALL_ON(void);
void BSP_LED_ALL_OFF(void);
void BSP_LED_ALL_TOGGLE(void);

#endif
