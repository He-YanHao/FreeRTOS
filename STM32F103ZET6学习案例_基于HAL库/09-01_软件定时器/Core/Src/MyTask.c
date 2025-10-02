#include "MyTask.h"

char pcWriteBuffer[512]; // 足够大的缓冲区
UBaseType_t Debugging_Num = 0;

/* 启动任务的配置 */
TaskHandle_t Task0_handle;      //声明句柄
StackType_t Task0_stack[Task0_STACK]; // 静态任务的任务栈，以数组形式存储
StaticTask_t Task0_tcb;                    // 静态任务的TCB结构体类型
//无需关心具体结果 储存任务细节

TaskHandle_t Task1_handle;      //声明句柄
StackType_t Task1_stack[Task1_STACK]; // 静态任务的任务栈，以数组形式存储
StaticTask_t Task1_tcb;                    // 静态任务的TCB结构体类型
//无需关心具体结果 储存任务细节

TaskHandle_t Task2_handle;      //声明句柄
StackType_t Task2_stack[Task2_STACK]; // 静态任务的任务栈，以数组形式存储
StaticTask_t Task2_tcb;                    // 静态任务的TCB结构体类型
//无需关心具体结果 储存任务细节

TaskHandle_t Debugging_handle;      //声明句柄
StackType_t Debugging_stack[Debugging_STACK]; // 静态任务的任务栈，以数组形式存储
StaticTask_t Debugging_tcb;                    // 静态任务的TCB结构体类型
//无需关心具体结果 储存任务细节

//句柄
TimerHandle_t TimerHandle1 = NULL;
TimerHandle_t TimerHandle2 = NULL;

void freertos_start(void)
{
    printf("任务开始\r\n");
    //创建任务
      Task0_handle    =    xTaskCreateStatic(
                           Task0,// 任务函数地址
                           "Task0",         // 任务名称
                           Task0_STACK,     // 栈大小
                           NULL,            // 参数
                           Task0_PRIORITY,  // 优先级
                           Task0_stack,     // 任务栈数组
                           &Task0_tcb );    // 任务控制块
      Task1_handle    =    xTaskCreateStatic(
                           Task1,           // 任务函数地址
                           "Task1",         // 任务名称
                           Task1_STACK,     // 栈大小
                           NULL,            // 参数
                           Task1_PRIORITY,  // 优先级
                           Task1_stack,     // 任务栈数组
                           &Task1_tcb );    // 任务控制块
      Task2_handle    =    xTaskCreateStatic(
                           Task2,           // 任务函数地址
                           "Task2",         // 任务名称
                           Task2_STACK,     // 栈大小
                           NULL,            // 参数
                           Task2_PRIORITY,  // 优先级
                           Task2_stack,     // 任务栈数组
                           &Task2_tcb );    // 任务控制块
      Debugging_handle  =  xTaskCreateStatic(
                           Debugging,       // 任务函数地址
                           "Debugging",         // 任务名称
                           Debugging_STACK,     // 栈大小
                           NULL,            // 参数
                           Debugging_PRIORITY,  // 优先级
                           Debugging_stack,     // 任务栈数组
                           &Debugging_tcb );    // 任务控制块
    //vTaskDelete(NULL);//自删 未启用调度器暂时不会自删
    vTaskStartScheduler();//启用任务调度器
    //vTaskDelete(NULL);//自删 未启用调度器暂时不会自删
}

// 定时器回调函数
void led_timer_callback1(TimerHandle_t xTimer)
{
    //LED0_Toggle();  // 切换LED状态
    // 注意：回调函数中不能调用任何阻塞API！
}
void led_timer_callback2(TimerHandle_t xTimer)
{
    LED1_Toggle();  // 切换LED状态
    // 注意：回调函数中不能调用任何阻塞API！
}

//task0
void Task0(void *pvParameters)
{
    TimerHandle1 = xTimerCreate("Time", 200, pdTRUE, (void *)1, led_timer_callback1);
    if(TimerHandle1 != NULL)
    {
        printf("软件定时器1创建成功\r\n");
    }
    xTimerStart(TimerHandle1, 0);
    TimerHandle2 = xTimerCreate("Time", 200, pdTRUE, (void *)2, led_timer_callback2);
    if(TimerHandle2 != NULL)
    {
        printf("软件定时器2创建成功\r\n");
    }
    xTimerStart(TimerHandle2, 0);
    vTaskDelete(NULL);
}

//
void Task1(void *pvParameters)
{
    uint8_t key = 0;
    uint32_t time1 = 200;
    uint32_t time2 = 200;
    while(1)
    {
        key = Key_return();
        if(key == KEY0)
        {
            if(time1 > 1000)
            {
                time1 = 200;
            }
            else
            {
                time1 = time1 + 200;
            }
            xTimerChangePeriod(TimerHandle1, time1, 0);
            printf("\r\n");
        }
        else if(key == KEY1)
        {
            if(time2 > 1000)
            {
                time2 = 200;
            }
            else
            {
                time2 = time2 + 200;
            }
            xTimerChangePeriod(TimerHandle2, time2, 0);
            printf("\r\n");
        }
        vTaskDelay(50);
    }
}

//
void Task2(void *pvParameters)
{
    uint32_t num = 0;
    while(1)
    {
        printf("%u\r\n",num);
        vTaskDelay(50);
    }
}

void Debugging(void *pvParameters)
{
    while(1)
    {
//        Debugging_Num = uxTaskGetStackHighWaterMark(Task1_handle);
//        printf("Task1堆栈高水位标记为：%lu\r\n",Debugging_Num);
//        Debugging_Num = uxTaskGetStackHighWaterMark(Task2_handle);
//        printf("Task2堆栈高水位标记为：%lu\r\n",Debugging_Num);
//        Debugging_Num = uxTaskGetStackHighWaterMark(NULL);
//        printf("Debugging堆栈高水位标记为：%lu\r\n",Debugging_Num);
//        vTaskGetRunTimeStats(pcWriteBuffer);
//        printf("%s\r\n",pcWriteBuffer);
        vTaskDelay(3000);
    }
}

// 使用 DWT 周期计数器 (Cortex-M3/M4)
void ConfigureTimerForRuntimeStats(void) {
    /* 启用 DWT 计数器 */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t GetRuntimeCounterValue(void) {
    /* 返回 CPU 周期计数 */
    return DWT->CYCCNT;
}

/* 静态创建方式，需要手动指定2个特殊任务的资源 */
/* 空闲任务的配置 */
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE]; // 静态任务的任务栈，以数组形式存储
StaticTask_t idle_task_tcb;                            // 静态任务的TCB结构体类型
/* 分配空闲任务的资源 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &idle_task_tcb;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* 软件定时器任务的配置 */
StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH]; // 静态任务的任务栈，以数组形式存储
StaticTask_t timer_task_tcb;                            // 静态任务的TCB结构体类型
/* 分配软件定时器任务的资源 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &timer_task_tcb;
    *ppxTimerTaskStackBuffer = timer_task_stack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
