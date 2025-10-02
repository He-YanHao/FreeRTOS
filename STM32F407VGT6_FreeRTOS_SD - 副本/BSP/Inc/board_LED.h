#ifndef __BOARD_LED_H
#define __BOARD_LED_H

#include "stm32f4xx.h"

void board_LED_Init(void);
void board_LED_ON(void);
void board_LED_OFF(void);
void board_LED_Toggle(void);

#endif
