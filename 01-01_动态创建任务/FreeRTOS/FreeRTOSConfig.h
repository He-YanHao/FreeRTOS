#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H//防止重复定义

/* 抢占与时间片任务管理 */
#define configUSE_PREEMPTION		1
//置 1：FreeRTOS 使用抢占式调度器；  置 0：FreeRTOS 使用协作式调度器（时间片）。
//抢占式调度：在这种调度方式中，系统总是选择优先级最高的任务进行调度。
//一旦高优先级的任务准备就绪之后，它就会马上被调度而不等待低优先级的任务主动放弃CPU，高优先级的任务抢占了低优先级任务的CPU使用权。
//这是 FreeRTOS 的默认和推荐配置。
//协作式调度则是由任务主动放弃CPU，然后才进行任务调度。

/* 钩子函数 */
#define configUSE_IDLE_HOOK			0
//置 1：启用空闲任务钩子； 置 0：禁用空闲任务钩子。
//FreeRTOS 在启动时会自动创建一个最低优先级（优先级 0）的 IDLE 任务
//当没有其他任务运行时，调度器会执行 IDLE 任务。
//用于执行 vTaskDelete() 删除任务后的内存清理 和 低功耗模式。
//如果 configUSE_IDLE_HOOK = 1，用户可以定义一个 void vApplicationIdleHook(void) 函数。
//该函数会在空闲任务每次循环时被调用，适合执行低优先级后台任务或低功耗管理。
#define configUSE_TICK_HOOK			0
//置 1：启用 Tick 钩子； 置 0：禁用 Tick 钩子。
//FreeRTOS规定了函数的名字和参数：void vApplicationTickHook(void )
//是否允许每个时间片结束时的钩子函数。

/* 基础配置 */
#define configCPU_CLOCK_HZ			( ( unsigned long ) 72000000 )
//精确指定处理器内核的时钟频率
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
//RTOS系统节拍中断的频率。即一秒中断的次数，每次中断RTOS都会进行任务调度
#define configMAX_PRIORITIES		( 5 ) 
//可使用的最大优先级
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 ) 
//定义空闲任务(Idle Task)和定时器服务任务(Timer Task)的默认栈大小
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 17 * 1024 ) )
//定义系统动态内存堆的总大小

/* 任务配置 */
#define configMAX_TASK_NAME_LEN		( 16 ) 
//任务名字字符串长度

/* 调试配置 */
#define configUSE_TRACE_FACILITY	0
//启用系统跟踪和调试功能
//置 1：解锁完整调试功能； 置 0：基础调度功能。
//解锁完整调试功能 TCB+28字节/任务

#define configUSE_16_BIT_TICKS		0
//系统节拍计数器变量数据类型，1表示为16位无符号整形，0表示为32位无符号整形

#define configIDLE_SHOULD_YIELD		1
//空闲任务放弃CPU使用权给其他同优先级的用户任务
//置 1：每次循环主动让出CPU； 置 0：完整执行时间片	。

/* FreeRTOS可选函数配置选项 */
#define INCLUDE_vTaskPrioritySet		1
//允许运行时动态修改任务优先级
#define INCLUDE_uxTaskPriorityGet		1
//查询任务当前优先级
#define INCLUDE_vTaskDelete				1
////置 1：允许动态删除任务； 置 0：。
#define INCLUDE_vTaskCleanUpResources	0
//任务删除时的资源清理机制
#define INCLUDE_vTaskSuspend			1
//启用任务挂起(vTaskSuspend())和恢复(vTaskResume())功能
#define INCLUDE_vTaskDelayUntil			1
//启用固定频率的精确延时功能（基于绝对时间）
#define INCLUDE_vTaskDelay				1
//启用基本的相对时间延时功能

/* 中断配置 */
#define configKERNEL_INTERRUPT_PRIORITY 		255
//设置 FreeRTOS 内核使用的中断优先级
//数值越大优先级越低
//为了确保用户中断可抢占内核，系统中断设计为最低。
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191
//能够安全调用 FreeRTOS API 的最高中断优先级级别
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15
//用于定义内核中断的原始优先级值的配置参数

/* FreeRTOS与中断服务函数有关的配置选项 */
#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler
#define INCLUDE_xTaskGetSchedulerState   1

#endif
