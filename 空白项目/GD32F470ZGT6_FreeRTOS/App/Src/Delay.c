#include "Delay.h"

void delay()
{
    for(uint32_t i = 0; i < 70000; i++)
    {
//        for(uint32_t j = 0; j < 100000; j++)
//        {
//            
//        }
    }
}

void Delay_ms(uint32_t ms)
{
    while(ms)
    {
        delay();
        ms--;
    }
}
