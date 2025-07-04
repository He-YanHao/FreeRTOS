#include "MyTask.h"

//���
TaskHandle_t Task_Start_TaskHandle;
TaskHandle_t LED0_TaskHandle;
TaskHandle_t LED1_TaskHandle;
TaskHandle_t Key_TaskHandle;

/**
 * @description: ����FreeRTOS
 * @return {*}
 */
void FreeRTOS_Start(void)
{
    xTaskCreate((TaskFunction_t)Task_Start,               // �������ĵ�ַ
                (char *)"Task_Start",                     // �������ַ���
                (configSTACK_DEPTH_TYPE)Task_Start_STACK, // ����ջ��С��Ĭ����С128����λ4�ֽ�
                (void *)NULL,                             // ���ݸ�����Ĳ���
                (UBaseType_t)Task_Start_PRIORITY,         // ��������ȼ�
                (TaskHandle_t *)&Task_Start_TaskHandle);      // �������ĵ�ַ
    /* ����������:���Զ������������� */
    vTaskStartScheduler();
}

void Task_Start(void)
{
    taskENTER_CRITICAL();//����������Դ
    /* �������� */
    xTaskCreate((TaskFunction_t)LED0__Toggle,               // �������ĵ�ַ
                (char *)"LED0_Toggle",                     // �������ַ���
                (configSTACK_DEPTH_TYPE)LED0__Toggle_STACK, // ����ջ��С��Ĭ����С128����λ4�ֽ�
                (void *)NULL,                             // ���ݸ�����Ĳ���
                (UBaseType_t)LED0__Toggle_PRIORITY,         // ��������ȼ�
                (TaskHandle_t *)&LED0_TaskHandle);      // �������ĵ�ַ

    xTaskCreate((TaskFunction_t)LED1__Toggle,               // �������ĵ�ַ
                (char *)"LED1_Toggle",                     // �������ַ���
                (configSTACK_DEPTH_TYPE)LED1__Toggle_STACK, // ����ջ��С��Ĭ����С128����λ4�ֽ�
                (void *)NULL,                             // ���ݸ�����Ĳ���
                (UBaseType_t)LED1__Toggle_PRIORITY,         // ��������ȼ�
                (TaskHandle_t *)&LED1_TaskHandle);      // �������ĵ�ַ

    xTaskCreate((TaskFunction_t)Key,               // �������ĵ�ַ
                (char *)"Key",                     // �������ַ���
                (configSTACK_DEPTH_TYPE)Key_STACK, // ����ջ��С��Ĭ����С128����λ4�ֽ�
                (void *)NULL,                             // ���ݸ�����Ĳ���
                (UBaseType_t)Key_PRIORITY,         // ��������ȼ�
                (TaskHandle_t *)&Key_TaskHandle);      // �������ĵ�ַ
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();//��������������Դ
}

void LED0__Toggle(void *pvParameters)
{
    while (1)
    {
        LED0_Toggle();
        vTaskDelay(500);
    }
}

void LED1__Toggle(void *pvParameters)
{
    while (1)
    {
        LED1_Toggle();
        vTaskDelay(500);
    }
}

void Key(void *pvParameters)
{
    uint8_t num = 0;
    while(1)
    {
        num = Key_return();
        if(num == 1)
        {
            if(LED0_TaskHandle != NULL)
            {
                OLED_ShowString(2, 1, "LED0_Delete");
                vTaskDelete(LED0_TaskHandle);
                LED0_TaskHandle = NULL;
            }
        }
        if(num == 2)
        {
            if(LED1_TaskHandle != NULL)
            {
                OLED_ShowString(3, 1, "LED1_Delete");
                vTaskDelete(LED1_TaskHandle);
                LED1_TaskHandle = NULL;
            }
        }
        vTaskDelay(500);
    }
}
