//
// Created by XFZAILDD on 24-7-30.
//
#include "button.h"
#include <stdio.h>

Button buttons[BUTTON_NUM] = {
        {BUTTON1_GPIO_Port, BUTTON1_Pin, BUTTON_RELEASED, 0},
        {BUTTON2_GPIO_Port, BUTTON2_Pin, BUTTON_RELEASED, 0},
        {BUTTON3_GPIO_Port, BUTTON3_Pin, BUTTON_RELEASED, 0},
        {BUTTON4_GPIO_Port, BUTTON4_Pin, BUTTON_RELEASED, 0},
};

void KeyScan() {
    for (int i = 0; i < BUTTON_NUM; i++) {
        switch (buttons[i].state) {
            case BUTTON_RELEASED:
                if (HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin) == 0) {
                    buttons[i].state = BUTTON_DEBOUNCE;
                    buttons[i].debounce_time = HAL_GetTick();
                }
                break;
            case BUTTON_DEBOUNCE:
                if (HAL_GetTick() - buttons[i].debounce_time >= 10) { // 10ms去抖动时间
                    if (HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin) == 0) {
                        buttons[i].state = BUTTON_PRESSED;
                    } else {
                        buttons[i].state = BUTTON_RELEASED;
                    }
                }
                break;
            case BUTTON_PRESSED:
                if (HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin) == 1) {
                    buttons[i].state = BUTTON_DEBOUNCE;
                    buttons[i].debounce_time = HAL_GetTick();
                }
                break;
        }
    }
}