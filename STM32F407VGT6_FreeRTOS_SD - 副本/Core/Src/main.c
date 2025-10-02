#include "main.h"

int main()
{
    Delay_Init();
    board_BSP_USART_Init();
    printf("串口初始化完毕...\r\n");
    board_LED_Init();
    printf("LED初始化完毕...\r\n");
    uint8_t sd = board_SD_Init();
    if(sd == 0)
    {
        printf("SD初始化成功\r\n");
    } else
    {
        printf("SD初始化失败代码为%d\r\n",sd);
    }
//    uint8_t W[512] = {"ASDFGHJKL"};
//    uint8_t R[512] = {""};
//    SD_WriteSingleBlock(10,&W[0]);
//    printf("写数据为：%s\r\n",&W[0]);
//    SD_ReadSingleBlock(10,&R[0]);
//    printf("读数据为：%s\r\n",&R[0]);
    //FreeRTOS_Start();
    while(1)
    {
        
    }
}
