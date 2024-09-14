//
// Created by XFZAILDD on 24-7-30.
//
#ifndef FILTER_H
#define FILTER_H

#include "main.h"

typedef struct {
    uint16_t *sf_buf;       /* 数据缓冲区 */
    uint16_t sf_sum;        /* N个数据的算术和 */
    uint8_t sf_length;      /* 数据个数 */
    int sf_number;          /* 数据个数 */
    int sf_pos;             /* 缓冲区位置 */
} Sliding_Filter_TypeDef;

void sliding_filter_init(Sliding_Filter_TypeDef *data, uint8_t sliding_filter_length, uint8_t number);

int sliding_filter(Sliding_Filter_TypeDef *data, int32_t val);


#endif //FILTER_H
