#ifndef __MYTASK_H
#define __MYTASK_H

/* freertos����ĵ�ͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"

/* Ӳ�� */
#include "OLED.h"
#include "LED.h"
#include "Key.h"

#include "usart.h"

/* ������������� */
#define LED0_Toggle_Task_STACK    128      //���ö�ջ
#define LED0_Toggle_Task_PRIORITY 2        //�������ȼ�

#define LED1_Toggle_Task_STACK    128      //���ö�ջ
#define LED1_Toggle_Task_PRIORITY 2        //�������ȼ�

#define Key_STACK    256      //���ö�ջ
#define Key_PRIORITY 1        //�������ȼ�

void freertos_start(void);
void LED0_Toggle_Task(void *pvParameters); //����������
void LED1_Toggle_Task(void *pvParameters); //����������
void OLED_OUT(void *pvParameters);
void Key(void *pvParameters);

#endif
