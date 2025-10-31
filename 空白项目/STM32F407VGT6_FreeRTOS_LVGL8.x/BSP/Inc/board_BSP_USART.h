#ifndef __BOARD_BSP_USART_H
#define __BOARD_BSP_USART_H

#include "stm32f4xx.h"
#include <stdio.h>  // 添加这行

#define BSP_USART_RCC       RCC_APB2Periph_USART1
#define BSP_USART_TX_RCC    RCC_AHB1Periph_GPIOA
#define BSP_USART_RX_RCC    RCC_AHB1Periph_GPIOA
#define BSP_USART               USART1
#define BSP_USART_TX_PORT       GPIOA
#define BSP_USART_TX_PIN        GPIO_Pin_9
#define BSP_USART_RX_PORT       GPIOA
#define BSP_USART_RX_PIN        GPIO_Pin_10
#define BSP_USART_AF            GPIO_AF_USART1
#define BSP_USART_TX_AF_PIN     GPIO_PinSource9
#define BSP_USART_RX_AF_PIN     GPIO_PinSource10

void board_BSP_USART_Init(void);
void usart_send_data(uint8_t ucch);
void usart_send_String(uint8_t *ucstr);

#endif
