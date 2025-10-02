#ifndef __BOARD_SD_H
#define __BOARD_SD_H

#include "stm32f4xx.h"
#include <stdbool.h>
#include "board_BSP_USART.h"
#include "Delay.h"

// SDIO错误码定义
#define SDIO_ERROR_NONE             0      // 无错误，操作成功
#define SDIO_ERROR_INIT_FAILED      1      // SDIO初始化失败
#define SDIO_ERROR_CLOCK_FAILED     2      // 时钟配置失败
#define SDIO_ERROR_POWER_FAILED     3      // 电源配置失败
#define SDIO_ERROR_CMD_FAILED       4      // 命令发送失败
#define SDIO_ERROR_DATA_TIMEOUT     5      // 数据传输超时
#define SDIO_ERROR_DATA_CRC         6      // 数据CRC校验错误
#define SDIO_ERROR_RESPONSE         7      // 响应错误或超时

// SD卡命令定义
//#define SD_CMD0_GO_IDLE_STATE       0      // 复位SD卡，使其进入空闲状态
//#define SD_CMD8_SEND_IF_COND        8      // 发送接口条件，检查SD卡电压兼容性
//#define SD_CMD55_APP_CMD            55     // 告知SD卡下一个命令是应用特定命令
//#define SD_ACMD41_SD_APP_OP_COND    41     // 发送操作条件，激活卡初始化过程
//#define SD_CMD2_ALL_SEND_CID        2      // 获取所有卡的CID(卡识别寄存器)
//#define SD_CMD3_SEND_RELATIVE_ADDR  3      // 获取卡的相对地址(RCA)
//#define SD_CMD7_SELECT_DESELECT_CARD 7     // 选择/取消选择指定的卡
//#define SD_CMD16_SET_BLOCKLEN       16     // 设置数据块长度(字节)
//#define SD_CMD17_READ_SINGLE_BLOCK  17     // 读取单个数据块
//#define SD_CMD24_WRITE_SINGLE_BLOCK 24     // 写入单个数据块
//#define SD_CMD13_SEND_STATUS        13     // 获取卡状态

// SD卡命令定义
#define CMD0    0       // GO_IDLE_STATE - 复位SD卡
#define CMD1    1       // CMD1 for MMC
#define CMD2    2       // 获取CID（卡识别寄存器）
#define CMD3    3       // 获取RCA（相对卡地址）
#define CMD7    7       // 
#define CMD8    8       // SEND_IF_COND - 发送接口条件
#define CMD11   11      // 
#define CMD13   13      // 
#define CMD16   16      // SET_BLOCKLEN - 设置块长度
#define CMD17   17      // READ_SINGLE_BLOCK - 读取单个块
#define CMD24   24      // WRITE_BLOCK - 写入单个块
#define CMD55   55      // APP_CMD - 应用命令前缀
#define CMD58   58      // READ_OCR - 读OCR寄存器
#define ACMD41  41      // SD_SEND_OP_COND - 开始卡初始化

// SD卡类型
#define SD_TYPE_MMC    0x01
#define SD_TYPE_V1     0x02
#define SD_TYPE_V2     0x04
#define SD_TYPE_V2HC   0x06

// 响应类型
#define R1_READY_STATE         0x00
#define R1_IDLE_STATE          0x01
#define R1_ILLEGAL_COMMAND     0x04

// SD卡状态定义
#define SD_CARD_READY              0      // 卡就绪状态
#define SD_CARD_IDENTIFICATION     1      // 识别状态，卡正在响应复位命令
#define SD_CARD_STANDBY            2      // 待机状态，卡已被识别但未被选中
#define SD_CARD_TRANSFER           3      // 传输状态，卡被选中，可以进行数据传输

// SDIO时钟配置
#define SDIO_INIT_CLK_DIV           0x76  // 初始化时钟分频 (400kHz左右)
#define SDIO_NORMAL_CLK_DIV         0x00  // 正常工作时钟分频 (最高速度)

// 初始化函数声明
void board_SDIO_GPIO_Init(void);//SDIO GPIO初始化
void board_SDIO_ClockConfig(uint8_t clock_div);//SDIO时钟配置
void SDIO_SwitchToHighSpeed(void);//切换到高速模式 (SD卡初始化完成后调用)
uint8_t SD_SendCommand(uint32_t cmd, uint32_t arg, uint8_t response_type);
//cmd: 命令索引  arg: 命令参数  response_type: 响应类型
uint8_t board_SDIO_Init(void);//SDIO完整初始化函数

// SD卡操作函数
uint8_t SD_ReadSingleBlock(uint32_t sector, uint8_t *buffer);//读取单个扇区
//sector 扇区号   buffer 数据缓冲区
uint8_t SD_WriteSingleBlock(uint32_t sector, const uint8_t *buffer);//写入单个扇区
//sector 扇区号   buffer 数据缓冲区
uint8_t SD_GetCardStatus(void);//获取SD卡状态

// 测试函数
void SD_ReadWriteTest(void);//SD卡读写测试


#endif
