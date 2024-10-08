//
// Created by XFZAILDD on 2024/8/5.
//
#include <stdio.h>
#include <string.h>
#include "calibration.h"
#include "flash.h"
#include "tim.h"
#include "joystick.h"
#include "usart.h"

uint16_t recording_value = 0;

Joystick_ADC joystick_x_adc = {4096, 0, 0};
Joystick_ADC joystick_y_adc = {4096, 0, 0};

Trigger_ADC trigger_1_adc = {4096, 0};
Trigger_ADC trigger_2_adc = {4096, 0};


void FindMaxAndMin(int lastest_x, int lastest_y, int lastest_t1, int lastest_t2) {
    if (lastest_t1 > trigger_1_adc.max) {
        trigger_1_adc.max = lastest_t1;
    }
    if (lastest_t2 > trigger_2_adc.max) {
        trigger_2_adc.max = lastest_t2;
    }

    if (lastest_x < joystick_x_adc.min) {
        joystick_x_adc.min = lastest_x;
    }
    if (lastest_x > joystick_x_adc.max) {
        joystick_x_adc.max = lastest_x;
    }
    if (lastest_y < joystick_y_adc.min) {
        joystick_y_adc.min = lastest_y;
    }
    if (lastest_y > joystick_y_adc.max) {
        joystick_y_adc.max = lastest_y;
    }
    // printf("x_min:%u,x_max:%u,x_center:%u,y_min:%u,y_max:%u,y_center:%u\n",
    //        joystick_x_adc.min, joystick_x_adc.max, joystick_x_adc.center,
    //        joystick_y_adc.min, joystick_y_adc.max, joystick_y_adc.center);
}

void read_calibration_value() {
    uint32_t JOYSTICH_ADC_VALUE[12];
    FlashRead(&JOYSTICH_ADC_VALUE[0], 12);
    joystick_x_adc.min = JOYSTICH_ADC_VALUE[0];
    joystick_x_adc.max = JOYSTICH_ADC_VALUE[1];
    joystick_x_adc.center_min = JOYSTICH_ADC_VALUE[2];
    joystick_x_adc.center_max = JOYSTICH_ADC_VALUE[3];
    joystick_y_adc.min = JOYSTICH_ADC_VALUE[4];
    joystick_y_adc.max = JOYSTICH_ADC_VALUE[5];
    joystick_y_adc.center_min = JOYSTICH_ADC_VALUE[6];
    joystick_y_adc.center_max = JOYSTICH_ADC_VALUE[7];
    trigger_1_adc.min = JOYSTICH_ADC_VALUE[8];
    trigger_1_adc.max = JOYSTICH_ADC_VALUE[9];
    trigger_2_adc.min = JOYSTICH_ADC_VALUE[10];
    trigger_2_adc.max = JOYSTICH_ADC_VALUE[11];
}

void calibration_joystick() {
    HAL_TIM_Base_Stop_IT(&htim2);
    printf("Calibration Start\n");
    recording_value = 0;

    joystick_x_adc.min = 4096;
    joystick_x_adc.max = 0;

    joystick_y_adc.min = 4096;
    joystick_y_adc.max = 0;

    // joystick_x_adc.center_max = 0;
    // joystick_x_adc.center_min = 0;
    // joystick_y_adc.center_max = 0;
    // joystick_y_adc.center_min = 0;
    trigger_1_adc.max = 0;
    trigger_2_adc.max = 0;

    printf("请松开所有按钮与摇杆\n");
    HAL_Delay(1000);
    trigger_1_adc.min = dma_adc[2];
    trigger_2_adc.min = dma_adc[3];
    printf("请将摇杆向右推到底,然后慢慢放松到中间,准备好后按下:B按钮,重试请按下:A\n");
    HAL_Delay(1000);
    while (1) {
        if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == 0) {
            joystick_x_adc.center_min = dma_adc[0];
            printf("校准成功 joystick x adc center_min: %d\n", joystick_x_adc.center_min);
            break;
        }
        if (HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin) == 0) {
            printf("请将摇杆向右推到底,然后慢慢放松到中间,准备好后按下:B按钮,重试请按下:A按钮\n");
            HAL_Delay(1000);
        }
    }

    printf("请将摇杆向左推到底,然后慢慢放松到中间,准备好后按下:B按钮,重试请按下:A\n");
    HAL_Delay(1000);
    while (1) {
        if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == 0) {
            joystick_x_adc.center_max = dma_adc[0];
            printf("校准成功 joystick x adc center_max: %d\n", joystick_x_adc.center_max);
            break;
        }
        if (HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin) == 0) {
            printf("请将摇杆向左推到底,然后慢慢放松到中间,准备好后按下:B按钮,重试请按下:A按钮\n");
            HAL_Delay(1000);
        }
    }

    printf("请将摇杆向上推到底,然后慢慢放松到中间,准备好后按下:B按钮,重试请按下:A\n");
    HAL_Delay(1000);
    while (1) {
        if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == 0) {
            joystick_y_adc.center_min = dma_adc[1];
            printf("校准成功 joystick y adc center_min: %d\n", joystick_y_adc.center_min);
            break;
        }
        if (HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin) == 0) {
            printf("请将摇杆向上推到底,然后慢慢放松到中间,准备好后按下:B按钮,重试请按下:A按钮\n");
            HAL_Delay(1000);
        }
    }

    printf("请将摇杆向下推到底,然后慢慢放松到中间,准备好后按下:B按钮,重试请按下:A\n");
    HAL_Delay(1000);
    while (1) {
        if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == 0) {
            joystick_y_adc.center_max = dma_adc[1];
            printf("校准成功 joystick y adc center_max: %d\n", joystick_y_adc.center_max);
            break;
        }
        if (HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin) == 0) {
            printf("请将摇杆向下推到底,然后慢慢放松到中间,准备好后按下:B按钮,重试请按下:A按钮\n");
            HAL_Delay(1000);
        }
    }

    printf("请四周摇动摇杆,按下扳机到底,完成后按下:B按钮,重试请按下:A按钮\n");
    HAL_Delay(2000);
    while (1) {
        FindMaxAndMin(dma_adc[0], dma_adc[1], dma_adc[2], dma_adc[3]);
        if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == 0) {
            printf("校准成功\n");
            break;
        }
    }

    printf("B: save and exit X: exit \n");
    HAL_Delay(1000);
    while (1) {
        if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == 0) {
            printf("save\n");
            HAL_Delay(1000);
            break;
        }
        if (HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin) == 0) {
            printf("exit\n");
            HAL_Delay(1000);
            return;
        }
    }
    HAL_TIM_Base_Stop(&htim2);
    __disable_irq();
    uint32_t number[] = {
        joystick_x_adc.min, joystick_x_adc.max, joystick_x_adc.center_min, joystick_x_adc.center_max,
        joystick_y_adc.min, joystick_y_adc.max, joystick_y_adc.center_min, joystick_y_adc.center_max,
        trigger_1_adc.min, trigger_1_adc.max,
        trigger_2_adc.min, trigger_2_adc.max
    };
    char buffer[300];
    char str[] =
        "Calibration Successful : {\"id\":%ld,\"adc_x_min\":%u,\"adc_x_max\":%u,\"adc_x_center_min\":%u,\"adc_x_center_max\":%u,\"adc_y_min\":%u,\"adc_y_max\":%u,\"adc_y_center_min\":%u,\"adc_y_center_max\":%u,\"adc_t1_min\":%u,\"adc_t1_max\":%u,\"adc_t2_min\":%u,\"adc_t2_max\":%u}\n";
    sprintf(buffer,
            str,
            *(__IO uint32_t*)(0X1FFFF7E8),
            joystick_x_adc.min,
            joystick_x_adc.max,
            joystick_x_adc.center_min,
            joystick_x_adc.center_max,
            joystick_y_adc.min,
            joystick_y_adc.max,
            joystick_y_adc.center_min,
            joystick_y_adc.center_max,
            trigger_1_adc.min,
            trigger_1_adc.max,
            trigger_2_adc.min,
            trigger_2_adc.max);
    printf(buffer);
    FlashWrite(number, 12);
    __enable_irq();
    HAL_TIM_Base_Start_IT(&htim2);
    printf("Calibration End\n");
}
