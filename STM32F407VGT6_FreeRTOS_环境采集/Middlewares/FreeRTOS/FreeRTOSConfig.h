#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H//防止重复定义

/* 抢占与时间片任务管理 */
#define configUSE_PREEMPTION		1
//置 1：FreeRTOS 使用抢占式调度器；    置 0：FreeRTOS 使用协作式调度器（时间片）。
//抢占式调度：系统总是选择优先级最高的任务进行调度而不等待低优先级的任务主动放弃CPU。是默认和推荐配置。
//协作式调度：由任务主动放弃CPU，然后才进行任务调度。
#define configUSE_TIME_SLICING      1
//置 1：启用时间片调度；    置 0：禁用时间片调度。
//时间片调度：默认启用时间片调度，每个相同优先级的任务就绪后会轮流各自运行一个时间片。禁用则需等待运行任务主动让出。

/* 基础配置 */
#define configCPU_CLOCK_HZ			( ( unsigned long ) 168000000 )
//精确指定处理器内核的时钟频率
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
//RTOS系统节拍中断的频率。即一秒中断的次数，每次中断RTOS都会进行任务调度
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 256 ) 
//定义空闲任务(Idle Task)和定时器服务任务(Timer Task)的默认栈大小
#define configUSE_16_BIT_TICKS		0
//系统节拍计数器变量数据类型，1表示为16位无符号整形，0表示为32位无符号整形

/* 任务配置 */
#define configMAX_TASK_NAME_LEN		    ( 16 ) 
//任务名称字符串最大长度
#define INCLUDE_vTaskDelete				1
//置 1：编译动态删除任务函数；置 0：不编译。 删除任务前应确保任务已释放所有资源
#define INCLUDE_vTaskCleanUpResources	0
//控制任务删除时的资源清理机制，已经被淘汰，现代版本中较少需要。一般置0。
#define INCLUDE_vTaskSuspend			1
//置 1：编译任务挂起与恢复函数，不含中断安全版本；置 0：不编译。
#define INCLUDE_xTaskResumeFromISR		1
//置 1：编译任务挂起恢复函数的中断安全版本；置 0：不编译。
#define configUSE_APPLICATION_TASK_TAG  0                       
//启用任务标签(Task Tag)功能
#define configUSE_NEWLIB_REENTRANT            0
//任务创建时分配Newlib的重入结构体
//为每个任务创建独立的 Newlib 重入结构体，解决多任务环境下的标准库线程安全问题。

/* 延时函数配置 */
#define INCLUDE_vTaskDelayUntil			1
//置 1：编译基于绝对时间的精确延时功能函数；置 0：不编译。
#define INCLUDE_vTaskDelay				1
//置 1：编译基于相对时间的延时功能函数；置 0：不编译。

/* 与内存申请有关配置选项 */
#define configSUPPORT_DYNAMIC_ALLOCATION      1
//支持动态内存申请
#define configSUPPORT_STATIC_ALLOCATION       0
//支持静态内存
#define configTOTAL_HEAP_SIZE                 ((size_t)(48*1024))
//定义系统动态内存堆的总大小
#define configSTACK_DEPTH_TYPE                uint16_t
//定义任务堆栈深度的数据类型

/* 优先级管理 */
#define configMAX_PRIORITIES		    ( 5 ) 
//可使用的最大优先级
#define INCLUDE_uxTaskPriorityGet		1
//置 1：编译查询任务优先级函数；置 0：不编译。
#define INCLUDE_vTaskPrioritySet		1
//置 1：编译修改任务优先级函数；置 0：不编译。
#define configUSE_PORT_OPTIMISED_TASK_SELECTION       1
//置 1：基于特定架构利用CPU特殊指令找到最高优先级的就绪任务； 置 0：通用C找到最高优先级的就绪任务。

/* 信号量与互斥量配置 */
#define configUSE_MUTEXES                     1
//为1时使用互斥信号量
#define configUSE_RECURSIVE_MUTEXES           1
//为1时使用递归互斥信号量
#define configUSE_COUNTING_SEMAPHORES         1
//为1时使用计数信号量

/* 队列配置 */
#define configUSE_QUEUE_SETS                  1    
//是否启用队列

/* 任务通知 */
#define configUSE_TASK_NOTIFICATIONS          1   
//开启任务通知功能，包括信号量、事件标志组和消息邮箱，默认开启。
#define configTASK_NOTIFICATION_ARRAY_ENTRIES 1
//定义任务通知数组的大小

/* 信号量和消息队列共同配置 */
#define configQUEUE_REGISTRY_SIZE             10                                 
//设置可以注册的信号量和消息队列个数

/* 与软件定时器有关的配置选项 */
#define configUSE_TIMERS                      1
//启用软件定时器
#define configTIMER_TASK_PRIORITY             (configMAX_PRIORITIES-1)
//软件定时器优先级 默认最大优先级
#define configTIMER_QUEUE_LENGTH              10
//软件定时器队列长度
#define configTIMER_TASK_STACK_DEPTH          (configMINIMAL_STACK_SIZE*2)
//软件定时器任务堆栈大小

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
//设置 FreeRTOS 内核使用的中断优先级，数值越大优先级越低，为了确保用户中断可抢占内核，系统中断设计为最低。
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

/* 低功耗宏 */
#define configUSE_TICKLESS_IDLE               1
//设置为 1 时，启用 Tickless 低功耗模式，会影响程序下载。
    #if configUSE_TICKLESS_IDLE
    #define configEXPECTED_IDLE_TIME_BEFORE_SLEEP 2
    //设置进入低功耗模式的最小空闲Tick数，以避免频繁进出睡眠。通常建议≥2。
    #define configPRE_SLEEP_PROCESSING(x)  vPreSleepProcessing(x)
    //可选宏。在进入低功耗前调用，用于关闭外设时钟等。
    #define configPOST_SLEEP_PROCESSING(x) vPostSleepProcessing(x)
    //可选宏。在退出低功耗后调用，用于重新开启外设等。
    void vPreSleepProcessing( uint32_t xExpectedIdleTime );
    void vPostSleepProcessing( uint32_t xExpectedIdleTime );
#endif

/* 与运行时间和任务状态收集有关的配置选项，多用于调试。 */
#define configGENERATE_RUN_TIME_STATS         1
//启用运行时间统计功能
    #if configGENERATE_RUN_TIME_STATS
    //如果启用了运行时间统计功能
    #define configUSE_TRACE_FACILITY	          1
    //启用系统跟踪和调试功能
    //置 1：解锁完整调试功能，会使TCB+28字节/任务； 置 0：基础调度功能。
    #define configUSE_STATS_FORMATTING_FUNCTIONS  1                       
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

/* 钩子函数 */
#define configUSE_IDLE_HOOK			          0
//置 1：启用空闲任务钩子函数； 置 0：禁用空闲任务钩子函数。
//空闲任务钩子函数：FreeRTOS在启动时会自动创建一个最低优先级（优先级 0）的 IDLE 任务，当没有其他任务运行时，调度器会执行 IDLE 任务，用于执行 vTaskDelete() 删除任务后的内存清理 和 低功耗模式。
//钩子函数原型：void vApplicationIdleHook(void)
#define configIDLE_SHOULD_YIELD		          1
//置 1：空闲任务进入后检查是否有用户定义的优先级为0任务，若有则立即主动让出CPU； 置 0：空闲任务完作为普通任务与用户定义的优先级为0任务竞争时间片。
//可以置2，与1略有不同。
#define configUSE_TICK_HOOK			          0
//置 1：启用时钟节拍钩子函数； 置 0：禁用时钟节拍钩子函数。
//时钟节拍钩子函数：在系统的时钟节拍中断服务程序中调用。它是在中断上下文中执行的！
//执行时间必须非常短，因为它是在中断中执行的。且仅能调用以FromISR结尾中断安全版本的API。
//钩子函数原型：void vApplicationTickHook(void )
#define configUSE_DAEMON_TASK_STARTUP_HOOK    0
//置 1：启用守护进程钩子函数； 置 0：禁用守护进程钩子函数。
//在定时器服务任务启动时，且在其进入主循环之前被调用一次，用于初始化一些需要在该任务上下文中运行的资源。
//钩子函数原型：void vApplicationDaemonTaskStartupHook( void );
#define configCHECK_FOR_STACK_OVERFLOW        0
//置 1或2：启用栈溢出钩子函数； 置 0：禁用栈溢出钩子函数。
//此值可以为1或者2，因为有两种栈溢出检测方法。
//栈溢出钩子函数：当内核检测到某个任务的栈空间溢出时调用。
//钩子函数原型：void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
#define configUSE_MALLOC_FAILED_HOOK          0
//置 1：启用Malloc失败钩子函数； 置 0：禁用Malloc失败钩子函数。
//当使用 pvPortMalloc() 分配内存失败时调用。
//钩子函数原型：void vApplicationMallocFailedHook( void )

/* 断言 */
//#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
//#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)

/* 协程有关的配置选项 */
#define configUSE_CO_ROUTINES                 0
//置 1：启用协程功能，启用协程以后必须添加文件croutine.c； 置 0：禁用协程功能。
//协程功能可以节省系统资源，但不支持抢占调度，不支持阻塞函数，必须主动让出CPU，推荐禁止。
#define configMAX_CO_ROUTINE_PRIORITIES       ( 2 )
//定义协程优先级等级数 协程的有效优先级数目

/* 其他配置 */
#define configENABLE_BACKWARD_COMPATIBILITY   0
//置 1：兼容老版本； 置 0：只支持新版。
#define configUSE_ALTERNATIVE_API             0
//必须置 0。FreeRTOS早期定义了基于特定平台的功能函数，但现在这些特点函数和通用函数性能差异不大，已弃用。
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS   0
//定义线程本地存储指针的个数，为每个任务提供线程本地存储（TLS）指针，类似于线程局部变量。
#define configMESSAGE_BUFFER_LENGTH_TYPE      size_t
//定义消息缓冲区中消息长度的数据类型

#endif
