#ifndef __BOARD_SD_H
#define __BOARD_SD_H

#define SD_DEBUG
//DeBug开关 非DeBug开关取消上一行注释
#ifdef SD_DEBUG
#include "board_BSP_USART.h"
#endif

#include "stm32f4xx.h"
#include <stdbool.h>
#include "Delay.h"

// SDIO错误码定义（按功能分类排序）
#define SDIO_ERROR_NONE             0      // 无错误，操作成功
// 硬件初始化相关错误
#define SDIO_ERROR_INIT_FAILED      1      // SDIO初始化失败
#define SDIO_ERROR_CLOCK_FAILED     2      // 时钟配置失败
#define SDIO_ERROR_POWER_FAILED     3      // 电源配置失败
// 命令传输相关错误
#define SDIO_ERROR_CMD_FAILED       11     // 命令发送失败
#define SDIO_ERROR_CMD_TIMEOUT      12     // 命令传输超时
#define SDIO_ERROR_CMD_CRC          13     // 命令CRC校验错误
#define SDIO_ERR_CMD_RSP_MISMATCH   14     // 命令响应不匹配
// 数据传输相关错误

#define SDIO_ERROR_DATA_TIMEOUT     21     // 数据传输超时
#define SDIO_ERROR_DATA_CRC         22     // 数据CRC校验错误
// 协议和响应相关错误
#define SDIO_ERROR_RESPONSE         31     // 响应错误或超时
#define SDIO_ERROR_ACMD41_FAILED    32     // ACMD41初始化失败
// SD卡特定错误
#define SD_ERROR_VOLTAGE_MISMATCH   41     // 电压范围不匹配
#define SD_REQUEST_NOT_APPLICABLE   42     // 不支持高速度


// SD卡命令定义 - 完整版
#define CMD0    0       // GO_IDLE_STATE - 复位SD卡，进入空闲状态
#define CMD1    1       // SEND_OP_COND - MMC卡初始化命令（SD卡不使用）
#define CMD2    2       // ALL_SEND_CID - 获取CID（卡识别寄存器）
#define CMD3    3       // SEND_RELATIVE_ADDR - 获取RCA（相对卡地址）
#define CMD4    4       // SET_DSR - 设置驱动级寄存器（可选，很少使用）
#define CMD5    5       // IO_SEND_OP_COND - SDIO卡专用
#define CMD6    6       // SWITCH_FUNC - 切换卡功能（如总线宽度、高速模式）
#define CMD7    7       // SELECT/DESELECT_CARD - 选择/取消选择卡
#define CMD8    8       // SEND_IF_COND - 发送接口条件，检查SD卡版本
#define CMD9    9       // SEND_CSD - 获取CSD（卡特定数据寄存器）
#define CMD10   10      // SEND_CID - 获取CID（与CMD2相同，但需要RCA）
#define CMD11   11      // VOLTAGE_SWITCH - 电压切换命令
#define CMD12   12      // STOP_TRANSMISSION - 停止数据传输
#define CMD13   13      // SEND_STATUS - 发送卡状态
#define CMD15   15      // GO_INACTIVE_STATE - 进入非活动状态
#define CMD16   16      // SET_BLOCKLEN - 设置块长度（字节数）
#define CMD17   17      // READ_SINGLE_BLOCK - 读取单个块
#define CMD18   18      // READ_MULTIPLE_BLOCK - 读取多个块
#define CMD23   23      // SET_BLOCK_COUNT - 设置块计数（MMC卡用）
#define CMD24   24      // WRITE_BLOCK - 写入单个块
#define CMD25   25      // WRITE_MULTIPLE_BLOCK - 写入多个块
#define CMD27   27      // PROGRAM_CSD - 编程CSD寄存器
#define CMD28   28      // SET_WRITE_PROT - 设置写保护
#define CMD29   29      // CLR_WRITE_PROT - 清除写保护
#define CMD30   30      // SEND_WRITE_PROT - 发送写保护状态
#define CMD32   32      // ERASE_WR_BLK_START - 设置擦除起始地址
#define CMD33   33      // ERASE_WR_BLK_END - 设置擦除结束地址
#define CMD38   38      // ERASE - 擦除指定范围内的块
#define CMD41   41      // SD_SEND_OP_COND - SD卡初始化（ACMD41）
#define CMD42   42      // LOCK_UNLOCK - 锁定/解锁命令
#define CMD55   55      // APP_CMD - 应用命令前缀
#define CMD56   56      // GEN_CMD - 通用命令
#define CMD58   58      // READ_OCR - 读取OCR（操作条件寄存器）
#define CMD59   59      // CRC_ON_OFF - CRC校验开关

// 应用特定命令（Application Specific Commands）
// 注意：这些命令必须在CMD55之后发送
#define ACMD6   6       // 设置总线宽度
#define ACMD13  13      // 读取SD状态
#define ACMD22  22      // 发送已写入块数
#define ACMD23  23      // 设置预擦除块数
#define ACMD41  41      // SD卡初始化
#define ACMD42  42      // 设置/清除卡检测
#define ACMD51  51      // 读取SCR（SD配置寄存器）

//外部调用函数
uint8_t board_SD_Init(void);


//内部引用函数
void board_SDIO_GPIO_Init(void);//初始化GPIO
void board_SDIO_ClockConfig(void);//时钟配置
uint8_t SDIO_SwitchToHighSpeed(void);//切换到高速模式 (SD卡初始化完成后调用)
uint8_t SD_SendCommand(uint32_t cmd, uint32_t arg, uint8_t response_type);
//cmd 命令索引  arg 命令参数  response_type 响应类型  retval 错误代码
uint8_t SD_SendCommand_NoCRC(uint32_t cmd, uint32_t arg, uint8_t response_type);

#endif
