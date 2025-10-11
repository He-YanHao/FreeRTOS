#ifndef __MYADC_H
#define __MYADC_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "board_BSP_USART.h"

void MyADC_Init(void);
uint16_t Get_ADC_Value(void);

#endif
