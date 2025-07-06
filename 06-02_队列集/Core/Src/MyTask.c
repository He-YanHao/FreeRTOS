#include "MyTask.h"

//char pcWriteBuffer[512]; // �㹻��Ļ�����

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
QueueHandle_t queue2 = NULL;
QueueSetHandle_t queue0 = NULL;

//������
char name[] = "123456789";
//char *name = "123456789";

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
    vTaskStartScheduler();
    vTaskDelete(NULL);
}

//task0�Զ��������м�
void Task0(void *pvParameters)
{
    uint8_t re = 0;

    //������Ա1
    queue1 = xQueueCreate(2,2);
    //�жϳ�Ա2
    if(queue1 != NULL)
    {
        printf("queue1 is OK\r\n");
    }
    else
    {
        printf("\r\n");
    }

    //������Ա2
    queue2 = xQueueCreate(1,1);
    //�жϳ�Ա2
    if(queue2 != NULL)
    {
        printf("queue2 is OK\r\n");
    }
    else
    {
        printf("\r\n");
    }

    queue0 = xQueueCreateSet(2);
    //�������м�queue0
    if(queue0 != NULL)
    {
        printf("���м������ɹ�\r\n");
    }
    else
    {
        printf("���м�����ʧ��\r\n");
    }

    //��ӳ�Ա1
    re = xQueueAddToSet(queue1,queue0);
    if(re == pdPASS)
    {
        printf("��Ա1�ɹ���ӵ�������\r\n");
    }
    else
    {
        printf("��Ա1��ӵ�������ʧ��\r\n");
    }

    //��ӳ�Ա2
    re = 0;
    re = xQueueAddToSet(queue2,queue0);
    if(re == pdPASS)
    {
        printf("��Ա2�ɹ���ӵ�������\r\n");
    }
    else
    {
        printf("��Ա2��ӵ�������ʧ��\r\n");
    }

    vTaskDelete(NULL);
}

//����key0���task1��������� ����key1���task2���������
void Task1(void *pvParameters)
{
    uint8_t key = 0;
    uint8_t queue1_num = 1;
    uint8_t queue2_num = 2;    
    while(1)
    {
        key = Key_return();
        if(key == KEY0)
        {
            xQueueSendToBack(queue1,&queue1_num,0);
            printf("����1���...\r\n");
        }
        else if(key == KEY1)
        {
            xQueueSendToBack(queue2,&queue2_num,0);
            printf("����2���...\r\n");
        }
        vTaskDelay(50);
    }
}

//������м�
void Task2(void *pvParameters)
{
    QueueHandle_t Task2queue = NULL;
    uint8_t num = 0;
    while(1)
    {
        num = 0;
        Task2queue = xQueueSelectFromSet(queue0,0);
        if(Task2queue == queue1)
        {
            xQueueReceive(queue1,&num,0);
            printf("����1��������ֵΪ%d...\r\n",num);
        }
        else if(Task2queue == queue2)
        {
            xQueueReceive(queue2,&num,0);
            printf("����2��������ֵΪ%d...\r\n",num);
        }
        vTaskDelay(100);
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
