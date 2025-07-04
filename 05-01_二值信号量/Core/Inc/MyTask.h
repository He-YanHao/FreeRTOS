#ifndef __MYTASK_H
#define __MYTASK_H

/* freertos必须的的头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h" //队列相关的头文件
#include "semphr.h" //信号量相关的头文件

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

#define Task2_STACK    256      //配置堆栈
#define Task2_PRIORITY 1        //配置优先级

void freertos_start(void);
void Task0(void *pvParameters); //声明任务函数
void Task1(void *pvParameters); //声明任务函数
void Task2(void *pvParameters); //声明任务函数

void ConfigureTimerForRuntimeStats(void);
uint32_t GetRuntimeCounterValue(void);

#endif
