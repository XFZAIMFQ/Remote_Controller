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


void find_MaxAndMin(int lastest_x, int lastest_y, int lastest_t1, int lastest_t2) {

    if (lastest_t1 > trigger_1_adc.max) {
        joystick_x_adc.max = lastest_t1;
    }
    if (lastest_t2 > trigger_2_adc.max) {
        joystick_x_adc.max = lastest_t2;
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
    uint32_t JOYSTICH_ADC_VALUE[6];
    Flash_Read(&JOYSTICH_ADC_VALUE[0], 6);
    joystick_x_adc.min = JOYSTICH_ADC_VALUE[0];
    joystick_x_adc.max = JOYSTICH_ADC_VALUE[1];
    joystick_x_adc.center = JOYSTICH_ADC_VALUE[2];
    joystick_y_adc.min = JOYSTICH_ADC_VALUE[3];
    joystick_y_adc.max = JOYSTICH_ADC_VALUE[4];
    joystick_y_adc.center = JOYSTICH_ADC_VALUE[5];
}

void calibration_joystick() {
    HAL_TIM_Base_Stop_IT(&htim2);
    printf("Calibration Start\n");
    recording_value = 0;

    joystick_x_adc.min = 4096;
    joystick_x_adc.max = 0;

    joystick_y_adc.min = 4096;
    joystick_y_adc.max = 0;

    joystick_x_adc.center = dma_adc[1];
    joystick_y_adc.center = dma_adc[0];

    trigger_1_adc.min = dma_adc[2];
    trigger_1_adc.max = 0;

    trigger_2_adc.min = dma_adc[3];
    trigger_2_adc.max = 0;

    while (1) {
        find_MaxAndMin(dma_adc[2], dma_adc[1], dma_adc[3], dma_adc[0]);
        if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == 0) {
            __disable_irq();
            uint32_t number[] = {
                    joystick_x_adc.min, joystick_x_adc.max, joystick_x_adc.center, joystick_y_adc.min,
                    joystick_y_adc.max, joystick_y_adc.center, trigger_1_adc.min, trigger_1_adc.max,
                    trigger_2_adc.min, trigger_2_adc.max
            };
            char buffer[150];
            char str[] = "Calibration Successful : {\"id\":%ld,\"adc_x_min\":%u,\"adc_x_max\":%u,\"adc_x_center\":%u,\"adc_y_min\":%u,\"adc_y_max\":%u,\"adc_y_center\":%u,\"adc_t1_min\":%u,\"adc_t1_max\":%u,\"adc_t2_min\":%u,\"adc_t2_max\":%u}\n";
            sprintf(buffer,
                    str,
                    *(__IO uint32_t *) (0X1FFFF7E8),
                    joystick_x_adc.min,
                    joystick_x_adc.max,
                    joystick_x_adc.center,
                    joystick_y_adc.min,
                    joystick_y_adc.max,
                    joystick_y_adc.center,
                    trigger_1_adc.min,
                    trigger_1_adc.max,
                    trigger_2_adc.min,
                    trigger_2_adc.max);
            HAL_UART_Transmit(&huart1, (uint8_t *) buffer, strlen(buffer), 1000);
            Flash_Write(number, 6);
            __enable_irq();
            HAL_TIM_Base_Start_IT(&htim2);
            break;
        }
    }
    printf("Calibration End\n");
}