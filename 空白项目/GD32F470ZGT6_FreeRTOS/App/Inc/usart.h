#ifndef __USART_H
#define __USART_H

#include "gd32f4xx.h"

void MyUSART_Init(void);
void USART0_DATA(uint32_t DATA);
void USART0_STR(uint32_t* str);

#endif
