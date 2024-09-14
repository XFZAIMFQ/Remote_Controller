#include "data_fifo.h"

FIFO_Buffer data_fifo;


/**
 * @brief 初始化缓冲区
 * @param fifo 缓冲区
 * @param element_size 元素大小
 */
void FIFO_Init(FIFO_Buffer *fifo, size_t element_size) {
    fifo->head = 0;     /*  初始化头指针 */
    fifo->tail = 0;     /*  初始化尾指针 */
    fifo->element_size = element_size;  /*  设置缓冲区单个元素大小 */
    fifo->buffer = malloc(FIFO_SIZE * element_size);   /*  申请内存 */
}

/**
 * @brief 判断缓冲区是否为空
 * @param fifo 缓冲区
 * @return 1为空，0不为空
 */
int FIFO_IsEmpty(FIFO_Buffer *fifo) {
    return fifo->head == fifo->tail;
}

/**
 * @brief 判断缓冲区是否已满
 * @param fifo 缓冲区
 * @return 1为满，0不满
 */
int FIFO_IsFull(FIFO_Buffer *fifo) {
    return ((fifo->head + 1) % FIFO_SIZE) == fifo->tail;
}

/**
 * @brief 写入数据
 * @param fifo 缓冲区
 * @param data 数据
 */
void FIFO_Write(FIFO_Buffer *fifo, void *data) {
    if (!FIFO_IsFull(fifo)) {
        memcpy((char *) fifo->buffer + fifo->head * fifo->element_size, data, fifo->element_size);
        fifo->head = (fifo->head + 1) % FIFO_SIZE;
    }
}

/**
 * @brief 读取数据
 * @param fifo 缓冲区
 * @param data 数据
 */
void FIFO_Read(FIFO_Buffer *fifo, void *data) {
    if (!FIFO_IsEmpty(fifo)) {
        memcpy(data, (char *) fifo->buffer + fifo->tail * fifo->element_size, fifo->element_size);
        fifo->tail = (fifo->tail + 1) % FIFO_SIZE;
    }
}

/**
 * @brief 获取缓冲区中的数据数量
 * @param fifo 缓冲区
 * @return 数据数量
 */
int FIFO_theQuantity(FIFO_Buffer *fifo) {
    return (fifo->head - fifo->tail + FIFO_SIZE) % FIFO_SIZE;
}

/**
 * @brief 释放缓冲区
 * @param fifo 缓冲区
 */
void FIFO_Free(FIFO_Buffer *fifo) {
    free(fifo->buffer);
}