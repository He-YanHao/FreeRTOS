#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H//防止重复定义

/* 抢占与时间片任务管理 */
#define configUSE_PREEMPTION		1
//置 1：FreeRTOS 使用抢占式调度器；  置 0：FreeRTOS 使用协作式调度器（时间片）。
//抢占式调度：在这种调度方式中，系统总是选择优先级最高的任务进行调度。
//一旦高优先级的任务准备就绪之后，它就会马上被调度而不等待低优先级的任务主动放弃CPU，高优先级的任务抢占了低优先级任务的CPU使用权。
//这是 FreeRTOS 的默认和推荐配置。
//协作式调度则是由任务主动放弃CPU，然后才进行任务调度。
#define configUSE_TIME_SLICING      1		
//置 1：抢占式调度； 置 0：协作式调度。
//只有 configUSE_PREEMPTION = 1（）时，configUSE_TIME_SLICING 才有意义。
//如果 configUSE_PREEMPTION = 0（），任务切换完全由任务自己控制，时间片轮转无效。
//使能时间片调度(默认式使能的)。当优先级相同的时候，就会采用时间片调度。
//这意味着 RTOS 调度器总是运行处于最高优先级的就绪任务，在每个FreeRTOS 系统节拍中断时在相同优先级的多个任务间进行任务切换。
//如果宏configUSE_TIME_SLICING 设置为 0，FreeRTOS 调度器仍然总是运行处于最高优先级的就绪任务，但是当 RTOS 系统节拍中断发生时，相同优先级的多个任务之间不再进行任务切换，而是在执行完高优先级的任务之后才进行任务切换。
//一般来说，FreeRTOS 默认支持32 个优先级，很少情况会把 32 个优先级全用完，所以，官方建议采用抢占式调度。

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
#define configIDLE_SHOULD_YIELD		1
//空闲任务放弃CPU使用权给其他同优先级的用户任务
//置 1：每次循环主动让出CPU； 置 0：完整执行时间片	。

/* 基础配置 */
#define configCPU_CLOCK_HZ			( ( unsigned long ) 72000000 )
//精确指定处理器内核的时钟频率
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
//RTOS系统节拍中断的频率。即一秒中断的次数，每次中断RTOS都会进行任务调度
#define configMAX_PRIORITIES		( 5 ) 
//可使用的最大优先级
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 ) 
//定义空闲任务(Idle Task)和定时器服务任务(Timer Task)的默认栈大小
#define configUSE_16_BIT_TICKS		0
//系统节拍计数器变量数据类型，1表示为16位无符号整形，0表示为32位无符号整形

/* 任务配置 */
#define configMAX_TASK_NAME_LEN		( 16 ) 
//任务名字字符串长度

/* FreeRTOS与内存申请有关配置选项 */
#define configSUPPORT_DYNAMIC_ALLOCATION      1
//支持动态内存申请
#define configSUPPORT_STATIC_ALLOCATION       1
//支持静态内存
#define configTOTAL_HEAP_SIZE                 ((size_t)(36*1024))
//定义系统动态内存堆的总大小
#define configCHECK_FOR_STACK_OVERFLOW        0
//大于0时启用堆栈溢出检测功能
//此值可以为1或者2，因为有两种栈溢出检测方法
//用户必须提供一个栈溢出钩子函数

/* FreeRTOS与软件定时器有关的配置选项 */
#define configUSE_TIMERS                      1
//启用时，自动创建定时器守护任务 (Timer Daemon Task / Timer Service Task) 和定时器命令队列 (Timer Command Queue)。
#define configTIMER_TASK_PRIORITY             (configMAX_PRIORITIES-1)
//软件定时器优先级 配置最大优先级configMAX_PRIORITIES -1，为系统可用的最高优先级。
#define configTIMER_QUEUE_LENGTH              10
//软件定时器队列长度
#define configTIMER_TASK_STACK_DEPTH          (configMINIMAL_STACK_SIZE*2)
//软件定时器任务堆栈大小

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
#define INCLUDE_xTaskResumeFromISR		1
//中断中恢复任务函数所需宏
#define INCLUDE_vTaskDelayUntil			1
//启用固定频率的精确延时功能（基于绝对时间）
#define INCLUDE_vTaskDelay				1
//启用基本的相对时间延时功能

/* 队列与信号量配置 */
#define configUSE_QUEUE_SETS                  1    
//启用队列
#define configUSE_TASK_NOTIFICATIONS          1   
//开启任务通知功能，包括信号量、事件标志组和消息邮箱，默认开启。
#define configTASK_NOTIFICATION_ARRAY_ENTRIES 1
//定义任务通知数组的大小
#define configUSE_MUTEXES                     0    
//为1时使用互斥信号量
#define configUSE_RECURSIVE_MUTEXES           0   
//为1时使用递归互斥信号量                                            
#define configUSE_COUNTING_SEMAPHORES         0
//为1时使用计数信号量
#define configUSE_ALTERNATIVE_API             0
//已弃用!!!   为1时使用替代 API，可能更高效，但依赖硬件支持，需测试稳定性。
#define configQUEUE_REGISTRY_SIZE             10                                 
//设置可以注册的信号量和消息队列个数

/* 与运行时间和任务状态收集有关的配置选项，多用于调试。 */
#define configGENERATE_RUN_TIME_STATS         1
//启用运行时间统计功能
#if configGENERATE_RUN_TIME_STATS
//如果启用了运行时间统计功能
#define configUSE_TRACE_FACILITY	          1
//启用系统跟踪和调试功能
//置 1：解锁完整调试功能； 置 0：基础调度功能。
//解锁完整调试功能 TCB+28字节/任务
#define configUSE_STATS_FORMATTING_FUNCTIONS  1                       
//与宏configUSE_TRACE_FACILITY同时为1时会编译下面3个函数
//prvWriteNameToBuffer()
//vTaskList(),
//vTaskGetRunTimeStats()
//将任务信息格式化为人类可读的字符串形式
#define INCLUDE_uxTaskGetStackHighWaterMark 1
//启用栈水位标记
extern void ConfigureTimerForRuntimeStats(void);
//DWT计时器 初始化用于运行时统计的定时器
extern uint32_t GetRuntimeCounterValue(void);
//DWT计时器 获取当前定时器的计数值
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() ConfigureTimerForRuntimeStats()
//初始化用于运行时统计的定时器
#define portGET_RUN_TIME_COUNTER_VALUE() GetRuntimeCounterValue()
//获取当前定时器的计数值
#endif

/* 中断配置 */
#ifdef __NVIC_PRIO_BITS                       //如果定义了__NVIC_PRIO_BITS
	#define configPRIO_BITS       		      __NVIC_PRIO_BITS
    //如果定义直接用__NVIC_PRIO_BITS的值代替configPRIO_BITS
#else
	#define configPRIO_BITS                   4
    //如果没有定义，使用默认值4，可以满足大多数情况。
#endif
//通过条件编译确定优先级位宽
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY            15
//中断最低优先级 STM32里数字越大优先级越低
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY       5
//FreeRTOS可屏蔽的最高中断优先级
#define configKERNEL_INTERRUPT_PRIORITY       ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )	/* 240 */
//configLIBRARY_LOWEST_INTERRUPT_PRIORITY为中断最低优先级(STM32标准)，STM32里为15。
//configPRIO_BITS为中断优先级的位数，默认为4
//configKERNEL_INTERRUPT_PRIORITY设置为FreeRTOS里最低优先级(FreeRTOS标准)，保证任何任务都能抢占。
//设置 FreeRTOS 内核使用的中断优先级
//数值越大优先级越低
//为了确保用户中断可抢占内核，系统中断设计为最低。
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )	/* 80 */
//configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY为FreeRTOS可屏蔽的最高中断优先级(STM32标准)
//configMAX_SYSCALL_INTERRUPT_PRIORITY为FreeRTOS可以管理（屏蔽）的最高中断优先级(FreeRTOS标准)
//能够安全调用 FreeRTOS API 的最高中断优先级级别
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15
//用于定义内核中断的原始优先级值的配置参数

/* FreeRTOS与中断服务函数有关的配置选项 */
#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler
#define INCLUDE_xTaskGetSchedulerState   1

#endif
