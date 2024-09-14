#ifndef ECG_DATA_FIFO_H
#define ECG_DATA_FIFO_H

#include "main.h"
#include "joystick.h"
#include "button.h"
#include <malloc.h>
#include <memory.h>

#define FIFO_SIZE 500

typedef struct {
    uint16_t sliding_filter_dma_adc[JOYSTICK_VALUE];
    ButtonState button_state[BUTTON_NUM];
} handle_data;

typedef struct {
    void *buffer;   /*  缓冲区 */
    size_t element_size;  /*  缓冲区单个元素大小 */
    uint16_t head;  /*  头指针 */
    uint16_t tail;  /*  尾指针 */
} FIFO_Buffer;

extern FIFO_Buffer data_fifo;

void FIFO_Init(FIFO_Buffer *fifo, size_t element_size);
int FIFO_IsEmpty(FIFO_Buffer *fifo);
int FIFO_IsFull(FIFO_Buffer *fifo);
void FIFO_Write(FIFO_Buffer *fifo, void *data);
void FIFO_Read(FIFO_Buffer *fifo, void *data);
int FIFO_theQuantity(FIFO_Buffer *fifo);
void FIFO_Free(FIFO_Buffer *fifo);


#endif
