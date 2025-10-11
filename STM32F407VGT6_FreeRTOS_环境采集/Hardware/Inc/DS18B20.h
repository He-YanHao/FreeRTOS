#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f4xx.h"
#include "Delay.h"
#include "board_BSP_USART.h"

#define DS18B20_RCC  RCC_AHB1Periph_GPIOC
#define DS18B20_Pin  GPIO_Pin_6
#define DS18B20_GPIO GPIOC
#define DS18B20_UP   GPIO_SetBits(DS18B20_GPIO,DS18B20_Pin)
#define DS18B20_LOW  GPIO_ResetBits(DS18B20_GPIO,DS18B20_Pin)
#define DS18B20_IN   GPIO_ReadInputDataBit(DS18B20_GPIO,DS18B20_Pin)

uint8_t DS18B20_GPIO_Init(void);
void DS18B20_OUT_Change(void);
void DS18B20_IN_Change(void);
uint8_t DS18B20_Check(void); //检测DS18B20是否存在  1不存在 0存在
uint8_t DS18B20_Read_Byte(void);//从DS18B20读取一个字节 返回读取到的字节数据
void DS18B20_Write_Byte(uint8_t dat);//写一个字节到DS18B20 dat为要写入的字节
void DS18B20_Start(void);//DS18B20开始温度转换
float DS18B20_GetTemperture(void);//从DS18B20得到温度值

#endif
