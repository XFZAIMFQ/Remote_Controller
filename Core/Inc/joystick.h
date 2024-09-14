//
// Created by XFZAILDD on 24-7-30.
//
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "main.h"
#include "filter.h"

#define JOYSTICK_VALUE 4
#define DEADZONE_THRESHOLD 100

extern uint16_t dma_adc[JOYSTICK_VALUE];
extern uint16_t sliding_filter_dma_adc[JOYSTICK_VALUE];
extern Sliding_Filter_TypeDef joysticks[JOYSTICK_VALUE];

void init_joystick();
void sliding_filter_joystick_value();
float ConvertToJoystickValue(int adc, int min, int max, int center);

#endif //JOYSTICK_H
