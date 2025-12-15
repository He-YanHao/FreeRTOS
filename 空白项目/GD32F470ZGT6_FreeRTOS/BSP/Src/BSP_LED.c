#include "BSP_LED.h"

void BSP_LED1_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOE);
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    BSP_LED1_OFF();
}

void BSP_LED2_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    BSP_LED2_OFF();
}

void BSP_LED3_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOG);
    gpio_mode_set(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    BSP_LED3_OFF();
}


void BSP_LED4_Init(void)
{
    //开始时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    /* 设置GPIO模式 */

    //GPIOA 输出模式 悬空模式，无上拉和下拉 PIN_1
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_5);
    //设置GPIO输出模式和速度
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
    BSP_LED4_OFF();
}

void BSP_LED_ALL_Init(void)
{
    BSP_LED1_Init();
    BSP_LED2_Init();
    BSP_LED3_Init();
    BSP_LED4_Init();
}

void BSP_LED1_ON(void)
{
    gpio_bit_set(GPIOE,GPIO_PIN_3);
}

void BSP_LED1_OFF(void)
{
    gpio_bit_reset(GPIOE,GPIO_PIN_3);
}

void BSP_LED1_TOGGLE(void)
{
    gpio_bit_toggle(GPIOE,GPIO_PIN_3);
}

void BSP_LED2_ON(void)
{
    gpio_bit_set(GPIOD,GPIO_PIN_7);
}

void BSP_LED2_OFF(void)
{
    gpio_bit_reset(GPIOD,GPIO_PIN_7);
}

void BSP_LED2_TOGGLE(void)
{
    gpio_bit_toggle(GPIOD,GPIO_PIN_7);
}

void BSP_LED3_ON(void)
{
    gpio_bit_set(GPIOG,GPIO_PIN_3);
}

void BSP_LED3_OFF(void)
{
    gpio_bit_reset(GPIOG,GPIO_PIN_3);
}

void BSP_LED3_TOGGLE(void)
{
    gpio_bit_toggle(GPIOG,GPIO_PIN_3);
}

void BSP_LED4_ON(void)
{
    gpio_bit_set(GPIOA,GPIO_PIN_5);
}

void BSP_LED4_OFF(void)
{
    gpio_bit_reset(GPIOA,GPIO_PIN_5);
}

void BSP_LED4_TOGGLE(void)
{
    gpio_bit_toggle(GPIOA,GPIO_PIN_5);
}

void BSP_LED_ALL_ON(void)
{
    BSP_LED1_ON();
    BSP_LED2_ON();
    BSP_LED3_ON();
    BSP_LED4_ON();
}

void BSP_LED_ALL_OFF(void)
{
    BSP_LED1_OFF();
    BSP_LED2_OFF();
    BSP_LED3_OFF();
    BSP_LED4_OFF();
}

void BSP_LED_ALL_TOGGLE(void)
{
    BSP_LED1_TOGGLE();
    BSP_LED2_TOGGLE();
    BSP_LED3_TOGGLE();
    BSP_LED4_TOGGLE();
}
