#include "MyTask.h"

char pcWriteBuffer[512]; // �㹻��Ļ�����

/* ������������� */
TaskHandle_t LED0_Toggle_Task_handle;      //�������
StackType_t LED0_Toggle_Task_stack[LED0_Toggle_Task_STACK]; // ��̬���������ջ����������ʽ�洢
//typedef uint32_t StackType_t;  // ����32λ������
StaticTask_t LED0_Toggle_Task_tcb;                    // ��̬�����TCB�ṹ������
//������ľ����� ��������ϸ��

TaskHandle_t LED1_Toggle_Task_handle;      //�������
StackType_t LED1_Toggle_Task_stack[LED1_Toggle_Task_STACK]; // ��̬���������ջ����������ʽ�洢
//typedef uint32_t StackType_t;  // ����32λ������
StaticTask_t LED1_Toggle_Task_tcb;                    // ��̬�����TCB�ṹ������
//������ľ����� ��������ϸ��

TaskHandle_t Key_handle;      //�������
StackType_t Key_stack[Key_STACK]; // ��̬���������ջ����������ʽ�洢
//typedef uint32_t StackType_t;  // ����32λ������
StaticTask_t Key_tcb;                    // ��̬�����TCB�ṹ������
//������ľ����� ��������ϸ��


void freertos_start(void)
{
    //��������
 LED0_Toggle_Task_handle = xTaskCreateStatic(
                           LED0_Toggle_Task,          // ��������ַ
                           "LED1_Toggle_Task",        // ��������
                           LED0_Toggle_Task_STACK,    // ջ��С
                           NULL,                     // ����
                           LED0_Toggle_Task_PRIORITY, // ���ȼ�
                           LED0_Toggle_Task_stack,    // ����ջ����
                           &LED0_Toggle_Task_tcb );   // ������ƿ�
// LED1_Toggle_Task_handle = xTaskCreateStatic(
//                           LED1_Toggle_Task,          // ��������ַ
//                           "LED2_Toggle_Task",        // ��������
//                           LED1_Toggle_Task_STACK,    // ջ��С
//                           NULL,                     // ����
//                           LED1_Toggle_Task_PRIORITY, // ���ȼ�
//                           LED1_Toggle_Task_stack,    // ����ջ����
//                           &LED1_Toggle_Task_tcb );   // ������ƿ�
//    Key_handle      =      xTaskCreateStatic(
//                           Key,          // ��������ַ
//                           "Key",        // ��������
//                           Key_STACK,    // ջ��С
//                           NULL,              // ����
//                           Key_PRIORITY, // ���ȼ�
//                           Key_stack,    // ����ջ����
//                           &Key_tcb );   // ������ƿ�
    vTaskStartScheduler();
}

void LED0_Toggle_Task(void *pvParameters)
{
    taskENTER_CRITICAL();//����������Դ
    while (1)
    {
        //LED0_Toggle();
        vTaskDelay(500);
    }
}

void LED1_Toggle_Task(void *pvParameters)
{
    while (1)
    {
        LED1_Toggle();
        vTaskDelay(50);
    }
}

void Key(void *pvParameters)
{
    uint8_t keynum = 0;
    while(1)
    {
        keynum = Key_return();
        if(keynum == KEY0)
        {
            printf(" >>>>>> LED_OFF >>>>> \r\n");
            vTaskSuspend(LED1_Toggle_Task_handle);            
        }
        if(keynum == KEY1)
        {
            printf(" >>>>>> LED_ON >>>>> \r\n");
            vTaskResume(LED1_Toggle_Task_handle);
        }
        vTaskList(pcWriteBuffer);
        printf("Task List:\n%s\n", pcWriteBuffer);
        vTaskDelay(1000);
    }
}

/* ��̬������ʽ����Ҫ�ֶ�ָ��2�������������Դ */
/* ������������� */
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE]; // ��̬���������ջ����������ʽ�洢
StaticTask_t idle_task_tcb;                            // ��̬�����TCB�ṹ������
/* ��������������Դ */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &idle_task_tcb;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* �����ʱ����������� */
StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH]; // ��̬���������ջ����������ʽ�洢
StaticTask_t timer_task_tcb;                            // ��̬�����TCB�ṹ������
/* ���������ʱ���������Դ */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &timer_task_tcb;
    *ppxTimerTaskStackBuffer = timer_task_stack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
