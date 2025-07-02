#include "key.h"

/**
  * @brief  精确延时1ms（72MHz主频）
  * @param  无
  * @retval 无
  * @note 基于72个时钟周期/微秒计算，经实测校准
  */
void Delay_1ms(void)
{
    volatile uint32_t delay = 72 * 1000 / 4;  // 经编译器优化调整后的数值
    while(delay--);
}

void Delay_ms(uint16_t ms)
{
    while(ms--)
    {
        Delay_1ms();
    }
}

void Key_Init(void)
{
    // 1. 使能GPIOE时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    // 2. 配置GPIO
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // 3. 初始化
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void Key_EXTI_Init(void)
{
    /* 1. 使能GPIOE时钟 */
    __HAL_RCC_GPIOE_CLK_ENABLE();
  
    /* 2. 配置按键GPIO */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;      // PE3(KEY1), PE4(KEY0)
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;        // 下降沿触发中断
    GPIO_InitStruct.Pull = GPIO_PULLUP;                 // 内部上拉(按键接地)
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  
    /* 3. 使能SYSCFG时钟 */
    __HAL_RCC_AFIO_CLK_ENABLE();
    
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    
    /* 4. 配置NVIC */
    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);             // 设置优先级
    HAL_NVIC_SetPriority(EXTI4_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);                     // 使能EXTI3中断
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);                     // 使能EXTI4中断
}

uint8_t Key_return(void)
{
    // 按键扫描函数
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
    {  // 按键按下（低电平）
        Delay_ms(10);// 消抖延时
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET); // 等待松开
        }
        return KEY0;
    }
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET)
    {  // 按键按下（低电平）
        Delay_ms(10);// 消抖延时
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET); // 等待松开
        }
        return KEY1;
    }
    return 0;
}

void EXTI3_IRQHandler(void)
{
    LED1_ON();
}

void EXTI4_IRQHandler(void)
{
    LED0_ON();
}
