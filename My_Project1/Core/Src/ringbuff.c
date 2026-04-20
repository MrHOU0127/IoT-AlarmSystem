#include "ringbuff.h"

void RingBuff_Write(RingBuff_t *rb, uint8_t data) {
    rb->buffer[rb->write] = data;
    rb->write = (rb->write + 1) % UART_BUFF_SIZE;
}

uint8_t RingBuff_Read(RingBuff_t *rb) {
    uint8_t data = rb->buffer[rb->read];
    rb->read = (rb->read + 1) % UART_BUFF_SIZE;
    return data;
}

uint16_t RingBuff_GetLen(RingBuff_t *rb) {
    return (rb->write - rb->read + UART_BUFF_SIZE) % UART_BUFF_SIZE;
}

