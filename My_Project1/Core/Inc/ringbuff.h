#ifndef __RINGBUFF_H
#define __RINGBUFF_H

#include "main.h"
#include <stdint.h>

#define UART_BUFF_SIZE 256

typedef struct {
    uint8_t buffer[UART_BUFF_SIZE];
    uint16_t write;
    uint16_t read;
} RingBuff_t;

void RingBuff_Write(RingBuff_t *rb, uint8_t data);
uint8_t RingBuff_Read(RingBuff_t *rb);
uint16_t RingBuff_GetLen(RingBuff_t *rb);

#endif

