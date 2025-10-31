#ifndef __FREERTOS_TASK_H
#define __FREERTOS_TASK_H

/* freertos必须的的头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"

#include "lvgl.h" 
#include "lv_port_disp_template.h" 
#include "lv_port_indev_template.h"

void FreeRTOS_Start(void);

#define Task_Start_STACK    256
#define Task_Start_PRIORITY 3
void Task_Start(void);

//LED闪烁任务
#define LED_Toggle_STACK    256
#define LED_Toggle_PRIORITY 1
void FreeRTOS_LED_Toggle(void *pvParameters);

//串口输出任务
#define FreeRTOS_USART_STACK    256
#define FreeRTOS_USART_PRIORITY 1
void FreeRTOS_USART(void *pvParameters);

//lvgl任务
#define LV_DEMO_TASK_PRIO   3           /* 任务优先级 */ 
#define LV_DEMO_STK_SIZE    1024        /* 任务堆栈大小 */ 
void lv_demo_task(void *pvParameters);  /* 任务函数 */

void lvgl_demo(void); 
 
#endif
