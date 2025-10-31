#include "board_LED.h"

void LED_Init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

    GPIO_InitTypeDef  GPIO_InitStructure;//创建结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOB,GPIO_Pin_2);
}

void LED_ON(void)
{
    // 配置PB2引脚转化代码为高电平
    GPIO_SetBits(GPIOB,GPIO_Pin_2);
}

void LED_OFF(void)
{
    // 配置PB2引脚输出低电平转化代码为
    GPIO_ResetBits(GPIOB,GPIO_Pin_2);
}

void LED_Toggle(void)
{
    //
    GPIO_ToggleBits(GPIOB,GPIO_Pin_2);
}
