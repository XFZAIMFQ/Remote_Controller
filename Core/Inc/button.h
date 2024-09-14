//
// Created by XFZAILDD on 24-7-30.
//
#ifndef BUTTON_H
#define BUTTON_H

#include "main.h"

#define BUTTON_NUM 4


typedef enum {
    BUTTON_RELEASED = 0,    //按键释放
    BUTTON_PRESSED,     //按键按下
    BUTTON_DEBOUNCE     //按键抖动
} ButtonState;

typedef struct {
    GPIO_TypeDef *port;     // 按键端口
    uint16_t pin;           // 按键引脚
    ButtonState state;      // 按键状态
    uint32_t debounce_time; // 按键按下时间
} Button;

// 按键数组
extern Button buttons[BUTTON_NUM];

void KeyScan();

#endif // BUTTON_H
