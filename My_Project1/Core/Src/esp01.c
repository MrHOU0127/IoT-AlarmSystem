#include "esp01.h"
#include <stdio.h>
#include <string.h>

RingBuff_t esp01_rb = {0};

void ESP01_Init(void)
{
    // 开启串口1接收中断
    HAL_UART_Receive_IT(&huart1, &esp01_rb.buffer[esp01_rb.write], 1);
    HAL_Delay(1000);

    // 复位ESP01
    HAL_UART_Transmit(&huart1, (uint8_t*)"AT+RST\r\n", 7, 100);
    HAL_Delay(1000);

    // 设置为Station模式
    HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CWMODE=1\r\n", 11, 100);
    HAL_Delay(500);

    // ==============================================
    // 在这里修改为 你的英文WiFi名称和密码
    // ==============================================
    HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CWJAP=\"WIFI_NAME\",\"WIFI_PASSWORD\"\r\n", 38, 1000);
    HAL_Delay(3000);
}

void ESP01_SendData(char *data)
{
    char cmd[128];
    // 服务器连接指令（自行修改IP）
    sprintf(cmd, "AT+CIPSTART=\"TCP\",\"192.168.1.1\",80\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t*)cmd, strlen(cmd), 500);
    HAL_Delay(1000);
}

// 串口接收中断回调
void ESP01_UART_Callback(void)
{
    RingBuff_Write(&esp01_rb, esp01_rb.buffer[esp01_rb.write]);
    HAL_UART_Receive_IT(&huart1, &esp01_rb.buffer[esp01_rb.write], 1);
}
