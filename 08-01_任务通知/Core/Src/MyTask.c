#include "MyTask.h"

char pcWriteBuffer[512]; // �㹻��Ļ�����

/* ������������� */
TaskHandle_t Task0_handle;      //�������
StackType_t Task0_stack[Task0_STACK]; // ��̬���������ջ����������ʽ�洢
StaticTask_t Task0_tcb;                    // ��̬�����TCB�ṹ������
//������ľ����� ��������ϸ��

TaskHandle_t Task1_handle;      //�������
StackType_t Task1_stack[Task1_STACK]; // ��̬���������ջ����������ʽ�洢
StaticTask_t Task1_tcb;                    // ��̬�����TCB�ṹ������
//������ľ����� ��������ϸ��


void freertos_start(void)
{
    //��������
      Task0_handle    =    xTaskCreateStatic(
                           Key_NotifyGive,// ��������ַ
                           "Task0",         // ��������
                           Task0_STACK,     // ջ��С
                           NULL,            // ����
                           Task0_PRIORITY,  // ���ȼ�
                           Task0_stack,     // ����ջ����
                           &Task0_tcb );    // ������ƿ�
      Task1_handle    =    xTaskCreateStatic(
                           printf_Task1Notify,          // ��������ַ
                           "Task1",        // ��������
                           Task1_STACK,    // ջ��С
                           NULL,                     // ����
                           Task1_PRIORITY, // ���ȼ�
                           Task1_stack,    // ����ջ����
                           &Task1_tcb );   // ������ƿ�
    vTaskStartScheduler();
}

//����key1���task1��������1
void Key_NotifyGive(void *pvParameters)
{
    uint8_t keynum = 0;
    while(1)
    {
        
        keynum = Key_return();
        if(keynum == KEY1)
        {
            printf("���°���\r\n");
            xTaskNotifyGive(Task1_handle);
        }
    }
    
}

//ÿ1�����һ��task1������
void printf_Task1Notify(void *pvParameters)
{
    uint32_t handle = 0;
    while(1)
    {
        printf("task1����...\r\n");
        handle = ulTaskNotifyValueClear(Task1_handle,0);
        printf("%d\r\n",handle);
        //printf("%s\r\n",pcWriteBuffer);
        vTaskDelay(1000);
    }
    
}


// ʹ�� DWT ���ڼ����� (Cortex-M3/M4)
void ConfigureTimerForRuntimeStats(void) {
    /* ���� DWT ������ */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t GetRuntimeCounterValue(void) {
    /* ���� CPU ���ڼ��� */
    return DWT->CYCCNT;
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
