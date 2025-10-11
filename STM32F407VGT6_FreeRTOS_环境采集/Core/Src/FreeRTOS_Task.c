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
TimerHandle_t FreeRTOS_OUT_TaskHandle;
void Task_Start(void)
{
    printf("Task_Start...\r\n");
    /* 创建任务 */
    xTaskCreate((TaskFunction_t)FreeRTOS_LED_Toggle,      // 任务函数的地址
                (char *)"FreeRTOS_LED_Toggle",            // 任务名字符串
                (configSTACK_DEPTH_TYPE)LED_Toggle_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)LED_Toggle_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&FreeRTOS_LED_Toggle_TaskHandle);// 任务句柄的地址
    
    FreeRTOS_OUT_TaskHandle = xTimerCreate(
                "FreeRTOS_OUT_TIMER",
                pdMS_TO_TICKS(1000), //时间片
                pdTRUE,
                (void*)1,
                FreeRTOS_OUT);
    if(FreeRTOS_OUT_TaskHandle != NULL)
    {
        if(xTimerStart(FreeRTOS_OUT_TaskHandle, 0) != pdPASS)
        {
            printf("xTimerStart NO\n\r");
        }else
        {
            printf("xTimerStart OK\n\r");
        }
    }
    vTaskDelete(NULL);
}

void FreeRTOS_LED_Toggle(void *pvParameters)
{
    while(1)
    {
        LED_Toggle();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

char taskListBuffer[512] = {};
char runTimeStatsBuffer[512] = {};

void FreeRTOS_OUT(TimerHandle_t xTimer)
{
    float Temperature = 0, Sunlight = 0;
    Temperature = DS18B20_GetTemperture();
    Sunlight = Get_ADC_Value();
    printf("Temperature:%f\r\n", Temperature);
    printf("Sunlight:%f\r\n", Sunlight);
    OLED_ShowFloatNum(0, 16, Temperature, 7, 16, 1);
    OLED_ShowNum(0, 48, Sunlight, 4, 16, 1);
    OLED_Refresh();

    vTaskList(taskListBuffer);
    printf("Name          State   Priority Stack   Num\n");
    printf("%s\n", taskListBuffer);

    vTaskGetRunTimeStats(runTimeStatsBuffer);
    printf("CPU Usage Summary:\n");
    printf("%s\n", runTimeStatsBuffer);
}

void vPreSleepProcessing( TickType_t xExpectedIdleTime )
{
    if(xExpectedIdleTime > 10)
    {
        printf("vPreSleepProcessing:%d\n\r",xExpectedIdleTime);
        // 中等时间空闲：进入睡眠模式
    }
}

void vPostSleepProcessing( TickType_t xExpectedIdleTime )
{
    printf("vPostSleepProcessing:%d\n\r",xExpectedIdleTime);
    // 睡眠模式唤醒后的处理
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
