#ifndef __FREERTOS_TASK_H
#define __FREERTOS_TASK_H

/* freertos必须的的头文件 */
#include "FreeRTOS.h" //
#include "task.h"     //任务管理
#include "timers.h"   //软件定时器
#include "queue.h"    //

#include "board.h"
#include "DS18B20.h"
#include "MyADC.h"
#include "OLED.h"

void FreeRTOS_Start(void);

#define Task_Start_STACK    256
#define Task_Start_PRIORITY 4
void Task_Start(void);

#define LED_Toggle_STACK    256
#define LED_Toggle_PRIORITY 1
void FreeRTOS_LED_Toggle(void *pvParameters);

void FreeRTOS_OUT(TimerHandle_t xTimer);

void vPreSleepProcessing( TickType_t xExpectedIdleTime );
void vPostSleepProcessing( TickType_t xExpectedIdleTime );

#endif
