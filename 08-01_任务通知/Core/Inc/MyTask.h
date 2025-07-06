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
#define Task0_STACK    256      //配置堆栈
#define Task0_PRIORITY 1        //配置优先级

#define Task1_STACK    256      //配置堆栈
#define Task1_PRIORITY 1        //配置优先级

void freertos_start(void);
void Key_NotifyGive(void *pvParameters); //声明任务函数
void printf_Task1Notify(void *pvParameters); //声明任务函数

void ConfigureTimerForRuntimeStats(void);
uint32_t GetRuntimeCounterValue(void);

#endif
