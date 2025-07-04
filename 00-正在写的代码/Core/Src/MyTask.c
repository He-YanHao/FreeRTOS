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

TaskHandle_t Task2_handle;      //�������
StackType_t Task2_stack[Task2_STACK]; // ��̬���������ջ����������ʽ�洢
StaticTask_t Task2_tcb;                    // ��̬�����TCB�ṹ������
//������ľ����� ��������ϸ��

/* ���о�� */
QueueHandle_t queue1 = NULL;

void freertos_start(void)
{
    //��������
      Task0_handle    =    xTaskCreateStatic(
                           Task0,// ��������ַ
                           "Task0",         // ��������
                           Task0_STACK,     // ջ��С
                           NULL,            // ����
                           Task0_PRIORITY,  // ���ȼ�
                           Task0_stack,     // ����ջ����
                           &Task0_tcb );    // ������ƿ�
      Task1_handle    =    xTaskCreateStatic(
                           Task1,           // ��������ַ
                           "Task1",         // ��������
                           Task1_STACK,     // ջ��С
                           NULL,            // ����
                           Task1_PRIORITY,  // ���ȼ�
                           Task1_stack,     // ����ջ����
                           &Task1_tcb );    // ������ƿ�
      Task2_handle    =    xTaskCreateStatic(
                           Task2,           // ��������ַ
                           "Task2",         // ��������
                           Task2_STACK,     // ջ��С
                           NULL,            // ����
                           Task2_PRIORITY,  // ���ȼ�
                           Task2_stack,     // ����ջ����
                           &Task2_tcb );    // ������ƿ�
    vTaskStartScheduler();
}

//task0�Զ���������
void Task0(void *pvParameters)
{
    queue1 = xQueueCreate(16,4);
    if (queue1 != NULL)
    {
        printf("queue1�����ɹ�\r\n");
    }
    else
    {
        printf("queue1����ʧ��\r\n");
    }
    vTaskDelete(NULL);
}

//����key0���task1���������
void Task1(void *pvParameters)
{
    char name[] = "zhen shi you yi si";
    uint8_t key = Key_return();
    while(1)
    {
        printf("task1��ʼ����...\r\n");
        if(key == KEY0)
        {
            if(xQueueSend(queue1, &name, 100) == pdPASS)
            {
                printf("����1�����ɹ�...\r\n");
            }
            else
            {
                printf("����1����ʧ��...\r\n");
            }
        }
        vTaskDelay(1000);
        printf("task1��������...\r\n");
    }
    
}

//����key1���task2��ȡ��������
void Task2(void *pvParameters)
{
    uint8_t shou[128] = {0};
    uint8_t key2 = 0;
    while(1)
    {
        if(key2 == KEY0)
        {
        xQueueReceive(queue1, &shou, 100);
        printf("%s/n",shou);
        }
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
