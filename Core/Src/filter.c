//
// Created by XFZAILDD on 24-7-30.
//
#include <malloc.h>
#include <memory.h>
#include "filter.h"
#include "joystick.h"

/**
 * @brief 滑动滤波器
 * @param data
 * @param vale
 * @return
 */
int sliding_filter(Sliding_Filter_TypeDef *data, int32_t vale) {
    // 除去最旧的数据，加入最新的数据
    data->sf_sum += (vale - (data->sf_buf[data->sf_pos]));
    // 将新数据加入缓冲区
    data->sf_buf[data->sf_pos] = vale;
    // 更新缓冲区位置
    data->sf_pos = (data->sf_pos + 1) % data->sf_length;
    //  数据个数不足时，sf_number是实际个数，个数足够时，sf_number最多也只是SLIDING_FILTER_LENGTH
    data->sf_number += (data->sf_number < data->sf_length);
    return (data->sf_sum) / (data->sf_number);
}

/**
 * @brief 初始化滑动滤波器
 * @param data
 * @param sliding_filter_length
 */
void sliding_filter_init(Sliding_Filter_TypeDef *data, uint8_t sliding_filter_length, uint8_t number) {
    for (int i = 0; i < number; i++) {
        data[i].sf_sum = 0;
        data[i].sf_number = 0;
        data[i].sf_pos = 0;
        data[i].sf_length = sliding_filter_length;
        data[i].sf_buf = (uint16_t *) malloc(sliding_filter_length * sizeof(uint16_t));
        if (data[i].sf_buf != NULL) {
            memset(data[i].sf_buf, 0, sliding_filter_length * sizeof(uint16_t)); // 确保缓冲区清零
        }
    }
}

