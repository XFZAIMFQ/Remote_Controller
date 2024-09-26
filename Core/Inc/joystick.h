//
// Created by XFZAILDD on 24-7-30.
//
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "calibration.h"
#include "main.h"
#include "filter.h"

#define JOYSTICK_VALUE 4
#define DEADZONE_THRESHOLD1 100
#define DEADZONE_THRESHOLD2 50

extern uint16_t dma_adc[JOYSTICK_VALUE];
extern uint16_t sliding_filter_dma_adc[JOYSTICK_VALUE];
extern Sliding_Filter_TypeDef joysticks[JOYSTICK_VALUE];

void InitJoystick();
void SlidingFilterJoystickValue();
float ConvertToJoystickValue(int adc, Joystick_ADC* joystick);
float ConvertToTriggerValue(int adc, int min, int max, int deadzone_threshold);

#endif //JOYSTICK_H
