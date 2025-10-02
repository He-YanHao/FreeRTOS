#include "board_BSP_USART.h"

void board_BSP_USART_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);    //开启串口1的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);      // 开启端口时钟

    //IO口用作串口引脚要配置复用模式
    GPIO_PinAFConfig(BSP_USART_TX_PORT, BSP_USART_TX_AF_PIN, BSP_USART_AF);
    GPIO_PinAFConfig(BSP_USART_RX_PORT, BSP_USART_RX_AF_PIN, BSP_USART_AF);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin           = BSP_USART_TX_PIN;    //TX引脚
    GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_AF;        //IO口用作串口引脚要配置复用模式
    GPIO_InitStructure.GPIO_Speed         = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType         = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd          = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin           = BSP_USART_RX_PIN;    //RX引脚
    GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_AF;        //IO口用作串口引脚要配置复用模式
    GPIO_InitStructure.GPIO_Speed         = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType         = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd          = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    USART_InitTypeDef USART_InitStructure;
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate              = 115200;//设置波特率
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;//字节长度为8bit
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;//1个停止位
    USART_InitStructure.USART_Parity                = USART_Parity_No ;//没有校验位
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;//将串口配置为收发模式
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None; //不提供流控
    USART_Init(BSP_USART,&USART_InitStructure);//将相关参数初始化给串口1

    USART_ClearFlag(BSP_USART,USART_FLAG_RXNE);//初始配置时清除接受置位
    
    USART_Cmd(BSP_USART,ENABLE);    //开启串口1
}

void usart_send_data(uint8_t ucch)
{
    USART_SendData(BSP_USART, (uint8_t)ucch);

        // 等待发送数据缓冲区标志置位
    while( RESET == USART_GetFlagStatus(BSP_USART, USART_FLAG_TXE) ){}
}

void usart_send_String(uint8_t *ucstr)
{
      while(ucstr && *ucstr)  // 地址为空或者值为空跳出
      {
        usart_send_data(*ucstr++);
      }
}

#if !defined(__MICROLIB)
//不使用微库的话就需要添加下面的函数
#if (__ARMCLIB_VERSION <= 6000000)
//如果编译器是AC5  就定义下面这个结构体
struct __FILE
{
        int handle;
};
#endif

FILE __stdout;

//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
        x = x;
}
#endif

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
      USART_SendData(BSP_USART, (uint8_t)ch);

      while( RESET == USART_GetFlagStatus(BSP_USART, USART_FLAG_TXE) ){}

      return ch;
}
