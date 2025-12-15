#include "main.h"

int main(void)
{
    MyUSART_Init();
    BSP_LED_ALL_Init();

    FreeRTOS_Start();
    while(1)
    {

    }
}
