#ifndef __BOARD_KEY_H
#define __BOARD_KEY_H

#include "stm32f4xx.h"
#include "Delay.h"

typedef enum{
    KEY_NO,KEY0,KEY1
}KEY_NUM;

void Key_Init(void);
void KeyEXTI_Init(void);
KEY_NUM Key_scan(void);

#endif
