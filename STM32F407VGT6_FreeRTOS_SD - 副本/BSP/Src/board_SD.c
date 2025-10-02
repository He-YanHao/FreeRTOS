#include "board_SD.h"
#include <string.h>

/**
  * @brief  SDIO GPIO初始化
  * @note   配置SDIO相关的GPIO引脚为复用功能
  * @param  None
  * @retval None
  */
void board_SDIO_GPIO_Init(void)
{
    // 使能GPIOC和GPIOD时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructureC;
    // 配置PC8~PC12为SDIO引脚
    GPIO_InitStructureC.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructureC.GPIO_Mode  = GPIO_Mode_AF;       //复用模式
    GPIO_InitStructureC.GPIO_Speed = GPIO_Speed_100MHz; //速度
    GPIO_InitStructureC.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructureC.GPIO_PuPd  = GPIO_PuPd_UP;       //上拉
    GPIO_Init(GPIOC, &GPIO_InitStructureC);

    // 配置PD2为SDIO引脚
    GPIO_InitTypeDef GPIO_InitStructureD;
    GPIO_InitStructureD.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructureD.GPIO_Mode  = GPIO_Mode_AF;       //复用模式
    GPIO_InitStructureD.GPIO_Speed = GPIO_Speed_100MHz; //速度
    GPIO_InitStructureD.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructureD.GPIO_PuPd  = GPIO_PuPd_UP;       //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructureD);

    // 连接GPIO到SDIO复用功能
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8,  GPIO_AF_SDIO);  // PC8: SDIO_D0
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,  GPIO_AF_SDIO);  // PC9: SDIO_D1  
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO); // PC10: SDIO_D2
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO); // PC11: SDIO_D3
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO); // PC12: SDIO_CK
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2,  GPIO_AF_SDIO);  // PD2: SDIO_CMD
}

/**
  * @brief  SDIO时钟配置 - 固定为初始化速度(400kHz)
  * @param  None
  * @retval None
  */
void board_SDIO_ClockConfig(void)
{
    // 使能SDIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, ENABLE);
    // 关闭SDIO时钟
    SDIO_ClockCmd(DISABLE);
    SDIO_InitTypeDef SDIO_InitStructure;
    // 配置SDIO参数 - 固定为初始化速度400kHz
    SDIO_InitStructure.SDIO_ClockDiv = 0x76;        // 400kHz分频系数
    SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
    SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
    SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
    SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;  // 初始化时使用1位总线
    SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Enable;
    SDIO_Init(&SDIO_InitStructure);    
    // 重新使能SDIO时钟
    SDIO_ClockCmd(ENABLE);
}

/**
  * @brief  切换到高速模式 (SD卡初始化完成后调用)
  * @param  None
  * @retval SD_Error: SD卡错误代码
  */
uint8_t SDIO_SwitchToHighSpeed(void)
{
    /* 检查SD卡是否支持高速模式 */
    if(((SDIO->RESP1 >> 30) & 0x1) == 0) 
    {
        return SD_REQUEST_NOT_APPLICABLE;//不支持高速模式
    }
    /* 禁用SDIO时钟 */
    SDIO_ClockCmd(DISABLE);
    SDIO_InitTypeDef SDIO_InitStructure;
    /* 配置SDIO时钟为高速模式 */
    SDIO_InitStructure.SDIO_ClockDiv = 0x00;// 使用传输时钟分频 最高速度
    SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
    SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Enable;//旁路分频器，直接使用HCLK
    SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
    SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_4b;
    SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Enable;
    SDIO_Init(&SDIO_InitStructure);
    /* 重新使能SDIO时钟 */
    SDIO_ClockCmd(ENABLE);
    /* 等待时钟稳定 */
    uint32_t timeout = 0xFFFF;
    while((SDIO->CLKCR & SDIO_CLKCR_CLKEN) != SDIO_CLKCR_CLKEN)
    {
        if(timeout-- == 0)
        {
            return SDIO_ERROR_CLOCK_FAILED;
        }
    }
    /* 可选：发送CMD6切换到高速模式 */
    /* 注意：实际应用中需要根据SD卡版本和容量类型决定是否需要发送CMD6 */
    return SDIO_ERROR_NONE;
}

/**
  * @brief  发送SD卡命令
  * @param  cmd: 命令索引
  * @param  arg: 命令参数
  * @param  response_type: 响应类型
  * @retval 错误代码
  */
uint8_t SD_SendCommand(uint32_t cmd, uint32_t arg, uint8_t response_type)
{
    uint32_t timeout = 0xFFFF;//超时等待
    // 清除所有标志
    SDIO_ClearFlag((SDIO_FLAG_CCRCFAIL|SDIO_FLAG_CTIMEOUT|SDIO_FLAG_CMDREND|SDIO_FLAG_CMDSENT));
    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
    // 配置命令
    SDIO_CmdInitStructure.SDIO_Argument = arg;
    SDIO_CmdInitStructure.SDIO_CmdIndex = cmd;
    SDIO_CmdInitStructure.SDIO_Response = response_type;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    // 等待响应或超时
    while (timeout > 0)
    {
        // 检查命令超时
        if (SDIO_GetFlagStatus(SDIO_FLAG_CTIMEOUT) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
            return SDIO_ERROR_CMD_TIMEOUT; // 明确表示命令超时
        }
        // 检查CRC失败
        if (SDIO_GetFlagStatus(SDIO_FLAG_CCRCFAIL) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
            if(cmd == 41)
            {
                #ifdef SD_DEBUG
                    printf("发现CRC错误，但是不管它，视为成功...\r\n");
                #endif
                return SDIO_ERROR_NONE;//忽略CRC错误
            } else
            {
                return SDIO_ERROR_CMD_CRC; // 明确表示命令CRC失败
            }
        }
        // 根据响应类型检查不同的完成标志
        if (response_type == SDIO_Response_No)
        {
            // 无响应命令：检查CMDSENT标志
            if (SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) != RESET)
            {
                SDIO_ClearFlag(SDIO_FLAG_CMDSENT);
                return SDIO_ERROR_NONE;
            }
        } else
        {
            // 有响应命令：检查CMDREND标志
            if (SDIO_GetFlagStatus(SDIO_FLAG_CMDREND) != RESET)
            {
                SDIO_ClearFlag(SDIO_FLAG_CMDREND);
                // 验证响应命令索引是否匹配
                if (SDIO_GetCommandResponse() != cmd)
                {
                    return SDIO_ERR_CMD_RSP_MISMATCH;
                }
                return SDIO_ERROR_NONE;
            }
        }
        timeout--;
    }
    if(timeout == 0)
    {
        return SDIO_ERROR_DATA_TIMEOUT;
    }
    return SDIO_ERROR_NONE;
}

/**
  * @brief  发送SD命令（忽略CRC错误）
  * @param  cmd: 命令索引
  * @param  arg: 命令参数
  * @param  response_type: 响应类型
  * @retval 错误码
  * @note   专门用于ACMD41等需要忽略CRC检查的命令
  */
uint8_t SD_SendCommand_NoCRC(uint32_t cmd, uint32_t arg, uint8_t response_type)
{
    uint32_t timeout = 0xFFFF;//超时等待
    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
    // 配置命令
    SDIO_CmdInitStructure.SDIO_Argument = arg;
    SDIO_CmdInitStructure.SDIO_CmdIndex = cmd;
    SDIO_CmdInitStructure.SDIO_Response = response_type;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    // 清除所有标志
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT|SDIO_FLAG_CCRCFAIL|SDIO_FLAG_CMDSENT|SDIO_FLAG_CMDREND);
    // 等待响应或超时
    while (timeout--)
    {
        // 检查命令超时
        if (SDIO_GetFlagStatus(SDIO_FLAG_CTIMEOUT) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
            #ifdef SD_DEBUG
                printf("SD_SendCommand_NoCRC命令传输超时（硬件超时）...\r\n");
            #endif
            return SDIO_ERROR_CMD_TIMEOUT; // 明确表示命令超时
        }
        // 检查CRC失败
        if (SDIO_GetFlagStatus(SDIO_FLAG_CCRCFAIL) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
            #ifdef SD_DEBUG
                printf("发现CRC错误，但是不管它，视为成功...\r\n");
            #endif
            return SDIO_ERROR_NONE;
        }
        // 根据响应类型检查不同的完成标志
        if (response_type == SDIO_Response_No)
        {
            // 无响应命令：检查CMDSENT标志
            if (SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) != RESET)
            {
                SDIO_ClearFlag(SDIO_FLAG_CMDSENT);
                #ifdef SD_DEBUG
                    printf("无响应命令：检查CMDSENT标志成功...\r\n");
                #endif
                return SDIO_ERROR_NONE;
            }
        } else
        {
            // 有响应命令：检查CMDREND标志
            if (SDIO_GetFlagStatus(SDIO_FLAG_CMDREND) != RESET)
            {
                SDIO_ClearFlag(SDIO_FLAG_CMDREND);
                // 验证响应命令索引是否匹配
                if (SDIO_GetCommandResponse() != cmd)
                {
                    #ifdef SD_DEBUG
                        printf("验证响应命令索引是否匹配失败...\r\n");
                    #endif
                    return SDIO_ERR_CMD_RSP_MISMATCH;
                }
            return SDIO_ERROR_NONE;
            }
        }
    }
    if(timeout == 0)
    {
        #ifdef SD_DEBUG
            printf("SD_SendCommand_NoCRC命令传输超时（软件计数超时）...\r\n");
        #endif
        return SDIO_ERROR_DATA_TIMEOUT;
    }
    return SDIO_ERROR_NONE;
}

/**
  * @brief  SDIO完整初始化函数
  * @note   此函数供main函数直接调用，完成SDIO所有初始化工作
  * @param  None
  * @retval 错误代码 (0表示成功)
  */
uint8_t board_SD_Init(void)
{
    uint32_t timeout    = 0xFFFF;         //超时等待
    uint8_t  result     = SDIO_ERROR_NONE;//操作结果
    uint64_t response   = 0;//
    //uint8_t  cmd_result = 0;
    #ifdef SD_DEBUG
        printf("开始SD卡初始化...\r\n");
    #endif
    /* 第一阶段：初始化硬件 */
    // 1. 初始化GPIO口
    board_SDIO_GPIO_Init();      // 初始化SDIO GPIO引脚
    #ifdef SD_DEBUG
        printf("初始化SDIO的GPIO引脚完毕\r\n");
    #endif
    Delay_ms(10);//等待稳定
    // 2. 配置低速时钟
    board_SDIO_ClockConfig();    // 配置SDIO时钟
    #ifdef SD_DEBUG
        printf("配置SDIO时钟完毕\r\n");
    #endif
    Delay_ms(10);//等待稳定
    // 3. 设置SDIO电源状态为开启
    SDIO_SetPowerState(SDIO_PowerState_ON);
    #ifdef SD_DEBUG
        printf("开启SD卡电源...\r\n");
    #endif
    Delay_ms(10); // 短暂延时确保电源稳定
    // 4. 检查电源状态是否成功开启
    #ifdef SD_DEBUG
        printf("检查电源状态确认是否成功开启\r\n");
    #endif
    if(SDIO_GetPowerState() != SDIO_PowerState_ON)
    {
        printf("电源配置失败\r\n");
        return SDIO_ERROR_POWER_FAILED; //电源配置失败
    }
    // 5. 检查时钟是否正常（确保没有命令正在传输）
    #ifdef SD_DEBUG
        printf("检查时钟是否正常\r\n");
    #endif
    if(SDIO_GetFlagStatus(SDIO_FLAG_CMDACT) == SET)
    {
        printf("时钟配置失败\r\n");
        return SDIO_ERROR_CLOCK_FAILED; //时钟配置失败
    }
    
    /* 第二阶段：SD卡识别初始化流程 */
    // 6. 发送CMD0 - 使SD卡进入空闲状态（软件复位）
    result = SD_SendCommand(CMD0, 0, SDIO_Response_No);
    if(result != SDIO_ERROR_NONE)
    {
        printf("CMD0失败: %d\r\n", result);
        return result;
    }
    #ifdef SD_DEBUG
        printf("CMD0成功\r\n");
    #endif
    Delay_ms(5);
    // 7. 检查电压范围
    result = SD_SendCommand(CMD8, 0x000001AA, SDIO_Response_Short);
    if(result != SDIO_ERROR_NONE)
    {
        printf("CMD8失败: %d\r\n", result);
        #ifdef SD_DEBUG
            printf("可能是SD V1.x或非SD设备\r\n");
        #endif
    }
    else
    {
        #ifdef SD_DEBUG
            printf("CMD8成功，检测到SD V2.0+\r\n");
        #endif
        // 验证电压范围匹配
        response = SDIO_GetResponse(SDIO_RESP1);
        if((response & 0xFFF) != 0x1AA)//检查返回值的低12位是否为0x1AA
        {
            printf("电压范围不匹配,响应:%d\n",response);
            return SD_ERROR_VOLTAGE_MISMATCH;
        }
        #ifdef SD_DEBUG
            printf("电压范围验证通过\r\n");
        #endif
    }
    // 8. 应用命令
    timeout = 0x1;      //
    do {
        result = SD_SendCommand(CMD58, 0, SDIO_Response_Long);
        if (result != SDIO_ERROR_NONE)
        {
        uint32_t ocr = SDIO_GetResponse(SDIO_RESP1);
        #ifdef SD_DEBUG
            printf("OCR: 0x%08lX\r\n", ocr);
            printf("电压支持: %s%s%s%s\r\n",
                   (ocr & (1<<23)) ? "3.2-3.3V " : "",
                   (ocr & (1<<22)) ? "3.1-3.2V " : "",
                   (ocr & (1<<21)) ? "3.0-3.1V " : "",
                   (ocr & (1<<20)) ? "2.9-3.0V " : "");
            printf("卡容量: %s\r\n", (ocr & (1<<30)) ? "SDHC/SDXC" : "SDSC");
            printf("上电状态: %s\r\n", (ocr & (1<<31)) ? "就绪" : "初始化中");
        }
        #endif
        result = SD_SendCommand(CMD55, 0x00000000, SDIO_Response_Short);
        #ifdef SD_DEBUG
            printf("CMD55返回值%d\r\n",result);
        #endif
        if (result != SDIO_ERROR_NONE)
        {
            return SDIO_ERR_CMD_RSP_MISMATCH;
        }
        result = SD_SendCommand_NoCRC(ACMD41, 0x80FF8000, SDIO_Response_Long);
        if (result != SDIO_ERROR_NONE)//接受并判断返回值
        {
            #ifdef SD_DEBUG
                printf("ACMD41返回值失败\r\n");
                printf("ACMD41返回值%d\r\n",result);
            #endif
            return SDIO_ERROR_ACMD41_FAILED;  // ACMD41失败
        }
        response = SDIO_GetResponse(SDIO_RESP1);
        #ifdef SD_DEBUG
            printf("response:0x%08lX\r\n",response);
            printf("SDIO状态寄存器: 0x%08lX\r\n", SDIO->STA);
            printf("SDIO电源状态: %d\r\n", SDIO_GetPowerState());
            printf("=== SD卡初始化详细调试信息 ===\r\n");
    printf("SDIO->POWER: 0x%08lX\r\n", SDIO->POWER);
    printf("SDIO->CLKCR: 0x%08lX\r\n", SDIO->CLKCR);
    printf("SDIO->ARG: 0x%08lX\r\n", SDIO->ARG);
    printf("SDIO->CMD: 0x%08lX\r\n", SDIO->CMD);
    printf("SDIO->RESP1: 0x%08lX\r\n", SDIO->RESP1);
        #endif
        if (timeout-- == 0)
        {
            #ifdef SD_DEBUG
                printf("ACMD41超时失败\r\n");
            #endif
            return SDIO_ERROR_CMD_TIMEOUT;
        }
        Delay_ms(100);
    } while ((response & 0x80000000) == 0); // 等待卡就绪

    // 4. 发送CMD2（获取CID）
    SD_SendCommand(CMD2, 0, SDIO_Response_Long);

    // 5. 发送CMD3（获取RCA）  
    SD_SendCommand(CMD3, 0, SDIO_Response_Short);
    return SDIO_ERROR_NONE;//
}

