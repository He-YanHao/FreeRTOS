#include "key.h"

/**
  * @brief  ��ȷ��ʱ1ms��72MHz��Ƶ��
  * @param  ��
  * @retval ��
  * @note ����72��ʱ������/΢����㣬��ʵ��У׼
  */
void Delay_1ms(void)
{
    volatile uint32_t delay = 72 * 1000 / 4;  // ���������Ż����������ֵ
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
    // 1. ʹ��GPIOEʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    // 2. ����GPIO
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // 3. ��ʼ��
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void Key_EXTI_Init(void)
{
    /* 1. ʹ��GPIOEʱ�� */
    __HAL_RCC_GPIOE_CLK_ENABLE();
  
    /* 2. ���ð���GPIO */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;      // PE3(KEY1), PE4(KEY0)
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;        // �½��ش����ж�
    GPIO_InitStruct.Pull = GPIO_PULLUP;                 // �ڲ�����(�����ӵ�)
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  
    /* 3. ʹ��SYSCFGʱ�� */
    __HAL_RCC_AFIO_CLK_ENABLE();
    
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    
    /* 4. ����NVIC */
    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);             // �������ȼ�
    HAL_NVIC_SetPriority(EXTI4_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);                     // ʹ��EXTI3�ж�
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);                     // ʹ��EXTI4�ж�
}

uint8_t Key_return(void)
{
    // ����ɨ�躯��
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
    {  // �������£��͵�ƽ��
        Delay_ms(10);// ������ʱ
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET); // �ȴ��ɿ�
        }
        return KEY0;
    }
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET)
    {  // �������£��͵�ƽ��
        Delay_ms(10);// ������ʱ
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET); // �ȴ��ɿ�
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
