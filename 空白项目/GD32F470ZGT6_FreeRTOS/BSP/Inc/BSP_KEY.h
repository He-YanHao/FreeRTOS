#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "gd32f4xx.h"
#include "Delay.h"

void BSP_KEY_Init(void);
uint8_t key_scan(void);

#endif
