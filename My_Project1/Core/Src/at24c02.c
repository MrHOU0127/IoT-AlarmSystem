#include "at24c02.h"

void AT24C02_Write(uint8_t addr, uint8_t data) {
    HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    HAL_Delay(5);
}

uint8_t AT24C02_Read(uint8_t addr) {
    uint8_t data = 0;
    HAL_I2C_Mem_Read(&hi2c1, AT24C02_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    return data;
}

