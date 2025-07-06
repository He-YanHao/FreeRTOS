#include "MyTask.h"

char pcWriteBuffer[512]; // �㹻��Ļ�����
UBaseType_t Debugging_Num = 0;

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

TaskHandle_t Debugging_handle;      //�������
StackType_t Debugging_stack[Debugging_STACK]; // ��̬���������ջ����������ʽ�洢
StaticTask_t Debugging_tcb;                    // ��̬�����TCB�ṹ������
//������ľ����� ��������ϸ��

void freertos_start(void)
{
    printf("����ʼ\r\n");
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
      Debugging_handle  =  xTaskCreateStatic(
                           Debugging,       // ��������ַ
                           "Debugging",         // ��������
                           Debugging_STACK,     // ջ��С
                           NULL,            // ����
                           Debugging_PRIORITY,  // ���ȼ�
                           Debugging_stack,     // ����ջ����
                           &Debugging_tcb );    // ������ƿ�
    vTaskDelete(NULL);//��ɾ δ���õ�������ʱ������ɾ
    vTaskStartScheduler();//�������������
}

//task0�Զ��������м�
void Task0(void *pvParameters)
{
    Debugging_Num = uxTaskGetStackHighWaterMark(NULL);
    printf("Task0��ջ��ˮλ���Ϊ��%lu\r\n",Debugging_Num);
    vTaskDelete(NULL);
}

//
void Task1(void *pvParameters)
{   
    while(1)
    {
        for(uint8_t i = 0; i < 20; i++)
        {
            
            vTaskDelay(50);
        }
        Debugging_Num = uxTaskGetStackHighWaterMark(NULL);
        printf("Task1��ջ��ˮλ���Ϊ��%lu\r\n",Debugging_Num);
    }
}

//
void Task2(void *pvParameters)
{
    while(1)
    {
        for(uint8_t i = 0; i < 20; i++)
        {
            
            vTaskDelay(50);
        }
        Debugging_Num = uxTaskGetStackHighWaterMark(NULL);
        printf("Task2��ջ��ˮλ���Ϊ��%lu\r\n",Debugging_Num);
    }
}

void Debugging(void *pvParameters)
{
    while(1)
    {

        Debugging_Num = uxTaskGetStackHighWaterMark(NULL);
        printf("Debugging��ջ��ˮλ���Ϊ��%lu\r\n",Debugging_Num);
        vTaskGetRunTimeStats(pcWriteBuffer);
        printf("%s\r\n",pcWriteBuffer);
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
