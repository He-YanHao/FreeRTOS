#include "BSP_KEY.h"


void BSP_KEY_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_0);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
}

uint8_t key_scan(void)
{
    /* 先读取按键引脚的电平 如果高电平，按键按下 */
    if(gpio_input_bit_get(GPIOA,GPIO_PIN_0) == SET) // 按键按下
    {
        /* 延迟消抖 */
        Delay_ms(20);
        if(gpio_input_bit_get(GPIOA,GPIO_PIN_0) == SET) // 再次检测按键是否按下
        {
            while(gpio_input_bit_get(GPIOA,GPIO_PIN_0) == SET); // 等待按键松开
            return 1;
        }
    }
    return 0;
}
