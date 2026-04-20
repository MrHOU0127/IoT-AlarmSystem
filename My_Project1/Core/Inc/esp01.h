#ifndef __ESP01_H
#define __ESP01_H

#include "main.h"
#include "usart.h"
#include "ringbuff.h"

extern RingBuff_t esp01_rb;

void ESP01_Init(void);
void ESP01_SendData(char *data);
void ESP01_UART_Callback(void);
#endif

