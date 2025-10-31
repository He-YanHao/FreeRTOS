#include "main.h"

int main()
{
    board_BSP_USART_Init();
    printf("board_BSP_USART_Init...\r\n");
    LED_Init();
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,0,"LVGL",24,1);
    OLED_Refresh();
    printf("LED_Init...\r\n");
    FreeRTOS_Start();
    while(1)
    {
        
    }
}
