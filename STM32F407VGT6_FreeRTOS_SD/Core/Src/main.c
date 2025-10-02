#include "main.h"

int main()
{
    Delay_Init();
    board_BSP_USART_Init();
    printf("board_BSP_USART_Init...\r\n");
    board_LED_Init();
    printf("LED_Init...\r\n");
    uint8_t sd = board_SDIO_Init();
    if(sd == 0)
    {
        printf("SD_Init_succeed\r\n");
    }else
    {
        printf("SD初始化失败代码为%d\r\n",sd);
    }
    printf("SD状态代码为%d\r\n",SD_GetCardStatus());
    uint8_t W[512] = {"ASDFGHJKL"};
    uint8_t R[512] = {""};
    SD_WriteSingleBlock(10,&W[0]);
    printf("写数据为：%s\r\n",&W[0]);
    SD_ReadSingleBlock(10,&R[0]);
    printf("读数据为：%s\r\n",&R[0]);
    //FreeRTOS_Start();
    while(1)
    {
        
    }
}
