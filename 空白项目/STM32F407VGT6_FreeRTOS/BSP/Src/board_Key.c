#include "board_Key.h"

//开发板上一共有三个按键，一个复位、一个 BOOT0 和一个 KEY
void Key_Init(void)
{
    /* 开启时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void KeyEXTI_Init(void)
{
    /* 开启时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //A0为中断线
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

    EXTI_InitTypeDef   EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 开启时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//开启定时器时钟
    /* 配置定时器参数 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 10000;
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1; // TIMER3时钟频率84MHz
    
    NVIC_InitTypeDef NVIC_InitStructure2;
    NVIC_InitStructure2.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0x03;
    
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    NVIC_Init(&NVIC_InitStructure2);//使能中断
    TIM_ITConfig(TIM3,TIM_IT_Update, DISABLE);//关闭定时器
    TIM_Cmd(TIM3,ENABLE);
}


volatile uint32_t timerCount = 0;// 定时器累计值

#define DEBOUNCE_THRESHOLD 20    // 消抖阈值(ms)

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        // 读取当前定时器值
        uint32_t currentTime = TIM_GetCounter(TIM3);
        // 首次按下或超过抖动周期
        if(currentTime == 0 || currentTime > DEBOUNCE_THRESHOLD) //首次按下 或者 按键时间超过有效值
        {
            //KeyEXTI = 1;  // 标记有效按键
        }        
        TIM_SetCounter(TIM3, 0);//重置定时器
        TIM_Cmd(TIM3, ENABLE); // 重新启用定时器
        EXTI_ClearITPendingBit(EXTI_Line0);//清除中断标志位
    }
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);// 超过按键周期，复位状态
        //KeyEXTI = 0;//标志位清零
        TIM_Cmd(TIM3, DISABLE); // 关闭定时器
        TIM_SetCounter(TIM3, 0); // 计数器清零
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

KEY_NUM Key_scan(void)
{
    /* 获取PA0引脚的高低电平状态 */
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
        Delay_ms(15);
        while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
        return KEY1;
    } else
    {
        return KEY0;
    }
}
