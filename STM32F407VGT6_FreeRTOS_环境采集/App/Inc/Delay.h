#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus //如果是C++编译器
extern "C" { //以下代码按C语言规则编译
#endif

#include "stm32f4xx.h"

void Delay_Init(void);//初始化SysTick定时器
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H */
