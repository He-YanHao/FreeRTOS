#include "MyTask.h"

char pcWriteBuffer[512]; // 足够大的缓冲区

/* 启动任务的配置 */
TaskHandle_t LED0_Toggle_Task_handle;      //声明句柄
StackType_t LED0_Toggle_Task_stack[LED0_Toggle_Task_STACK]; // 静态任务的任务栈，以数组形式存储
//typedef uint32_t StackType_t;  // 对于32位处理器
StaticTask_t LED0_Toggle_Task_tcb;                    // 静态任务的TCB结构体类型
//无需关心具体结果 储存任务细节

TaskHandle_t LED1_Toggle_Task_handle;      //声明句柄
StackType_t LED1_Toggle_Task_stack[LED1_Toggle_Task_STACK]; // 静态任务的任务栈，以数组形式存储
//typedef uint32_t StackType_t;  // 对于32位处理器
StaticTask_t LED1_Toggle_Task_tcb;                    // 静态任务的TCB结构体类型
//无需关心具体结果 储存任务细节

TaskHandle_t Key_handle;      //声明句柄
StackType_t Key_stack[Key_STACK]; // 静态任务的任务栈，以数组形式存储
//typedef uint32_t StackType_t;  // 对于32位处理器
StaticTask_t Key_tcb;                    // 静态任务的TCB结构体类型
//无需关心具体结果 储存任务细节


void freertos_start(void)
{
    //创建任务
 LED0_Toggle_Task_handle = xTaskCreateStatic(
                           LED0_Toggle_Task,          // 任务函数地址
                           "LED1_Toggle_Task",        // 任务名称
                           LED0_Toggle_Task_STACK,    // 栈大小
                           NULL,                     // 参数
                           LED0_Toggle_Task_PRIORITY, // 优先级
                           LED0_Toggle_Task_stack,    // 任务栈数组
                           &LED0_Toggle_Task_tcb );   // 任务控制块
// LED1_Toggle_Task_handle = xTaskCreateStatic(
//                           LED1_Toggle_Task,          // 任务函数地址
//                           "LED2_Toggle_Task",        // 任务名称
//                           LED1_Toggle_Task_STACK,    // 栈大小
//                           NULL,                     // 参数
//                           LED1_Toggle_Task_PRIORITY, // 优先级
//                           LED1_Toggle_Task_stack,    // 任务栈数组
//                           &LED1_Toggle_Task_tcb );   // 任务控制块
//    Key_handle      =      xTaskCreateStatic(
//                           Key,          // 任务函数地址
//                           "Key",        // 任务名称
//                           Key_STACK,    // 栈大小
//                           NULL,              // 参数
//                           Key_PRIORITY, // 优先级
//                           Key_stack,    // 任务栈数组
//                           &Key_tcb );   // 任务控制块
    vTaskStartScheduler();
}

void LED0_Toggle_Task(void *pvParameters)
{
    taskENTER_CRITICAL();//保护共享资源
    while (1)
    {
        //LED0_Toggle();
        vTaskDelay(500);
    }
}

void LED1_Toggle_Task(void *pvParameters)
{
    while (1)
    {
        LED1_Toggle();
        vTaskDelay(50);
    }
}

void Key(void *pvParameters)
{
    uint8_t keynum = 0;
    while(1)
    {
        keynum = Key_return();
        if(keynum == KEY0)
        {
            printf(" >>>>>> LED_OFF >>>>> \r\n");
            vTaskSuspend(LED1_Toggle_Task_handle);            
        }
        if(keynum == KEY1)
        {
            printf(" >>>>>> LED_ON >>>>> \r\n");
            vTaskResume(LED1_Toggle_Task_handle);
        }
        vTaskList(pcWriteBuffer);
        printf("Task List:\n%s\n", pcWriteBuffer);
        vTaskDelay(1000);
    }
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
