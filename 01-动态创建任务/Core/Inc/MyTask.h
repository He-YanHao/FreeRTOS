#ifndef __MYTASK_H
#define __MYTASK_H

/* freertos必须的的头文件 */
#include "FreeRTOS.h"
#include "task.h"

/* 硬件 */
#include "OLED.h"
#include "LED.h"
#include "Key.h"

#define Task_Start_STACK 128
#define Task_Start_PRIORITY 3
void Task_Start(void);

#define LED0__Toggle_STACK 128
#define LED0__Toggle_PRIORITY 1
void LED0__Toggle(void *pvParameters);

#define LED1__Toggle_STACK 128
#define LED1__Toggle_PRIORITY 2
void LED1__Toggle(void *pvParameters);

#define Key_STACK 128
#define Key_PRIORITY 3
void Key(void *pvParameters);

void FreeRTOS_Start(void);

#endif
