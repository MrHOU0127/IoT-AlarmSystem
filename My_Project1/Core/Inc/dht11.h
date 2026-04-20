#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"

typedef struct {
    uint8_t temp;
    uint8_t humi;
} DHT11_Data_t;

uint8_t DHT11_Read(DHT11_Data_t *data);

#endif

