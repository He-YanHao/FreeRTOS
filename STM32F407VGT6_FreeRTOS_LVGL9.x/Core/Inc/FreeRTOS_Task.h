#ifndef __FREERTOS_TASK_H
#define __FREERTOS_TASK_H

/* freertos必须的的头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"

void FreeRTOS_Start(void);

#define Task_Start_STACK    256
#define Task_Start_PRIORITY 3
void Task_Start(void);

#define LED_Toggle_STACK    256
#define LED_Toggle_PRIORITY 1
void FreeRTOS_LED_Toggle(void *pvParameters);

#define FreeRTOS_USART_STACK    256
#define FreeRTOS_USART_PRIORITY 1
void FreeRTOS_USART(void *pvParameters);

#endif
