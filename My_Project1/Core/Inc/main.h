/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// 声明火焰报警标志位（供所有任务调用）
extern uint8_t Flame_Alarm_Flag;
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DHT11_PIN_Pin GPIO_PIN_13
#define DHT11_PIN_GPIO_Port GPIOC
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOD
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOD
#define W25Q64_SCK_Pin GPIO_PIN_5
#define W25Q64_SCK_GPIO_Port GPIOA
#define W25Q64_MISO_Pin GPIO_PIN_6
#define W25Q64_MISO_GPIO_Port GPIOA
#define W25Q64_MOSI_Pin GPIO_PIN_7
#define W25Q64_MOSI_GPIO_Port GPIOA
#define FLAME_INT_Pin GPIO_PIN_0
#define FLAME_INT_GPIO_Port GPIOB
#define FLAME_INT_EXTI_IRQn EXTI0_IRQn
#define SRF04_ECHO_Pin GPIO_PIN_1
#define SRF04_ECHO_GPIO_Port GPIOB
#define SRF04_ECHO_EXTI_IRQn EXTI1_IRQn
#define ESP01_TX_Pin GPIO_PIN_9
#define ESP01_TX_GPIO_Port GPIOA
#define ESP01_RX_Pin GPIO_PIN_10
#define ESP01_RX_GPIO_Port GPIOA
#define AT24C02_SCL_Pin GPIO_PIN_6
#define AT24C02_SCL_GPIO_Port GPIOB
#define AT24C02_SDA_Pin GPIO_PIN_7
#define AT24C02_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern uint8_t Flame_Alarm_Flag;
extern uint8_t UART_Recv_Flag;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
