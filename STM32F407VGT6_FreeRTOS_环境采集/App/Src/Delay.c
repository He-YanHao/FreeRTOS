#include "Delay.h"

static __IO uint32_t g_system_tick = 0;

/**
  * @brief 初始化SysTick定时器
  * @note 必须在延时函数调用前执行
  * @param 无
  * @retval 无
  */
void Delay_Init(void)
{
    /* NVIC Configuration */
#define NVIC_VTOR_MASK              0x3FFFFF80
#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x10000000 */
    SCB->VTOR  = (0x10000000 & NVIC_VTOR_MASK);
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    SCB->VTOR  = (0x08000000 & NVIC_VTOR_MASK);
#endif

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//选择SysTick时钟源
	SysTick->LOAD=0xFFFF; // 清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; // 开始计数
}

/**
  * @brief 阻塞式微秒延时
  * @param us: 延时的微秒数 (1~1000000)
  * @retval 无
  */
void Delay_us(uint32_t us)
{
    uint32_t ticks;//延时1us SysTick定时器对应的寄存器数值
    uint32_t told, tnow, tcnt = 0;

    // 计算需要的时钟数 = 延迟微秒数 * 每微秒的时钟数
    ticks = us * (SystemCoreClock / 1000000);

    // 获取当前的SysTick值
    told = SysTick->VAL;

    while (1)
    {
        // 重复刷新获取当前的SysTick值
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += SysTick->LOAD - tnow + told;

            told = tnow;

            // 如果达到了需要的时钟数，就退出循环
            if (tcnt >= ticks)
                break;
        }
    }
}

/**
 -  @brief  调用用内核的 systick 实现的毫秒延时
 -  @note   None
 -  @param  _ms:要延时的ms数
 -  @retval None
*/
void Delay_ms(uint32_t ms)
{
    Delay_us(ms * 1000);
}

void Delay_s(uint32_t s) 
{ 
    Delay_ms(s * 1000);
}
