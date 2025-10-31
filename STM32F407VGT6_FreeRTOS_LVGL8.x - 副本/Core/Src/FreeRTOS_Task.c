#include "FreeRTOS_Task.h"

//启动任务句柄
TaskHandle_t Task_Start_TaskHandle;
void FreeRTOS_Start(void)
{
    printf("FreeRTOS_Start...\r\n");

    lv_init();                 /* lvgl系统初始化 */ 
    lv_port_disp_init();       /* lvgl显示接口初始化,放在lv_init()的后面 */ 
    lv_port_indev_init();       /* lvgl输入接口初始化,放在lv_init()的后面 */

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

//LED闪烁任务句柄
TaskHandle_t FreeRTOS_LED_Toggle_TaskHandle;
//串口输出任务句柄
TaskHandle_t FreeRTOS_USART_TaskHandle;
//创建LVGL任务句柄
TaskHandle_t LV_DEMOTask_Handler;

void Task_Start(void)
{
    printf("Task_Start...\r\n");
    /* 创建任务 */
    /* 进入临界区 */ 
    taskENTER_CRITICAL();
    /* 创建LED闪烁任务 */
    xTaskCreate((TaskFunction_t)FreeRTOS_LED_Toggle,      // 任务函数的地址
                (char *)"FreeRTOS_LED_Toggle",            // 任务名字符串
                (configSTACK_DEPTH_TYPE)LED_Toggle_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)LED_Toggle_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&FreeRTOS_LED_Toggle_TaskHandle);      // 任务句柄的地址
    /* 创建串口输出任务 */
    xTaskCreate((TaskFunction_t)FreeRTOS_USART,      // 任务函数的地址
                (char *)"FreeRTOS_USART",            // 任务名字符串
                (configSTACK_DEPTH_TYPE)FreeRTOS_USART_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)FreeRTOS_USART_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&FreeRTOS_USART_TaskHandle);      // 任务句柄的地址
    /* 创建LVGL任务 */ 
    xTaskCreate((TaskFunction_t )lv_demo_task, 
                (const char*    )"lv_demo_task", 
                (uint16_t       )LV_DEMO_STK_SIZE,  
                (void*          )NULL, 
                (UBaseType_t    )LV_DEMO_TASK_PRIO, 
                (TaskHandle_t*  )&LV_DEMOTask_Handler);
    taskEXIT_CRITICAL();
    /* 退出临界区 */ 
    vTaskDelete(NULL);
    /* 删除自身 */ 
}

/** 
 * @brief       LVGL运行例程 
 * @param       pvParameters : 传入参数(未用到) 
 * @retval      无 
 */ 
void lv_demo_task(void *pvParameters) 
{ 
    //lv_demo_music();  /* 测试的demo */ 
    while(1) 
    { 
        lv_timer_handler(); /* LVGL计时器 */ 
        vTaskDelay(5); 
    } 
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
