#include "board.h"

void board_Init(void)
{
    //Delay_Init();
    LED_Init();
    Key_Init();
    //KeyEXTI_Init();
    board_BSP_USART_Init();
}
