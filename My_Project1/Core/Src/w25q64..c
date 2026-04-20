#include "w25q64.h"

static void W25Q64_CS_LOW(void) { HAL_GPIO_WritePin(W25Q64_CS_Port, W25Q64_CS_Pin, GPIO_PIN_RESET); }
static void W25Q64_CS_HIGH(void) { HAL_GPIO_WritePin(W25Q64_CS_Port, W25Q64_CS_Pin, GPIO_PIN_SET); }

void W25Q64_Read(uint32_t addr, uint8_t *buf, uint16_t len) {
    W25Q64_CS_LOW();
    uint8_t cmd[4] = {0x03, (addr>>16)&0xFF, (addr>>8)&0xFF, addr&0xFF};
    HAL_SPI_Transmit(&hspi1, cmd, 4, 100);
    HAL_SPI_Receive(&hspi1, buf, len, 100);
    W25Q64_CS_HIGH();
}

void W25Q64_Write(uint32_t addr, uint8_t *buf, uint16_t len) {
    W25Q64_CS_LOW();
    uint8_t cmd = 0x06;
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
    W25Q64_CS_HIGH();

    W25Q64_CS_LOW();
    cmd = 0x02;
    uint8_t add[4] = {cmd, (addr>>16)&0xFF, (addr>>8)&0xFF, addr&0xFF};
    HAL_SPI_Transmit(&hspi1, add, 4, 100);
    HAL_SPI_Transmit(&hspi1, buf, len, 100);
    W25Q64_CS_HIGH();
    HAL_Delay(5);
}

