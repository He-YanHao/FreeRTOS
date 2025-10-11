#include "MyADC.h"

void MyADC_Init(void)
{
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE); // 使能时钟

    GPIO_InitTypeDef  GPIO_InitStructure;           // 配置结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;    //模拟输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);          // 初始化GPIO

    ADC_DeInit();//ADC复位
    //ADC公共配置
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    ADC_CommonInitStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;//禁用DMA访问模式
    ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;//设置ADC为独立模式
    ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div2;//预分频器两次采样之间的延迟为5个ADC时钟周期
    ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
    //两次采样之间的延迟为5个ADC时钟周期
    ADC_CommonInit(&ADC_CommonInitStruct);
    //ADC特定配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;   //用连续转换模式
    ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right; //数据对齐方式为右对齐
    ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;//禁用外部触发转换
    ADC_InitStruct.ADC_NbrOfConversion=1;//设置转换数量为1
    ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;//设置ADC分辨率为12位
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;//禁用扫描模式
    //ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_Init(ADC1, &ADC_InitStruct);
    printf("ADC_OK\n");

    // ADC1配置
    // 配置ADC通道
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);
    // 使能ADC DMA请求
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    printf("ADC DMA Request Enabled\n");
    //ADC1使能
    ADC_Cmd(ADC1, ENABLE);
    printf("ADC Enabled\n");
}

/**********************************************************
 * 函 数 名 称：Get_ADC_Value
 * 函 数 功 能：读取ADC值
 * 传 入 参 数：ADC_CHANNEL_x=要采集的通道
 * 函 数 返 回：测量到的值
 * 作       者：LC
 * 备       注：默认采样周期为15个ADC采样时间
**********************************************************/
uint16_t Get_ADC_Value(void)
{
    uint16_t adc_value = 0;//返回值
    //设置采集通道
    ADC_RegularChannelConfig( ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);
    //开始软件转换
    ADC_SoftwareStartConv(ADC1);
    // 等待转换结束
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
    //读取采样值
    adc_value = ADC_GetConversionValue(ADC1);
    //返回采样值
    return adc_value;
}
