#include "dht11.h"
#include "gpio.h"

#define DHT11_PIN_Port GPIOC
#define DHT11_PIN_Pin GPIO_PIN_13

static void DHT11_Delay(uint32_t us) {
    uint32_t i = us * 8;
    while(i--);
}

static void DHT11_SetOutput(void) {
    GPIO_InitTypeDef gpio_conf = {0};
    gpio_conf.Pin = DHT11_PIN_Pin;
    gpio_conf.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_conf.Pull = GPIO_NOPULL;
    gpio_conf.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PIN_Port, &gpio_conf);
}

static void DHT11_SetInput(void) {
    GPIO_InitTypeDef gpio_conf = {0};
    gpio_conf.Pin = DHT11_PIN_Pin;
    gpio_conf.Mode = GPIO_MODE_INPUT;
    gpio_conf.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_PIN_Port, &gpio_conf);
}

uint8_t DHT11_Read(DHT11_Data_t *data) {
    uint8_t buf[5] = {0};
    DHT11_SetOutput();
    HAL_GPIO_WritePin(DHT11_PIN_Port, DHT11_PIN_Pin, GPIO_PIN_RESET);
    HAL_Delay(18);
    DHT11_SetInput();
    DHT11_Delay(40);

    if(HAL_GPIO_ReadPin(DHT11_PIN_Port, DHT11_PIN_Pin) == 0) {
        for(uint8_t i=0; i<5; i++) {
            for(uint8_t j=0; j<8; j++) {
                while(HAL_GPIO_ReadPin(DHT11_PIN_Port, DHT11_PIN_Pin) == 0);
                DHT11_Delay(40);
                if(HAL_GPIO_ReadPin(DHT11_PIN_Port, DHT11_PIN_Pin) == 1)
                    buf[i] |= (1 << (7-j));
                while(HAL_GPIO_ReadPin(DHT11_PIN_Port, DHT11_PIN_Pin) == 1);
            }
        }
        if(buf[0]+buf[1]+buf[2]+buf[3] == buf[4]) {
            data->humi = buf[0];
            data->temp = buf[2];
            return 0;
        }
    }
    return 1;
}

