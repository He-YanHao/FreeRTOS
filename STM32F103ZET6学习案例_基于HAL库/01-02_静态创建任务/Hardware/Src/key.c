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

uint8_t Key_return(void)
{
    // 按键扫描函数
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
    {  // 按键按下（低电平）
        Delay_ms(15);// 消抖延时
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET); // 等待松开
        }
        return KEY0;
    }
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET)
    {  // 按键按下（低电平）
        Delay_ms(15);// 消抖延时
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET); // 等待松开
        }
        return KEY1;
    }
    return 0;
}
