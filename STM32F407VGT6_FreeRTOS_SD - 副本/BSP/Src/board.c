#include "board.h"

void board_Init(void)
{
    Delay_Init();
    board_LED_Init();
    Key_Init();
    //KeyEXTI_Init();
    board_BSP_USART_Init();
    //board_SDIO_Init();
}
