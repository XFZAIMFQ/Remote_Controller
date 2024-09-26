//
// Created by XFZAILDD on 24-7-30.
//
#include "joystick.h"
#include "adc.h"
#include "math.h"

uint16_t dma_adc[JOYSTICK_VALUE] = {0};
uint16_t sliding_filter_dma_adc[JOYSTICK_VALUE] = {0};
Sliding_Filter_TypeDef joysticks[JOYSTICK_VALUE];


void InitJoystick() {
    sliding_filter_init(joysticks, 10, 4);
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)dma_adc, 4);
}

void SlidingFilterJoystickValue() {
    for (int i = 0; i < JOYSTICK_VALUE; ++i) {
        sliding_filter_dma_adc[i] = sliding_filter(&joysticks[i], dma_adc[i]);
    }
}

float ConvertToJoystickValue(int adc, Joystick_ADC* joystick) {
    float value = 0;
    if (adc < joystick->center_min - DEADZONE_THRESHOLD1) {
        if (adc < joystick->min + DEADZONE_THRESHOLD2)
            return -1;
        value = (float)(adc - joystick->center_min - DEADZONE_THRESHOLD1) / (float)(joystick->center_min -
            joystick->min);
    }
    if (adc > joystick->center_max + DEADZONE_THRESHOLD1) {
        if (adc > joystick->max - DEADZONE_THRESHOLD2)
            return 1;
        value = (float)(adc - joystick->center_max - DEADZONE_THRESHOLD1) / (float)(joystick->max - joystick->
            center_max);
    }
    return value;
}

float ConvertToTriggerValue(int adc, int min, int max, int deadzone_threshold) {
    if (adc <= min + deadzone_threshold) {
        return 0.0f;
    }
    if (adc >= max - deadzone_threshold) {
        return 1.0f;
    }
    return sqrtf((float)(adc - min) / (float)(max - min));
}
