//
// Created by XFZAILDD on 2024/8/5.
//
#ifndef USART_INSTRUCTION_H
#define USART_INSTRUCTION_H

#include "main.h"

#define BUFFER_SIZE 100  // 定义缓冲区大小
#define TIMEOUT_MS 1000  // 定义超时时间（毫秒）

void uart_instruction_init();
void CheckReceiveTimeout(void);

#endif //USART_INSTRUCTION_H
