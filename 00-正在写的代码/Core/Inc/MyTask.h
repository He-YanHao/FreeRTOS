#ifndef __MYTASK_H
#define __MYTASK_H

/* freertos����ĵ�ͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"   //�������
#include "timers.h" //�����ʱ��
#include "list.h"   //ʵ���ں�����
#include "queue.h"  //���С��ź�����������
#include "event_groups.h" //�¼���־���ͷ�ļ�

/* Ӳ�� */
#include "OLED.h"
#include "LED.h"
#include "Key.h"

#include "usart.h"

/* ������������� */
#define Task0_STACK    512      //���ö�ջ
#define Task0_PRIORITY 4        //�������ȼ�

#define Task1_STACK    256      //���ö�ջ
#define Task1_PRIORITY 2        //�������ȼ�

#define Task2_STACK    256      //���ö�ջ
#define Task2_PRIORITY 2        //�������ȼ�

#define Debugging_STACK    512      //���ö�ջ
#define Debugging_PRIORITY 1        //�������ȼ�


void freertos_start(void);
void Task0(void *pvParameters); //����������
void Task1(void *pvParameters); //����������
void Task2(void *pvParameters); //����������
void Debugging(void *pvParameters);//���Ժ���

void ConfigureTimerForRuntimeStats(void);
uint32_t GetRuntimeCounterValue(void);

#endif
