#include "FreeRTOS_Task.h"

// 任务句柄
TaskHandle_t Task_Start_TaskHandle;

void FreeRTOS_Start(void)
{
    xTaskCreate((TaskFunction_t)Task_Start,               // 任务函数的地址
                (char *)"Task_Start",                     // 任务名字符串
                (configSTACK_DEPTH_TYPE)Task_Start_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)Task_Start_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&Task_Start_TaskHandle);  // 任务句柄的地址
    /* 启动调度器:会自动创建空闲任务 */
    vTaskStartScheduler();
}

// 任务句柄
TaskHandle_t FreeRTOS_LED_Toggle_TaskHandle1;
TaskHandle_t FreeRTOS_LED_Toggle_TaskHandle2;

void Task_Start(void)
{
    /* 创建任务 */
    xTaskCreate((TaskFunction_t)FreeRTOS_LED_Toggle1,      // 任务函数的地址
                (char *)"FreeRTOS_LED_Toggle1",            // 任务名字符串
                (configSTACK_DEPTH_TYPE)LED_Toggle_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)LED_Toggle_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&FreeRTOS_LED_Toggle_TaskHandle1);      // 任务句柄的地址

    xTaskCreate((TaskFunction_t)FreeRTOS_LED_Toggle2,      // 任务函数的地址
                (char *)"FreeRTOS_LED_Toggle2",            // 任务名字符串
                (configSTACK_DEPTH_TYPE)LED_Toggle_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)LED_Toggle_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&FreeRTOS_LED_Toggle_TaskHandle2);      // 任务句柄的地址

    vTaskDelete(NULL);
}

void FreeRTOS_LED_Toggle1(void *pvParameters)
{
    while(1)
    {
        BSP_LED1_TOGGLE();
        vTaskDelay(pdMS_TO_TICKS(50));
        BSP_LED2_TOGGLE();
        vTaskDelay(pdMS_TO_TICKS(50));
        BSP_LED3_TOGGLE();
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
void FreeRTOS_LED_Toggle2(void *pvParameters)
{
    while(1)
    {
        BSP_LED4_TOGGLE();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}