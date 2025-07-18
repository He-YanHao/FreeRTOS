#ifndef __MYTASK_H
#define __MYTASK_H

/* freertos必须的的头文件 */
#include "FreeRTOS.h"
#include "task.h"

/* 硬件 */
#include "OLED.h"
#include "LED.h"
#include "Key.h"

#include "usart.h"

/* 启动任务的配置 */
#define LED0_Toggle_Task_STACK    128      //配置堆栈
#define LED0_Toggle_Task_PRIORITY 1        //配置优先级

#define LED1_Toggle_Task_STACK    128      //配置堆栈
#define LED1_Toggle_Task_PRIORITY 1        //配置优先级

#define Key_STACK    256      //配置堆栈
#define Key_PRIORITY 2        //配置优先级

void freertos_start(void);
void LED0_Toggle_Task(void *pvParameters); //声明任务函数
void LED1_Toggle_Task(void *pvParameters); //声明任务函数
void Key(void *pvParameters);

void ConfigureTimerForRuntimeStats(void);
uint32_t GetRuntimeCounterValue(void);

#endif
