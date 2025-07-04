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
#define Task0_STACK    256      //���ö�ջ
#define Task0_PRIORITY 1        //�������ȼ�

#define Task1_STACK    256      //���ö�ջ
#define Task1_PRIORITY 1        //�������ȼ�

void freertos_start(void);
void Key_NotifyGive(void *pvParameters); //����������
void printf_Task1Notify(void *pvParameters); //����������

void ConfigureTimerForRuntimeStats(void);
uint32_t GetRuntimeCounterValue(void);

#endif
