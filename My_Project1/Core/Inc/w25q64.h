#ifndef __W25Q64_H
#define __W25Q64_H

#include "main.h"
#include "spi.h"

#define W25Q64_CS_Pin GPIO_PIN_4
#define W25Q64_CS_Port GPIOA

void W25Q64_Read(uint32_t addr, uint8_t *buf, uint16_t len);
void W25Q64_Write(uint32_t addr, uint8_t *buf, uint16_t len);

#endif

