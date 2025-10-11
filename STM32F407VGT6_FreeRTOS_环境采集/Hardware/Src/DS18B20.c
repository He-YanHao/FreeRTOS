#include "DS18B20.h"

//初始化
uint8_t DS18B20_GPIO_Init(void)
{
    RCC_AHB1PeriphClockCmd(DS18B20_RCC, ENABLE);//使能GPIOB时钟
    GPIO_InitTypeDef  GPIO_InitStructure;//创建结构体
    GPIO_InitStructure.GPIO_Pin = DS18B20_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DS18B20_GPIO, &GPIO_InitStructure);
    DS18B20_UP;//默认高
    return DS18B20_Check();
}

//输出模式
void DS18B20_OUT_Change(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DS18B20_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DS18B20_GPIO, &GPIO_InitStructure);
}

//输入模式
void DS18B20_IN_Change(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DS18B20_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DS18B20_GPIO, &GPIO_InitStructure);
}

uint8_t DS18B20_Check(void) //检测DS18B20是否存在  1不存在 0存在
{
    uint8_t timeout = 0;  //超时等待
    //复位DS18B20
    DS18B20_OUT_Change(); //设置为输出模式
    DS18B20_LOW;          //拉低DQ
    Delay_us(750);        //拉低750us
    /* 拉低至少480us */
    DS18B20_UP;
    Delay_us(15);         //15us
    DS18B20_IN_Change();  //设置为输入模式
    /* 拉高大约 15~60us */
    //等待拉低，拉低说明有应答
    while ( (DS18B20_IN) && timeout<200)
    {//应答会拉低总线，要么应答，要么超时，不然一直循环。
        timeout++;//超时判断
        Delay_us(1);
    };
    //设备未应答
    if(timeout>=200)//超时就是没应答
    {
        return 1;//返回1就是失败了
    }
    else
    {
        timeout = 0;//清除超时等待
    }
    //等待18B20释放总线
    while ( !(DS18B20_IN) &&timeout<240)
    {
        timeout++;//超时判断
        Delay_us(1);
    };
    //释放总线失败
    if(timeout>=240)
    {
        return 1;
    }
    return 0;
}

//从DS18B20读取一个字节 返回读取到的字节数据
uint8_t DS18B20_Read_Byte(void)
{
    uint8_t i=0,dat=0;
    for (i=0;i<8;i++)
    {
        DS18B20_OUT_Change();//设置为输入模式
        DS18B20_LOW;         //拉低
        Delay_us(2);
        DS18B20_UP;          //释放总线
        DS18B20_IN_Change(); //设置为输入模式
        Delay_us(12);
        dat>>=1;
        if( DS18B20_IN )
        {
            dat=dat|0x80;
        }
        Delay_us(50);
    }
    return dat;
}


//写一个字节到DS18B20 dat为要写入的字节
void DS18B20_Write_Byte(uint8_t dat)
{
    uint8_t i;
    DS18B20_OUT_Change();//设置输出模式
    for (i=0; i<8; i++)
    {
        if ( (dat&0x01) ) //写1
        {
            DS18B20_LOW;
            Delay_us(2);
            DS18B20_UP;
            Delay_us(60);
        }
        else //写0
        {
            DS18B20_LOW;//拉低60us
            Delay_us(60);
            DS18B20_UP;//释放总线
            Delay_us(2);
        }
    dat=dat>>1;//传输下一位
    }
}

//DS18B20开始温度转换
void DS18B20_Start(void)
{
    DS18B20_Check();            //查询是否有设备应答
    DS18B20_Write_Byte(0xcc);   //对总线上所有设备进行寻址
    DS18B20_Write_Byte(0x44);   //启动温度转换
}

float DS18B20_GetTemperture(void)//从DS18B20得到温度值
{
    uint16_t temp;
    uint8_t dataL=0,dataH=0;//数据高 数据低
    float value;            //温度
    DS18B20_Start();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);  //对总线上所有设备进行寻址
    DS18B20_Write_Byte(0xbe);  // 读取数据命令
    dataL=DS18B20_Read_Byte(); //LSB
    dataH=DS18B20_Read_Byte(); //MSB
    temp=(dataH<<8)+dataL;     //整合数据
    if(dataH&0X80)             //高位为1，说明是负温度
    {
        temp=(~temp)+1;
        value=temp*(-0.0625);
    }
    else
    {
        value=temp*0.0625;
    }
    return value;
}
