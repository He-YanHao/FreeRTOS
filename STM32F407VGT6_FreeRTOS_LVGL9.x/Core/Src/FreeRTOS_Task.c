#include "FreeRTOS_Task.h"

TaskHandle_t Task_Start_TaskHandle;
void FreeRTOS_Start(void)
{
    printf("FreeRTOS_Start...\r\n");
    xTaskCreate((TaskFunction_t)Task_Start,               // 任务函数的地址
                (char *)"Task_Start",                     // 任务名字符串
                (configSTACK_DEPTH_TYPE)Task_Start_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)Task_Start_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&Task_Start_TaskHandle);  // 任务句柄的地址
    printf("FreeRTOS_Start OK!!!\r\n");
    /* 启动调度器:会自动创建空闲任务 */
    vTaskStartScheduler();
}

TaskHandle_t FreeRTOS_LED_Toggle_TaskHandle;
TaskHandle_t FreeRTOS_USART_TaskHandle;
void Task_Start(void)
{
    printf("Task_Start...\r\n");
    /* 创建任务 */
    xTaskCreate((TaskFunction_t)FreeRTOS_LED_Toggle,      // 任务函数的地址
                (char *)"FreeRTOS_LED_Toggle",            // 任务名字符串
                (configSTACK_DEPTH_TYPE)LED_Toggle_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)LED_Toggle_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&FreeRTOS_LED_Toggle_TaskHandle);      // 任务句柄的地址
    xTaskCreate((TaskFunction_t)FreeRTOS_USART,      // 任务函数的地址
                (char *)"FreeRTOS_USART",            // 任务名字符串
                (configSTACK_DEPTH_TYPE)FreeRTOS_USART_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)FreeRTOS_USART_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&FreeRTOS_USART_TaskHandle);      // 任务句柄的地址
    vTaskDelete(NULL);
}

void FreeRTOS_LED_Toggle(void *pvParameters)
{
    while(1)
    {
        LED_Toggle();
        printf("FreeRTOS_LED_Toggle\n\r");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

char taskListBuffer[512] = {};
char runTimeStatsBuffer[512] = {};

void FreeRTOS_USART(void *pvParameters)
{
    while(1)
    {
        printf("FreeRTOS_USART\n\r");
        vTaskDelay(pdMS_TO_TICKS(1000));

        vTaskList(taskListBuffer);
        printf("Name          State   Priority Stack   Num\n");
        printf("%s\n", taskListBuffer);

        vTaskGetRunTimeStats(runTimeStatsBuffer);
        printf("CPU Usage Summary:\n");
        printf("%s\n", runTimeStatsBuffer);
    }
}

// 使用 DWT 周期计数器 (Cortex-M3/M4)
#include "core_cm4.h" // 确保包含 CMSIS 头文件
//初始化用于运行时统计的定时器
void ConfigureTimerForRuntimeStats(void) {
    /* 启用 DWT 计数器 */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}
//获取当前定时器的计数值
uint32_t GetRuntimeCounterValue(void) {
    /* 返回 CPU 周期计数 */
    return DWT->CYCCNT;
}
