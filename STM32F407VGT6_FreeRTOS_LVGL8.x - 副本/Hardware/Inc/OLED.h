#ifndef __OLED_H
#define __OLED_H 

#include "stm32f4xx.h"
#include "Delay.h"

//测试LED端口定义
#define LED_ON   GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define LED_OFF  GPIO_SetBits(GPIOC,GPIO_Pin_12)

//OLED端口定义
// SCK = PA5
// SDA = PA7
// RES = PA3
// DC  = PA2
// CS  = PA4

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_3)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_2)//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_2)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_4)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

/*工具指令*/
void OLED_Init(void);//初始化OLED
void OLED_Clear(void);//清屏函数
void OLED_ClearFresh(void);//清显存函数
void OLED_Refresh(void);//更新显存到OLED	
void OLED_ColorTurn(uint8_t i);//反显函数
void OLED_DisplayTurn(uint8_t i);//屏幕旋转180度
void OLED_DisPlay_On(void);//开启OLED显示 
void OLED_DisPlay_Off(void);//关闭OLED显示
/*画图指令*/
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);//画点 x:0~127 y:0~63 t:1 填充 0,清空
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);////画线 x1,y1:起点坐标 x2,y2:结束坐标
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);//x,y:圆心坐标 r:圆的半径
/*文字指令*/
//字体大小size仅有 0806字体 1206字体 1608字体 2412字体 通过8 12 16 24调用
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
//在指定位置显示一个字符,包括部分字符 
//x:0~127 y:0~63 size1:选择字体 6x8/6x12/8x16/12x24 mode:0,反色显示;1,正常显示
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode);
//显示字符串 x,y:起点坐标 *chr:字符串起始地址 size1:字体大小  mode:0,反色显示;1,正常显示
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
//显示数字 x,y:起点坐标 num:要显示的数字 len:数字的位数 size:字体大小 mode:0,反色显示;1,正常显示
void OLED_ShowFloatNum(uint8_t x,uint8_t y,float num,uint8_t len,uint8_t size1,uint8_t mode);
//显示数字 x,y:起点坐标 num:要显示的数字 len:数字的位数 size:字体大小 mode:0,反色显示;1,正常显示
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode);
//显示汉字 x,y:起点坐标 num:汉字对应的序号 mode:0,反色显示;1,正常显示
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode);
//num 显示汉字的个数 space 每一遍显示的间隔 mode:0,反色显示;1,正常显示
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
//x,y：起点坐标 sizex,sizey,图片长宽 BMP[]：要写入的图片数组 mode:0,反色显示;1,正常显示
/*其他指令*/
void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_ShowChar6x8(uint8_t x,uint8_t y,uint8_t chr,uint8_t mode);
void OLED_WR_Byte(uint8_t dat,uint8_t mode);//SPI发送数据或指令

#endif
