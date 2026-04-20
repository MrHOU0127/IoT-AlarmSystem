#ifndef __AT24C02_H
#define __AT24C02_H

#include "main.h"
#include "i2c.h"

#define AT24C02_ADDR 0xA0

void AT24C02_Write(uint8_t addr, uint8_t data);
uint8_t AT24C02_Read(uint8_t addr);

#endif

