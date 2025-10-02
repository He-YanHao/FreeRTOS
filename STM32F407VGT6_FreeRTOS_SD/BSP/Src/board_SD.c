#include "board_SD.h"
#include <string.h>

// 全局变量
static uint32_t SD_RCA = 0;  // 相对卡地址
static uint8_t card_type = 0;  // SD卡类型
static uint32_t timeout = 0xFFFF;//超时等待

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
    GPIO_InitStructureC.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructureC.GPIO_Mode = GPIO_Mode_AF;       //复用模式
    GPIO_InitStructureC.GPIO_Speed = GPIO_Speed_100MHz; //速度
    GPIO_InitStructureC.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructureC.GPIO_PuPd = GPIO_PuPd_UP;       //上拉
    GPIO_Init(GPIOC, &GPIO_InitStructureC);

    // 配置PD2为SDIO引脚
    GPIO_InitTypeDef GPIO_InitStructureD;
    GPIO_InitStructureD.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructureC.GPIO_Mode = GPIO_Mode_AF;       //复用模式
    GPIO_InitStructureC.GPIO_Speed = GPIO_Speed_100MHz; //速度
    GPIO_InitStructureC.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructureC.GPIO_PuPd = GPIO_PuPd_UP;       //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructureD);

    // 连接GPIO到SDIO复用功能
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_SDIO);  // PC8: SDIO_D0
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SDIO);  // PC9: SDIO_D1  
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO); // PC10: SDIO_D2
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO); // PC11: SDIO_D3
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO); // PC12: SDIO_CK
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_SDIO);  // PD2: SDIO_CMD
}

/**
  * @brief  SDIO时钟配置
  * @param  clock_div: 时钟分频系数
  * @retval None
  */
void board_SDIO_ClockConfig(uint8_t clock_div)
{
    // 使能SDIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, ENABLE);

    // 关闭SDIO时钟
    SDIO_ClockCmd(DISABLE);
    SDIO_InitTypeDef SDIO_InitStructure;
    // 配置SDIO参数
    SDIO_InitStructure.SDIO_ClockDiv = clock_div;
    SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
    SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
    SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
    SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;  //初始化时使用1位总线
    SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Enable;
    SDIO_Init(&SDIO_InitStructure);
    
    // 重新使能SDIO时钟
    SDIO_ClockCmd(ENABLE);
}

/**
  * @brief  切换到高速模式 (SD卡初始化完成后调用)
  * @param  None
  * @retval None
  */
void SDIO_SwitchToHighSpeed(void)
{
    // 切换到4位总线宽度和高速时钟
    SDIO_InitTypeDef SDIO_InitStructure;
    
    SDIO_ClockCmd(DISABLE);
    
    SDIO_InitStructure.SDIO_ClockDiv = SDIO_NORMAL_CLK_DIV;
    SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
    SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
    SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
    SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_4b;  // 切换到4位总线
    SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Enable;
    
    SDIO_Init(&SDIO_InitStructure);
    
    SDIO_ClockCmd(ENABLE);
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
    timeout = 0xFFFF;//超时等待
    // 清除所有标志
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND);
    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
    // 配置命令
    SDIO_CmdInitStructure.SDIO_Argument = arg;
    SDIO_CmdInitStructure.SDIO_CmdIndex = cmd;
    SDIO_CmdInitStructure.SDIO_Response = response_type;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    
    // 等待响应或超时
    while(timeout--)
    {
        if(SDIO_GetFlagStatus(SDIO_FLAG_CTIMEOUT) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
            return SDIO_ERROR_DATA_TIMEOUT;
        }
        
        if(SDIO_GetFlagStatus(SDIO_FLAG_CCRCFAIL) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
            return SDIO_ERROR_DATA_CRC;
        }
        
        if(SDIO_GetFlagStatus(SDIO_FLAG_CMDREND) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_CMDREND);
            break;
        }
    }
    if(timeout == 0)
    {
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
uint8_t board_SDIO_Init(void)
{
    uint8_t ret;       // 操作返回值
    uint32_t response; // 命令响应值
    //uint8_t card_type = 0; // 0:未知, 1:SDv1, 2:SDv2, 3:MMC
    printf("开始SD卡初始化...\r\n");

    /* 第一阶段：SDIO硬件初始化 */
    // 1. 初始化GPIO引脚
    board_SDIO_GPIO_Init();
    printf("SD卡GPIO初始化完毕...\r\n");

    // 2. 配置SDIO时钟（初始化阶段使用低速时钟）
    board_SDIO_ClockConfig(SDIO_INIT_CLK_DIV);
    printf("SDIO时钟初始化完成(400kHz)\r\n");

    // 3. 设置SDIO电源状态为开启
    SDIO_SetPowerState(SDIO_PowerState_ON);
    printf("SD卡电源确定开启...\r\n");
    Delay_ms(1); // 短暂延时确保电源稳定
    
    // 4. 检查电源状态是否成功开启
    if(SDIO_GetPowerState() != SDIO_PowerState_ON)
    {
        printf("检查电源状态是否成功开启\r\n");
        return SDIO_ERROR_POWER_FAILED; //电源配置失败
    }
    // 5. 检查时钟是否正常（确保没有命令正在传输）
    if(SDIO_GetFlagStatus(SDIO_FLAG_CMDACT) == SET)
    {
        printf("检查时钟是否正常（确保没有命令正在传输）\r\n");
        return SDIO_ERROR_CLOCK_FAILED; //时钟配置失败
    }

    /* 第二阶段：SD卡识别初始化流程 */
    // 6. 发送CMD0 - 使SD卡进入空闲状态（软件复位）
    ret = SD_SendCommand(CMD0, 0, SDIO_Response_No);
    if(ret != SDIO_ERROR_NONE)
    {
        printf("CMD0失败: %d\r\n", ret);
        return ret;
    }
    printf("CMD0成功\r\n");
    Delay_ms(10);// 短暂延时确保初始化稳定
    
    // 7. 发送CMD8 - 检查SD卡电压兼容性（2.7-3.6V）和模式支持
    printf("检测卡类型...\r\n");
    ret = SD_SendCommand(CMD8, 0x1AA, SDIO_Response_Short);
    if(ret == SDIO_ERROR_NONE)
    {
        response = SDIO_GetResponse(SDIO_RESP1);
        printf("CMD8响应: 0x%08lX\r\n", response);
        // 检查响应格式：应该是0x000001AA（支持2.7-3.6V）
        if((response & 0xFFF) == 0x1AA)
        {
            card_type = 2; // SDv2.0+
            printf("检测到SD卡版本2.0+\r\n");
        } else
        {
            // CMD8响应异常，按SDv1.x处理
            card_type = 1;
            printf("CMD8响应异常(0x%08lX)，按SDv1.x处理\r\n", response);
        }
    } else {
        // CMD8失败，可能是SDv1.x或MMC卡
        card_type = 1;
        printf("CMD8无响应，按SDv1.x或MMC卡处理\r\n");
    }

    /* 第三阶段：SD卡初始化循环 */
    // 9. 发送ACMD41初始化SD卡，带超时检测
    timeout = 500; // 减少超时时间到5秒
    for(int attempt = 0; attempt < 2; attempt++) {
        printf("初始化尝试 %d...\r\n", attempt + 1);
        
        for(timeout = 1000; timeout > 0; timeout--) {
            if(card_type == 1 || card_type == 2) {
                // SD卡流程
                printf("发送CMD55...\r\n");
                ret = SD_SendCommand(CMD55, 0, SDIO_Response_Short);
                if(ret != SDIO_ERROR_NONE) {
                    printf("❌ CMD55失败: %d\r\n", ret);
                    if(attempt == 0) {
                        printf("尝试MMC卡初始化...\r\n");
                        card_type = 3;
                        break;
                    }
                    return ret;
                }
                printf("✅ CMD55成功\r\n");
                
                // ACMD41
                uint32_t acmd41_arg = (card_type == 2) ? 0x40000000 : 0x00000000;
                printf("发送ACMD41(参数:0x%08lX)...\r\n", acmd41_arg);
                ret = SD_SendCommand(ACMD41, acmd41_arg, SDIO_Response_Short);
            } else {
                // MMC卡流程
                printf("发送CMD1...\r\n");
                ret = SD_SendCommand(1, 0x80FF8000, SDIO_Response_Short);
            }
            
            if(ret != SDIO_ERROR_NONE) {
                printf("❌ 初始化命令失败: %d\r\n", ret);
                return ret;
            }
            
            response = SDIO_GetResponse(SDIO_RESP1);
            printf("响应: 0x%08lX (超时剩余: %lu)\r\n", response, timeout);
            
            if(response & 0x80000000) {
                printf("✅ 卡初始化完成\r\n");
                break;
            }
            Delay_ms(10);
        }
        
        if(timeout > 0) break;
        
        if(attempt == 0 && card_type != 3) {
            printf("第一次尝试失败，切换到MMC卡模式重试...\r\n");
            card_type = 3;
            timeout = 1000;
        }
    }


    
    // 10. 检查初始化超时
    if(timeout == 0)
    {
        return SDIO_ERROR_DATA_TIMEOUT;
    }
    
    /* 第四阶段：SD卡识别和数据准备 */
    /* 第四阶段：卡识别和配置 */
    // 获取OCR检查是否支持高容量
    if(SD_SendCommand(CMD58, 0, SDIO_Response_Short) == SDIO_ERROR_NONE) {
        response = SDIO_GetResponse(SDIO_RESP1);
        if(response & 0x40000000) {
            printf("高容量SD卡(SDHC/SDXC)\r\n");
        }
    }
    // 11. 发送CMD2获取CID（卡识别寄存器）
    ret = SD_SendCommand(CMD2, 0, SDIO_Response_Long);
    if(ret != SDIO_ERROR_NONE) 
    {
        return ret;
    }
    printf("CID获取成功\r\n");
    // 12. 发送CMD3获取RCA（相对卡地址）
    ret = SD_SendCommand(CMD3, 0, SDIO_Response_Short);
    if(ret != SDIO_ERROR_NONE) 
    {
        return ret;
    }
    // 13. 从响应中提取RCA并保存
    response = SDIO_GetResponse(SDIO_RESP1);
    SD_RCA = (response >> 16) & 0xFFFF;
    printf("RCA分配: 0x%04X\r\n", SD_RCA);
    
    // 14. 发送CMD7选择卡，使其进入传输状态
    ret = SD_SendCommand(CMD7, SD_RCA << 16, SDIO_Response_Short);
    if(ret != SDIO_ERROR_NONE) 
    {
        return ret;
    }
    
    /* 第五阶段：数据传输配置 */
    // 设置4位总线宽度
    ret = SD_SendCommand(CMD55, SD_RCA << 16, SDIO_Response_Short);
    if(ret == SDIO_ERROR_NONE) {
        ret = SD_SendCommand(6, 2, SDIO_Response_Short); // ACMD6: 总线宽度=4位
        if(ret == SDIO_ERROR_NONE) {
            printf("4位总线宽度设置成功\r\n");
        }
    }
    // 15. 发送CMD16设置数据块长度为512字节
    ret = SD_SendCommand(CMD16, 512, SDIO_Response_Short);
    if(ret != SDIO_ERROR_NONE) 
    {
        return ret;
    }    
    // 16. 切换到高速模式（提高数据传输速率）
    SDIO_SwitchToHighSpeed();// 切换到高速时钟
    // 切换到高速模式
    printf("切换到高速模式完成\r\n");

    printf("SD卡初始化成功完成！\r\n");
    return SDIO_ERROR_NONE; // 初始化成功
}

/**
  * @brief  读取单个扇区
  * @param  sector: 扇区号
  * @param  buffer: 数据缓冲区
  * @retval 错误代码
  */
uint8_t SD_ReadSingleBlock(uint32_t sector, uint8_t *buffer)
{
    uint8_t ret;
    uint32_t timeout = 0xFFFFFF;
    
    // 配置数据路径
    SDIO_DataInitTypeDef SDIO_DataInitStructure;
    SDIO_DataInitStructure.SDIO_DataTimeOut = 0xFFFFFF;
    SDIO_DataInitStructure.SDIO_DataLength = 512;
    SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_512b;
    SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
    SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
    SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
    
    SDIO_DataConfig(&SDIO_DataInitStructure);
    
    // 发送读命令
    ret = SD_SendCommand(CMD17, sector, SDIO_Response_Short);
    if(ret != SDIO_ERROR_NONE) return ret;
    
    // 等待数据接收完成
    while(timeout--)
    {
        if(SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
            return SDIO_ERROR_DATA_TIMEOUT;
        }
        
        if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
            return SDIO_ERROR_DATA_CRC;
        }
        
        if(SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
            return SDIO_ERROR_DATA_CRC;
        }
        
        if(SDIO_GetFlagStatus(SDIO_FLAG_DATAEND) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_DATAEND);
            break;
        }
        
        // 读取数据到缓冲区
        if(SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)
        {
            *((uint32_t*)buffer) = SDIO_ReadData();
            buffer += 4;
        }
    }
    
    if(timeout == 0)
        return SDIO_ERROR_DATA_TIMEOUT;
    
    return SDIO_ERROR_NONE;
}

/**
  * @brief  写入单个扇区
  * @param  sector: 扇区号
  * @param  buffer: 数据缓冲区
  * @retval 错误代码
  */
uint8_t SD_WriteSingleBlock(uint32_t sector, const uint8_t *buffer)
{
    uint8_t ret;
    uint32_t timeout = 0xFFFFFF;
    uint32_t count;
    const uint32_t *pBuffer = (const uint32_t*)buffer;
    
    // 配置数据路径
    SDIO_DataInitTypeDef SDIO_DataInitStructure;
    SDIO_DataInitStructure.SDIO_DataTimeOut = 0xFFFFFF;
    SDIO_DataInitStructure.SDIO_DataLength = 512;
    SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_512b;
    SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
    SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
    SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
    
    SDIO_DataConfig(&SDIO_DataInitStructure);
    
    // 发送写命令
    ret = SD_SendCommand(CMD24, sector, SDIO_Response_Short);
    if(ret != SDIO_ERROR_NONE) return ret;
    
    // 写入数据
    for(count = 0; count < 128; count++)
    {
        while(SDIO_GetFlagStatus(SDIO_FLAG_TXFIFOHE) == RESET);
        SDIO_WriteData(*pBuffer);
        pBuffer++;
    }
    
    // 等待写入完成
    while(timeout--)
    {
        if(SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
            return SDIO_ERROR_DATA_TIMEOUT;
        }
        
        if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
            return SDIO_ERROR_DATA_CRC;
        }
        
        if(SDIO_GetFlagStatus(SDIO_FLAG_DATAEND) != RESET)
        {
            SDIO_ClearFlag(SDIO_FLAG_DATAEND);
            break;
        }
    }
    
    if(timeout == 0)
        return SDIO_ERROR_DATA_TIMEOUT;
    
    return SDIO_ERROR_NONE;
}

/**
  * @brief  获取SD卡状态
  * @param  None
  * @retval 卡状态
  */
uint8_t SD_GetCardStatus(void)
{
    uint8_t ret = SD_SendCommand(CMD13, SD_RCA << 16, SDIO_Response_Short);
    if(ret == SDIO_ERROR_NONE)
    {
        uint32_t response = SDIO_GetResponse(SDIO_RESP1);
        return (response >> 9) & 0x0F;  // 返回状态字段
    }
    return 0xFF;
}

/**
  * @brief  SD卡读写测试
  * @param  None
  * @retval None
  */
void SD_ReadWriteTest(void)
{
    uint8_t ret;
    uint8_t writeBuffer[512];
    uint8_t readBuffer[512];
    uint32_t i;
    
    // 初始化测试数据
    for(i = 0; i < 512; i++)
    {
        writeBuffer[i] = i & 0xFF;  // 填充测试数据
    }
    
    // 测试扇区号（选择一个靠后的扇区，避免覆盖重要数据）
    uint32_t testSector = 1000;
    
    // 写入测试
    ret = SD_WriteSingleBlock(testSector, writeBuffer);
    if(ret == SDIO_ERROR_NONE)
    {
        // 写入成功，进行读取验证
        ret = SD_ReadSingleBlock(testSector, readBuffer);
        if(ret == SDIO_ERROR_NONE)
        {
            // 比较数据
            if(memcmp(writeBuffer, readBuffer, 512) == 0)
            {
                // 测试成功
                // 这里可以通过串口打印成功信息
                // printf("SD Card Read/Write Test: PASSED!\r\n");
            }
            else
            {
                // 数据不匹配
                // printf("SD Card Read/Write Test: DATA MISMATCH!\r\n");
            }
        }
        else
        {
            // 读取失败
            // printf("SD Card Read Test: FAILED! Error: %d\r\n", ret);
        }
    }
    else
    {
        // 写入失败
        // printf("SD Card Write Test: FAILED! Error: %d\r\n", ret);
    }
}
