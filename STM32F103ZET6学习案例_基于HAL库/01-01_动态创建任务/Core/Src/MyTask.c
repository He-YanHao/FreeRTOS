#include "MyTask.h"

//句柄
TaskHandle_t Task_Start_TaskHandle;
TaskHandle_t LED0_TaskHandle;
TaskHandle_t LED1_TaskHandle;
TaskHandle_t Key_TaskHandle;

/**
 * @description: 启动FreeRTOS
 * @return {*}
 */
void FreeRTOS_Start(void)
{
    printf("Creating Start Task...\r\n");
    xTaskCreate((TaskFunction_t)Task_Start,               // 任务函数的地址
                (char *)"Task_Start",                     // 任务名字符串
                (configSTACK_DEPTH_TYPE)Task_Start_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)Task_Start_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&Task_Start_TaskHandle);      // 任务句柄的地址
    /* 启动调度器:会自动创建空闲任务 */
    vTaskStartScheduler();
}

void Task_Start(void)
{
    taskENTER_CRITICAL();//保护共享资源
    /* 创建任务 */
    xTaskCreate((TaskFunction_t)LED0__Toggle,               // 任务函数的地址
                (char *)"LED0_Toggle",                     // 任务名字符串
                (configSTACK_DEPTH_TYPE)LED0__Toggle_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)LED0__Toggle_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&LED0_TaskHandle);      // 任务句柄的地址

    xTaskCreate((TaskFunction_t)LED1__Toggle,               // 任务函数的地址
                (char *)"LED1_Toggle",                     // 任务名字符串
                (configSTACK_DEPTH_TYPE)LED1__Toggle_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)LED1__Toggle_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&LED1_TaskHandle);      // 任务句柄的地址

    xTaskCreate((TaskFunction_t)Key,               // 任务函数的地址
                (char *)"Key",                     // 任务名字符串
                (configSTACK_DEPTH_TYPE)Key_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)Key_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&Key_TaskHandle);      // 任务句柄的地址
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();//结束保护共享资源
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
