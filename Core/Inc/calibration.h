//
// Created by XFZAILDD on 2024/8/5.
//
#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "main.h"

#define FREQUENCY 100
#define LONG_PRESS_TIME 5
#define RECORDING_VALUE LONG_PRESS_TIME * 1000 / FREQUENCY

extern uint16_t recording_value;

typedef struct {
    uint16_t min;
    uint16_t max;
    uint16_t center_min;
    uint16_t center_max;
} Joystick_ADC;

typedef struct {
    uint16_t min;
    uint16_t max;
} Trigger_ADC;

extern Joystick_ADC joystick_x_adc;
extern Joystick_ADC joystick_y_adc;
extern Trigger_ADC trigger_1_adc;
extern Trigger_ADC trigger_2_adc;

void calibration_joystick();
void read_calibration_value();

#endif //CALIBRATION_H
