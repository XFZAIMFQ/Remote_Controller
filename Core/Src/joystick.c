//
// Created by XFZAILDD on 24-7-30.
//
#include "joystick.h"
#include "adc.h"

uint16_t dma_adc[JOYSTICK_VALUE] = {0};
uint16_t sliding_filter_dma_adc[JOYSTICK_VALUE] = {0};
Sliding_Filter_TypeDef joysticks[JOYSTICK_VALUE];


void init_joystick() {
    sliding_filter_init(joysticks, 10, 4);
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *) dma_adc, 4);
}

void sliding_filter_joystick_value() {
    for (int i = 0; i < JOYSTICK_VALUE; ++i) {
        sliding_filter_dma_adc[i] = sliding_filter(&joysticks[i], dma_adc[i]);
    }
}

float ConvertToJoystickValue(int adc, int min, int max, int center) {
    if (adc < center - DEADZONE_THRESHOLD) {
        if (adc < min + DEADZONE_THRESHOLD)
            return -1;
        return (float) (adc - center) / (center - min);
    } else if (adc > center + DEADZONE_THRESHOLD) {
        if (adc > max - DEADZONE_THRESHOLD)
            return 1;
        return (float) (adc - center) / (max - center);
    } else {
        return 0;
    }
}

float ConvertToTriggerValue(int adc, int min, int max) {
    return ConvertToJoystickValue(adc, 0, max, min);

}