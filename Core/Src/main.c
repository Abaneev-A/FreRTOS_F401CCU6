/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */
typedef struct {
	char Buf[128];
} QUEUE_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myLED1Task */
osThreadId_t myLED1TaskHandle;
uint32_t myLED1TaskBuffer[ 128 ];
osStaticThreadDef_t myLED1TaskControlBlock;
const osThreadAttr_t myLED1Task_attributes = {
  .name = "myLED1Task",
  .cb_mem = &myLED1TaskControlBlock,
  .cb_size = sizeof(myLED1TaskControlBlock),
  .stack_mem = &myLED1TaskBuffer[0],
  .stack_size = sizeof(myLED1TaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myLED2Task */
osThreadId_t myLED2TaskHandle;
uint32_t myLED2TaskBuffer[ 128 ];
osStaticThreadDef_t myLED2TaskControlBlock;
const osThreadAttr_t myLED2Task_attributes = {
  .name = "myLED2Task",
  .cb_mem = &myLED2TaskControlBlock,
  .cb_size = sizeof(myLED2TaskControlBlock),
  .stack_mem = &myLED2TaskBuffer[0],
  .stack_size = sizeof(myLED2TaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myReadBtnTask */
osThreadId_t myReadBtnTaskHandle;
uint32_t myReadBtnBuffer[ 128 ];
osStaticThreadDef_t myReadBtnControlBlock;
const osThreadAttr_t myReadBtnTask_attributes = {
  .name = "myReadBtnTask",
  .cb_mem = &myReadBtnControlBlock,
  .cb_size = sizeof(myReadBtnControlBlock),
  .stack_mem = &myReadBtnBuffer[0],
  .stack_size = sizeof(myReadBtnBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myLED3Task */
osThreadId_t myLED3TaskHandle;
uint32_t myLED3TaskBuffer[ 128 ];
osStaticThreadDef_t myLED3TaskControlBlock;
const osThreadAttr_t myLED3Task_attributes = {
  .name = "myLED3Task",
  .cb_mem = &myLED3TaskControlBlock,
  .cb_size = sizeof(myLED3TaskControlBlock),
  .stack_mem = &myLED3TaskBuffer[0],
  .stack_size = sizeof(myLED3TaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myADCTask */
osThreadId_t myADCTaskHandle;
uint32_t myADCTaskBuffer[ 128 ];
osStaticThreadDef_t myADCTaskControlBlock;
const osThreadAttr_t myADCTask_attributes = {
  .name = "myADCTask",
  .cb_mem = &myADCTaskControlBlock,
  .cb_size = sizeof(myADCTaskControlBlock),
  .stack_mem = &myADCTaskBuffer[0],
  .stack_size = sizeof(myADCTaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myUARTTask */
osThreadId_t myUARTTaskHandle;
uint32_t myUARTTaskBuffer[ 128 ];
osStaticThreadDef_t myUARTTaskControlBlock;
const osThreadAttr_t myUARTTask_attributes = {
  .name = "myUARTTask",
  .cb_mem = &myUARTTaskControlBlock,
  .cb_size = sizeof(myUARTTaskControlBlock),
  .stack_mem = &myUARTTaskBuffer[0],
  .stack_size = sizeof(myUARTTaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for UARTQueue */
osMessageQueueId_t UARTQueueHandle;
uint8_t UARTQueueBuffer[ 10 * sizeof( QUEUE_t ) ];
osStaticMessageQDef_t UARTQueueControlBlock;
const osMessageQueueAttr_t UARTQueue_attributes = {
  .name = "UARTQueue",
  .cb_mem = &UARTQueueControlBlock,
  .cb_size = sizeof(UARTQueueControlBlock),
  .mq_mem = &UARTQueueBuffer,
  .mq_size = sizeof(UARTQueueBuffer)
};
/* Definitions for BtnSem */
osSemaphoreId_t BtnSemHandle;
osStaticSemaphoreDef_t BtControlBlock;
const osSemaphoreAttr_t BtnSem_attributes = {
  .name = "BtnSem",
  .cb_mem = &BtControlBlock,
  .cb_size = sizeof(BtControlBlock),
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
void StartDefaultTask(void *argument);
void StartLED1Task(void *argument);
void StartLED2Task(void *argument);
void StartReadBtnTask(void *argument);
void StartLED3Task(void *argument);
void StartADCTask(void *argument);
void StartUARTTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  //HAL_ADC_Start(&hadc1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of BtnSem */
  BtnSemHandle = osSemaphoreNew(1, 0, &BtnSem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of UARTQueue */
  UARTQueueHandle = osMessageQueueNew (10, sizeof(QUEUE_t), &UARTQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myLED1Task */
  myLED1TaskHandle = osThreadNew(StartLED1Task, NULL, &myLED1Task_attributes);

  /* creation of myLED2Task */
  myLED2TaskHandle = osThreadNew(StartLED2Task, NULL, &myLED2Task_attributes);

  /* creation of myReadBtnTask */
  myReadBtnTaskHandle = osThreadNew(StartReadBtnTask, NULL, &myReadBtnTask_attributes);

  /* creation of myLED3Task */
  myLED3TaskHandle = osThreadNew(StartLED3Task, NULL, &myLED3Task_attributes);

  /* creation of myADCTask */
  myADCTaskHandle = osThreadNew(StartADCTask, NULL, &myADCTask_attributes);

  /* creation of myUARTTask */
  myUARTTaskHandle = osThreadNew(StartUARTTask, NULL, &myUARTTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 300;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4095;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|LED2_Pin|LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Btn_Pin */
  GPIO_InitStruct.Pin = Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartLED1Task */
/**
* @brief Function implementing the myLED1Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLED1Task */
void StartLED1Task(void *argument)
{
  /* USER CODE BEGIN StartLED1Task */
	QUEUE_t msg;
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	strcpy(msg.Buf,"LED1 Blink\r\n");
	osMessageQueuePut(UARTQueueHandle, &msg, 0, osWaitForever);
	osDelay(1000);
  }
  /* USER CODE END StartLED1Task */
}

/* USER CODE BEGIN Header_StartLED2Task */
/**
* @brief Function implementing the myLED2Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLED2Task */
void StartLED2Task(void *argument)
{
  /* USER CODE BEGIN StartLED2Task */
	QUEUE_t msg;
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
	strcpy(msg.Buf,"LED2 Blink\r\n");
	osMessageQueuePut(UARTQueueHandle, &msg, 0, osWaitForever);
	osDelay(2000);
  }
  /* USER CODE END StartLED2Task */
}

/* USER CODE BEGIN Header_StartReadBtnTask */
/**
* @brief Function implementing the myReadBtnTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartReadBtnTask */
void StartReadBtnTask(void *argument)
{
  /* USER CODE BEGIN StartReadBtnTask */
  QUEUE_t msg;
  /* Infinite loop */
  for(;;)
  {
	if (!HAL_GPIO_ReadPin(Btn_GPIO_Port, Btn_Pin))
	{
		osSemaphoreRelease(BtnSemHandle);
		strcpy(msg.Buf,"Btn Pressed\r\n");
		osMessageQueuePut(UARTQueueHandle, &msg, 0, osWaitForever);
	}
	osDelay(200);
  }
  /* USER CODE END StartReadBtnTask */
}

/* USER CODE BEGIN Header_StartLED3Task */
/**
* @brief Function implementing the myLED3Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLED3Task */
void StartLED3Task(void *argument)
{
  /* USER CODE BEGIN StartLED3Task */
  /* Infinite loop */
  for(;;)
  {
	if (osSemaphoreAcquire(BtnSemHandle, osWaitForever) == osOK)
	{
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
	}
	 osDelay(100);
  }
  /* USER CODE END StartLED3Task */
}

/* USER CODE BEGIN Header_StartADCTask */
/**
* @brief Function implementing the myADCTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartADCTask */
void StartADCTask(void *argument)
{
  /* USER CODE BEGIN StartADCTask */
  QUEUE_t msg;
  /* Infinite loop */
  for(;;)
  {
	  HAL_ADC_Start(&hadc1);
	  uint16_t adc_res = HAL_ADC_GetValue(&hadc1);
	  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,adc_res);
	  if (adc_res < 10)
	  {
	    strcpy(msg.Buf,"MIN ADC\r\n");
	    osMessageQueuePut(UARTQueueHandle, &msg, 0, osWaitForever);
	    osDelay(100);
	  }
	  else if (adc_res > 4000)
	  {
	    strcpy(msg.Buf,"MAX ADC\r\n");
	    osMessageQueuePut(UARTQueueHandle, &msg, 0, osWaitForever);
	    osDelay(100);
	  }
	  osDelay(10);
  }
  /* USER CODE END StartADCTask */
}

/* USER CODE BEGIN Header_StartUARTTask */
/**
* @brief Function implementing the myUARTTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUARTTask */
void StartUARTTask(void *argument)
{
  /* USER CODE BEGIN StartUARTTask */
  QUEUE_t msg;
  /* Infinite loop */
  for(;;)
  {
	osMessageQueueGet(UARTQueueHandle, &msg, 0, osWaitForever);
	HAL_UART_Transmit(&huart1, (uint8_t*)msg.Buf, strlen(msg.Buf), osWaitForever);
	osDelay(1);
  }
  /* USER CODE END StartUARTTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
