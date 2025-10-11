#include "main.h"

int main()
{
    Delay_Init();
    board_BSP_USART_Init();
    printf("board_BSP_USART_Init...\r\n");
    LED_Init();
    printf("LED_Init...\r\n");
    MyADC_Init();
    printf("MyADC_Init...\r\n");
    DS18B20_GPIO_Init();
    printf("DS18B20_Init...\r\n");
    OLED_Init();
    printf("OLED_Init...\r\n");
    OLED_ShowString(0, 0, (uint8_t*)"Temperature", 16, 1);
    OLED_ShowString(0, 32, (uint8_t*)"Sunlight", 16, 1);
    FreeRTOS_Start();
    while(1)
    {
        
    }
}
