//
// Created by XFZAILDD on 2024/8/5.
//
#include "uart_instruction.h"
#include "usart.h"
#include "data_fifo.h"
#include <string.h>
#include <stdio.h>

char rxBuffer[BUFFER_SIZE]; // 存储接收到的字符串
volatile uint16_t rxIndex = 0; // 当前接收索引
volatile uint32_t lastRxTime = 0; // 上次接收字符的时间

void ProcessReceivedCommand(char* command);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART1) {
        char receivedChar = rxBuffer[rxIndex]; // 获取接收到的字符
        lastRxTime = HAL_GetTick(); // 更新最后接收时间
        if (receivedChar == '\n') {
            // 如果接收到换行符，表示指令结束
            rxBuffer[rxIndex] = '\0'; // 将字符串结束符放入数组
            ProcessReceivedCommand(rxBuffer); // 在这里处理接收到的指令
            rxIndex = 0; // 重置缓冲区和索引
            memset(rxBuffer, 0, BUFFER_SIZE);
        }
        else {
            if (rxIndex < BUFFER_SIZE - 1) {
                rxIndex++;
            }
            else {
                rxIndex = 0; // 缓冲区溢出处理，可以在这里添加错误处理代码
            }
        }
        // 继续接收下一个字符
        HAL_UART_Receive_IT(huart, (uint8_t *)&rxBuffer[rxIndex], 1);
    }
}

// 假设的指令处理函数
void ProcessReceivedCommand(char* command) {
    if (strcmp(command, "0") == 0) {
        // 将数据写入FIFO
        handle_data data;
        for (int i = 0; i < BUTTON_NUM; ++i) {
            data.button_state[i] = buttons[i].state;
        }
        for (int i = 0; i < JOYSTICK_VALUE; ++i) {
            data.sliding_filter_dma_adc[i] = sliding_filter_dma_adc[i];
        }
        FIFO_Write(&data_fifo, &data);
    }
    else {
        // 未知指令
    }
}

/**
 * @brief 检查接收超时
 */
void CheckReceiveTimeout(void) {
    if (HAL_GetTick() - lastRxTime > TIMEOUT_MS && rxIndex > 0) {
        // 超时处理代码
        rxBuffer[rxIndex] = '\0'; // 终止未完成的指令

        // 在这里处理未完成的指令
        ProcessReceivedCommand(rxBuffer);

        // 重置缓冲区和索引
        rxIndex = 0;
        memset(rxBuffer, 0, BUFFER_SIZE);
    }
}

/**
 * @brief 初始化串口接收
 */
void uart_instruction_init() {
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&rxBuffer[rxIndex], 1);
}
