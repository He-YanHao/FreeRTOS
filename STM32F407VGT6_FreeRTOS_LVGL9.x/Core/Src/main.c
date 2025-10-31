#include "main.h"

int main()
{
    board_BSP_USART_Init();
    printf("board_BSP_USART_Init...\r\n");
    LED_Init();
    printf("LED_Init...\r\n");
    //FreeRTOS_Start();
    while(1)
    {
    }
}
