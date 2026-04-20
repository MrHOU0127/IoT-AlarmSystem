#include "srf04.h"
#include "gpio.h"

uint32_t SRF04_Time = 0;
uint8_t SRF04_Flag = 0;

#define TRIG_Pin GPIO_PIN_2
#define TRIG_Port GPIOB

void SRF04_Start(void) {
    HAL_GPIO_WritePin(TRIG_Port, TRIG_Pin, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(TRIG_Port, TRIG_Pin, GPIO_PIN_RESET);
}

float SRF04_GetDistance(void) {
    float dis = 0;
    if(SRF04_Flag) {
        dis = (float)SRF04_Time * 0.017;
        SRF04_Flag = 0;
    }
    return dis;
}

// 把这个函数放到 EXTI1 中断回调里！
void SRF04_EXTI_Callback(void) {
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
        SRF04_Time = HAL_GetTick();
    else {
        SRF04_Time = HAL_GetTick() - SRF04_Time;
        SRF04_Flag = 1;
    }
}

