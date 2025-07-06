#include "MyTask.h"

//char pcWriteBuffer[512]; // 足够大的缓冲区

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

/* 队列句柄 */
QueueHandle_t queue1 = NULL;
QueueHandle_t queue2 = NULL;
QueueSetHandle_t queue0 = NULL;

//大数据
char name[] = "123456789";
//char *name = "123456789";

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
    vTaskStartScheduler();
    vTaskDelete(NULL);
}

//task0自动创建队列集
void Task0(void *pvParameters)
{
    uint8_t re = 0;

    //创建成员1
    queue1 = xQueueCreate(2,2);
    //判断成员2
    if(queue1 != NULL)
    {
        printf("queue1 is OK\r\n");
    }
    else
    {
        printf("\r\n");
    }

    //创建成员2
    queue2 = xQueueCreate(1,1);
    //判断成员2
    if(queue2 != NULL)
    {
        printf("queue2 is OK\r\n");
    }
    else
    {
        printf("\r\n");
    }

    queue0 = xQueueCreateSet(2);
    //创建队列集queue0
    if(queue0 != NULL)
    {
        printf("队列集创建成功\r\n");
    }
    else
    {
        printf("队列集创建失败\r\n");
    }

    //添加成员1
    re = xQueueAddToSet(queue1,queue0);
    if(re == pdPASS)
    {
        printf("成员1成功添加到集合中\r\n");
    }
    else
    {
        printf("成员1添加到集合中失败\r\n");
    }

    //添加成员2
    re = 0;
    re = xQueueAddToSet(queue2,queue0);
    if(re == pdPASS)
    {
        printf("成员2成功添加到集合中\r\n");
    }
    else
    {
        printf("成员2添加到集合中失败\r\n");
    }

    vTaskDelete(NULL);
}

//按下key0后给task1将数据入队 按下key1后给task2将数据入队
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
            printf("任务1入队...\r\n");
        }
        else if(key == KEY1)
        {
            xQueueSendToBack(queue2,&queue2_num,0);
            printf("任务2入队...\r\n");
        }
        vTaskDelay(50);
    }
}

//输出队列集
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
            printf("任务1就绪，数值为%d...\r\n",num);
        }
        else if(Task2queue == queue2)
        {
            xQueueReceive(queue2,&num,0);
            printf("任务2就绪，数值为%d...\r\n",num);
        }
        vTaskDelay(100);
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
