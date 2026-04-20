/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"  // 严格CMSIS-V1头文件，无V2内容

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dht11.h"
#include "srf04.h"
#include "at24c02.h"
#include "w25q64.h"
#include "esp01.h"
#include "ringbuff.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// 传感器数据结构体：大小17字节，远小于队列Item Size 32字节，可直接传输
typedef struct {
    float temp;       // 4字节
    float humi;       // 4字节
    float distance;   // 4字节
    uint8_t flame;    // 1字节
    uint8_t reserve[19]; // 填充到32字节，严格匹配队列Item Size（可选，不填也可）
} DataPacket_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define W25Q64_SAVE_ADDR  0x000000U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
// 外部火焰报警标志（中断→任务同步）
extern uint8_t Flame_Alarm_Flag;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId Task_CollectHandle;
osThreadId Task_SendHandle;
osThreadId Task_ReceiveHandle;
osMessageQId Data_QueueHandle;  // CMSIS-V1队列句柄
osSemaphoreId UART_SemaphoreHandle;  // CMSIS-V1二进制信号量句柄

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Parse_UART_Data(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Task_CollectFunc(void const * argument);
void Task_SendFunc(void const * argument);
void Task_ReceiveFunc(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of UART_Semaphore（严格二进制，初始Available=1） */
  osSemaphoreDef(UART_Semaphore);
  UART_SemaphoreHandle = osSemaphoreCreate(osSemaphore(UART_Semaphore), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of Data_Queue（严格匹配截图：长度10，Item Size 32字节） */
  osMessageQDef(Data_Queue, 10, 32);
  Data_QueueHandle = osMessageCreate(osMessageQ(Data_Queue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Task_Collect（严格截图配置：高优先级，栈128） */
  osThreadDef(Task_Collect, Task_CollectFunc, osPriorityHigh, 0, 128);
  Task_CollectHandle = osThreadCreate(osThread(Task_Collect), NULL);

  /* definition and creation of Task_Send（严格截图配置：中优先级，栈128） */
  osThreadDef(Task_Send, Task_SendFunc, osPriorityNormal, 0, 128);
  Task_SendHandle = osThreadCreate(osThread(Task_Send), NULL);

  /* definition and creation of Task_Receive（严格截图配置：低优先级，栈128） */
  osThreadDef(Task_Receive, Task_ReceiveFunc, osPriorityLow, 0, 128);
  Task_ReceiveHandle = osThreadCreate(osThread(Task_Receive), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  // 系统初始化：ESP01初始化（仅执行一次）
  ESP01_Init();
  
  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Task_CollectFunc */
/**
* @brief Function implementing the Task_Collect thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_CollectFunc */
void Task_CollectFunc(void const * argument)
{
  /* USER CODE BEGIN Task_CollectFunc */
  DHT11_Data_t dht_data;
  DataPacket_t data;  // 局部结构体，直接入队列
  
  /* Infinite loop */
  for(;;)
  {
    // 1. 采集传感器数据
    DHT11_Read(&dht_data);
    SRF04_Start();
    
    // 填充结构体
    data.temp = dht_data.temp;
    data.humi = dht_data.humi;
    data.distance = SRF04_GetDistance();
    data.flame = HAL_GPIO_ReadPin(FLAME_INT_GPIO_Port, FLAME_INT_Pin);

    // 2. CMSIS-V1队列发送：osMessagePut（V1专属API，无V2内容）
    osMessagePut(Data_QueueHandle, (uint32_t)&data, osWaitForever);

    osDelay(100);
  }
  /* USER CODE END Task_CollectFunc */
}

/* USER CODE BEGIN Header_Task_SendFunc */
/**
* @brief Function implementing the Task_Send thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_SendFunc */
void Task_SendFunc(void const * argument)
{
  /* USER CODE BEGIN Task_SendFunc */
  char send_buf[64];
  
  /* Infinite loop */
  for(;;)
  {
    // 修复：V1 正确队列接收（无参数错误）
    osEvent event = osMessageGet(Data_QueueHandle, osWaitForever);
    DataPacket_t *data = (DataPacket_t *)event.value.p;

    // 2. 本地存储：AT24C02
    AT24C02_Write(0x00, (uint8_t)data->temp);
    AT24C02_Write(0x01, (uint8_t)data->humi);
    
    // 3. ESP01上传数据
    sprintf(send_buf, "TEMP:%.1f HUMI:%.1f DIS:%.1f FLAME:%d",
            data->temp, data->humi, data->distance, data->flame);
    ESP01_SendData(send_buf);

    osDelay(10);
  }
  /* USER CODE END Task_SendFunc */
}

/* USER CODE BEGIN Header_Task_ReceiveFunc */
/**
* @brief Function implementing the Task_Receive thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_ReceiveFunc */
void Task_ReceiveFunc(void const * argument)
{
  /* USER CODE BEGIN Task_ReceiveFunc */
  /* Infinite loop */
  for(;;)
  {
    // ============= 火焰报警处理 =============
    if(Flame_Alarm_Flag == 1)
    {
      ESP01_SendData("ALARM: FIRE DETECTED!");
      Flame_Alarm_Flag = 0;
    }

    // ============= 串口数据处理 =============
    if(UART_Recv_Flag == 1)
    {
      Parse_UART_Data();
      UART_Recv_Flag = 0;
    }

    osDelay(10);  // 轮询延迟，不占CPU
  }
  /* USER CODE END Task_ReceiveFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// 串口数据解析空实现（后续可补充业务逻辑）
void Parse_UART_Data(void)
{
  uint16_t len = RingBuff_GetLen(&esp01_rb);
  for(uint16_t i=0; i<len; i++)
  {
    uint8_t data = RingBuff_Read(&esp01_rb);
    // 自定义数据处理逻辑
  }
}
/* USER CODE END Application */
