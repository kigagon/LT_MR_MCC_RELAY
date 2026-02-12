/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Compile_Data.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

IWDG_HandleTypeDef hiwdg;

UART_HandleTypeDef hlpuart1;
DMA_HandleTypeDef hdma_lpuart1_tx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

PCD_HandleTypeDef hpcd_USB_DRD_FS;

/* USER CODE BEGIN PV */

uint8_t MCC_Address_tmp[4] ;
uint8_t MCC_R_Address ;

uint8_t SUB_UART_TX_buf[SUB_UART_buf_len] ;
uint8_t SUB_UART_RX_buf[SUB_UART_buf_len];
uint8_t SUB_UART_Tx_buf_tmp[SUB_UART_buf_len];
uint8_t SUB_UART_RX_buf_tmp[SUB_UART_buf_len];
uint8_t SUB_UART_State ;					// Define reception start state
int SUB_UART_buf_count;					// Save the length of received data after starting reception
int SUB_UART_buf_count_tmp;				// Variable to store the length of the received data before initializing it when reception is completed
uint8_t SUB_UART_Receive_complete;		// Variable indicating that reception has been completed
int SUB_UART_buf_count_Save;

uint8_t RY_Status_Avr[8][RY_Status_Avr_Num];
uint8_t RY_Status_Avr_Val[8];

uint8_t RY_Status[8] ;
uint8_t Pump_ps_Status[8] ;
uint8_t Pump_ck_Status[8] ;
uint8_t Pump_err_Status[8] ;

#define Save_Num	10

uint8_t Pump_ps_Load_Num;
uint8_t Pump_ck_Load_Num;
uint8_t Pump_err_Load_Num;

uint8_t Pump_ps_Status_tmp[8][Save_Num] ;
uint8_t Pump_ck_Status_tmp[8][Save_Num];
uint8_t Pump_err_Status_tmp[8][Save_Num] ;


#define Version_SW		0
#define Version_MCC		1
#define Version_MCC_R	2
#define Version_TB		3
#define Version_DO		4
#define Version_EBC		5
uint8_t Version_Info[6][14];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_TIM1_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

///////////////Start timer interrupt operation function///////////////////

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM1)
  {
    HAL_GPIO_TogglePin(RUN_LED_GPIO_Port, RUN_LED_Pin);

    HAL_IWDG_Refresh(&hiwdg);

  }
  else if(htim->Instance == TIM2)
    {



    }
  else if(htim->Instance == TIM3)
  {



  }

}
////////////////Timer interrupt operation function ends ///////////////////
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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_LPUART1_UART_Init();
  MX_USB_PCD_Init();
  MX_TIM1_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */

  // Timer1: 1 second cycle
  // Timer15: 0.1 second cycle
  // Timer16: 0.1 second cycle
  HAL_TIM_OC_Start_IT(&htim1,TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim2,TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim3,TIM_CHANNEL_1);

  LED_Control(ERR_LED_GPIO_Port, ERR_LED_Pin , LED_Off);

  Compile_Date();

  Read_Address();

  MCC_R_Address = 1;
  /* huart1 RX Interrupt  Enable */
  /* Process Unlocked */
  __HAL_UNLOCK(&hlpuart1);
  /* Enable the UART Parity Error Interrupt */
  __HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_PE);
  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_ERR);
  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_RXNE);

  /*
  PUMP1_ON();
  HAL_Delay(200);
  PUMP2_ON();
  HAL_Delay(200);
  PUMP3_ON();
  HAL_Delay(200);
  PUMP4_ON();
  HAL_Delay(200);
  PUMP5_ON();
  HAL_Delay(200);
  PUMP6_ON();
  HAL_Delay(200);

  HAL_Delay(1000);
  PUMP1_OFF();
  HAL_Delay(200);
  PUMP2_OFF();
  HAL_Delay(200);
  PUMP3_OFF();
  HAL_Delay(200);
  PUMP4_OFF();
  HAL_Delay(200);
  PUMP5_OFF();
  HAL_Delay(200);
  PUMP6_OFF();
  HAL_Delay(200);
*/

	for(int i=0;i<8;i++){
		RY_Status[i] = 0;
		Pump_ps_Status[i] = 0;
		Pump_ck_Status[i] = 0;
		Pump_err_Status[i] = 0;
	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	__HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_ERR);
	/* Enable the UART Data Register not empty Interrupt */
	__HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_RXNE);

	Set_Relay_Avr();
	Read_Pump_Ps();
	Read_Pump_Ck();
	Read_Pump_Err();

	if(SUB_UART_Receive_complete == 1){
	  SW_Com();
	}
	HAL_Delay(1);


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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 7;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_16;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  hiwdg.Init.EWI = 0;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 5599;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 9999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 5599;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 5599;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_DRD_FS.Instance = USB_DRD_FS;
  hpcd_USB_DRD_FS.Init.dev_endpoints = 8;
  hpcd_USB_DRD_FS.Init.speed = USBD_FS_SPEED;
  hpcd_USB_DRD_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_DRD_FS.Init.Sof_enable = DISABLE;
  hpcd_USB_DRD_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_DRD_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_DRD_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_DRD_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PUMP1_AUTO_Pin|PUMP2_AUTO_Pin|PUMP3_AUTO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, PUMP4_AUTO_Pin|PUMP5_AUTO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PUMP6_AUTO_Pin|RS485_DE_Pin|RS485_RE_Pin|ERR_LED_Pin
                          |RUN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PUMP1_PS_ERR_Pin PUMP6_PS_ERR_Pin PUMP5_PS_Pin PUMP5_PS_ERR_Pin */
  GPIO_InitStruct.Pin = PUMP1_PS_ERR_Pin|PUMP6_PS_ERR_Pin|PUMP5_PS_Pin|PUMP5_PS_ERR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PUMP1_CHECK_Pin PUMP2_CHECK_Pin PUMP3_CHECK_Pin ADDR_DIP3_Pin_Pin
                           ADDR_DIP2_Pin_Pin ADDR_DIP1_Pin_Pin */
  GPIO_InitStruct.Pin = PUMP1_CHECK_Pin|PUMP2_CHECK_Pin|PUMP3_CHECK_Pin|ADDR_DIP3_Pin_Pin
                          |ADDR_DIP2_Pin_Pin|ADDR_DIP1_Pin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PUMP4_CHECK_Pin PUMP5_CHECK_Pin PUMP6_CHECK_Pin ADDR_DIP0_Pin_Pin */
  GPIO_InitStruct.Pin = PUMP4_CHECK_Pin|PUMP5_CHECK_Pin|PUMP6_CHECK_Pin|ADDR_DIP0_Pin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PUMP1_AUTO_Pin PUMP2_AUTO_Pin PUMP3_AUTO_Pin */
  GPIO_InitStruct.Pin = PUMP1_AUTO_Pin|PUMP2_AUTO_Pin|PUMP3_AUTO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PUMP4_AUTO_Pin PUMP5_AUTO_Pin */
  GPIO_InitStruct.Pin = PUMP4_AUTO_Pin|PUMP5_AUTO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PUMP6_AUTO_Pin RS485_DE_Pin RS485_RE_Pin ERR_LED_Pin
                           RUN_LED_Pin */
  GPIO_InitStruct.Pin = PUMP6_AUTO_Pin|RS485_DE_Pin|RS485_RE_Pin|ERR_LED_Pin
                          |RUN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PUMP6_PS_Pin */
  GPIO_InitStruct.Pin = PUMP6_PS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PUMP6_PS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PUMP4_PS_Pin */
  GPIO_InitStruct.Pin = PUMP4_PS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PUMP4_PS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PUMP4_PS_ERR_Pin PUMP3_PS_Pin PUMP3_PS_ERR_Pin PUMP2_PS_Pin
                           PUMP2_PS_ERR_Pin PUMP1_PS_Pin */
  GPIO_InitStruct.Pin = PUMP4_PS_ERR_Pin|PUMP3_PS_Pin|PUMP3_PS_ERR_Pin|PUMP2_PS_Pin
                          |PUMP2_PS_ERR_Pin|PUMP1_PS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void Read_Address(void){

	  MCC_Address_tmp[3] =  ~(HAL_GPIO_ReadPin(ADDR_DIP0_Pin_GPIO_Port, ADDR_DIP0_Pin_Pin)) & 0x01;
	  MCC_Address_tmp[2] =  ~(HAL_GPIO_ReadPin(ADDR_DIP1_Pin_GPIO_Port, ADDR_DIP1_Pin_Pin)) & 0x01;
	  MCC_Address_tmp[1] =  ~(HAL_GPIO_ReadPin(ADDR_DIP2_Pin_GPIO_Port, ADDR_DIP2_Pin_Pin)) & 0x01;
	  MCC_Address_tmp[0] =  ~(HAL_GPIO_ReadPin(ADDR_DIP3_Pin_GPIO_Port, ADDR_DIP3_Pin_Pin)) & 0x01;

	 // MCC_Address[0] = (MCC_Address_tmp[3] << 3)|(MCC_Address_tmp[2] << 2)|(MCC_Address_tmp[1] << 1)|(MCC_Address_tmp[0] << 0);
	  MCC_R_Address = (MCC_Address_tmp[2] << 2)|(MCC_Address_tmp[1] << 1)|(MCC_Address_tmp[0] << 0);
}

void LED_Control(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin , uint16_t Staus){

  if(Staus == LED_On){
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
  }
  else if(Staus == LED_Off){
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
  }
}

void PUMP1_ON(void){
	HAL_GPIO_WritePin(PUMP1_AUTO_GPIO_Port, PUMP1_AUTO_Pin, GPIO_PIN_SET);
}
void PUMP1_OFF(void){
	HAL_GPIO_WritePin(PUMP1_AUTO_GPIO_Port, PUMP1_AUTO_Pin, GPIO_PIN_RESET);
}
void PUMP2_ON(void){
	HAL_GPIO_WritePin(PUMP2_AUTO_GPIO_Port, PUMP2_AUTO_Pin, GPIO_PIN_SET);
}
void PUMP2_OFF(void){
	HAL_GPIO_WritePin(PUMP2_AUTO_GPIO_Port, PUMP2_AUTO_Pin, GPIO_PIN_RESET);
}
void PUMP3_ON(void){
	HAL_GPIO_WritePin(PUMP3_AUTO_GPIO_Port, PUMP3_AUTO_Pin, GPIO_PIN_SET);
}
void PUMP3_OFF(void){
	HAL_GPIO_WritePin(PUMP3_AUTO_GPIO_Port, PUMP3_AUTO_Pin, GPIO_PIN_RESET);
}
void PUMP4_ON(void){
	HAL_GPIO_WritePin(PUMP4_AUTO_GPIO_Port, PUMP4_AUTO_Pin, GPIO_PIN_SET);
}
void PUMP4_OFF(void){
	HAL_GPIO_WritePin(PUMP4_AUTO_GPIO_Port, PUMP4_AUTO_Pin, GPIO_PIN_RESET);
}
void PUMP5_ON(void){
	HAL_GPIO_WritePin(PUMP5_AUTO_GPIO_Port, PUMP5_AUTO_Pin, GPIO_PIN_SET);
}
void PUMP5_OFF(void){
	HAL_GPIO_WritePin(PUMP5_AUTO_GPIO_Port, PUMP5_AUTO_Pin, GPIO_PIN_RESET);
}
void PUMP6_ON(void){
	HAL_GPIO_WritePin(PUMP6_AUTO_GPIO_Port, PUMP6_AUTO_Pin, GPIO_PIN_SET);
}
void PUMP6_OFF(void){
	HAL_GPIO_WritePin(PUMP6_AUTO_GPIO_Port, PUMP6_AUTO_Pin, GPIO_PIN_RESET);
}

/*
void Read_Pump_Ps(void){

    if(HAL_GPIO_ReadPin(PUMP1_PS_GPIO_Port, PUMP1_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status[0] = 1;
    }
    else{
      Pump_ps_Status[0] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP2_PS_GPIO_Port, PUMP2_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status[1] = 1;
    }
    else{
      Pump_ps_Status[1] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP3_PS_GPIO_Port, PUMP3_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status[2] = 1;
    }
    else{
      Pump_ps_Status[2] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP4_PS_GPIO_Port, PUMP4_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status[3] = 1;
    }
    else{
      Pump_ps_Status[3] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP5_PS_GPIO_Port, PUMP5_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status[4] = 1;
    }
    else{
      Pump_ps_Status[4] = 0;
    }
    if(HAL_GPIO_ReadPin(PUMP6_PS_GPIO_Port, PUMP6_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status[5] = 1;
    }
    else{
      Pump_ps_Status[5] = 0;
    }

}
*/

void Read_Pump_Ps(void){

	uint8_t Pump_ps_Status_Load[6];
	uint8_t Pump_ps_Status_Avr[6];

    if(HAL_GPIO_ReadPin(PUMP1_PS_GPIO_Port, PUMP1_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status_Load[0] = 1;
    }
    else{
      Pump_ps_Status_Load[0] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP2_PS_GPIO_Port, PUMP2_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status_Load[1] = 1;
    }
    else{
      Pump_ps_Status_Load[1] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP3_PS_GPIO_Port, PUMP3_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status_Load[2] = 1;
    }
    else{
      Pump_ps_Status_Load[2] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP4_PS_GPIO_Port, PUMP4_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status_Load[3] = 1;
    }
    else{
      Pump_ps_Status_Load[3] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP5_PS_GPIO_Port, PUMP5_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status_Load[4] = 1;
    }
    else{
      Pump_ps_Status_Load[4] = 0;
    }
    if(HAL_GPIO_ReadPin(PUMP6_PS_GPIO_Port, PUMP6_PS_Pin) == GPIO_PIN_RESET){
      Pump_ps_Status_Load[5] = 1;
    }
    else{
      Pump_ps_Status_Load[5] = 0;
    }

    for(int i=0; i< 6; i++){
    	Pump_ps_Status_tmp[i][Pump_ps_Load_Num] = Pump_ps_Status_Load[i];
    	Pump_ps_Status_Avr[i] = 0;
    }

    for(int i=0; i<6 ; i++){
    	for(int j=0; j< Save_Num; j++){
    		Pump_ps_Status_Avr[i] = Pump_ps_Status_Avr[i] + Pump_ps_Status_tmp[i][j];
    	}
    }

    for(int i=0; i< 6; i++){
    	if(Pump_ps_Status_Avr[i] >  (Save_Num / 2)){
    		Pump_ps_Status[i] = 1;
    	}
    	else{
    		Pump_ps_Status[i] = 0;
    	}
    }

    if(Pump_ps_Load_Num ==  Save_Num){
    	Pump_ps_Load_Num = 0;
    }
    else{
    	Pump_ps_Load_Num++;
    }

}

/*
void Read_Pump_Ck(void){

 if(HAL_GPIO_ReadPin(PUMP1_CHECK_GPIO_Port, PUMP1_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status[0] = 1;
    }
    else{
      Pump_ck_Status[0] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP2_CHECK_GPIO_Port, PUMP2_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status[1] = 1;
    }
    else{
      Pump_ck_Status[1] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP3_CHECK_GPIO_Port, PUMP3_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status[2] = 1;
    }
    else{
      Pump_ck_Status[2] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP4_CHECK_GPIO_Port, PUMP4_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status[3] = 1;
    }
    else{
      Pump_ck_Status[3] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP5_CHECK_GPIO_Port, PUMP5_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status[4] = 1;
    }
    else{
      Pump_ck_Status[4] = 0;
    }
    if(HAL_GPIO_ReadPin(PUMP6_CHECK_GPIO_Port, PUMP6_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status[5] = 1;
    }
    else{
      Pump_ck_Status[5] = 0;
    }
}
*/

void Read_Pump_Ck(void){

	uint8_t Pump_ck_Status_Load[6];
	uint8_t Pump_ck_Status_Avr[6];

    if(HAL_GPIO_ReadPin(PUMP1_CHECK_GPIO_Port, PUMP1_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status_Load[0] = 1;
    }
    else{
      Pump_ck_Status_Load[0] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP2_CHECK_GPIO_Port, PUMP2_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status_Load[1] = 1;
    }
    else{
      Pump_ck_Status_Load[1] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP3_CHECK_GPIO_Port, PUMP3_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status_Load[2] = 1;
    }
    else{
      Pump_ck_Status_Load[2] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP4_CHECK_GPIO_Port, PUMP4_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status_Load[3] = 1;
    }
    else{
      Pump_ck_Status_Load[3] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP5_CHECK_GPIO_Port, PUMP5_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status_Load[4] = 1;
    }
    else{
      Pump_ck_Status_Load[4] = 0;
    }
    if(HAL_GPIO_ReadPin(PUMP6_CHECK_GPIO_Port, PUMP6_CHECK_Pin) == GPIO_PIN_RESET){
      Pump_ck_Status_Load[5] = 1;
    }
    else{
      Pump_ck_Status_Load[5] = 0;
    }

    for(int i=0; i< 6; i++){
    	Pump_ck_Status_tmp[i][Pump_ck_Load_Num] = Pump_ck_Status_Load[i];
    	Pump_ck_Status_Avr[i] = 0;
    }

    for(int i=0; i<6 ; i++){
    	for(int j=0; j< Save_Num; j++){
    		Pump_ck_Status_Avr[i] = Pump_ck_Status_Avr[i] + Pump_ck_Status_tmp[i][j];
    	}
    }

    for(int i=0; i< 6; i++){
    	if(Pump_ck_Status_Avr[i] >  (Save_Num / 2)){
    		Pump_ck_Status[i] = 1;
    	}
    	else{
    		Pump_ck_Status[i] = 0;
    	}
    }

    if(Pump_ck_Load_Num ==  Save_Num){
    	Pump_ck_Load_Num = 0;
    }
    else{
    	Pump_ck_Load_Num++;
    }

}


/*
void Read_Pump_Err(void){

    if(HAL_GPIO_ReadPin(PUMP1_PS_ERR_GPIO_Port, PUMP1_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status[0] = 1;
    }
    else{
      Pump_err_Status[0] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP2_PS_ERR_GPIO_Port, PUMP2_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status[1] = 1;
    }
    else{
      Pump_err_Status[1] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP3_PS_ERR_GPIO_Port, PUMP3_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status[2] = 1;
    }
    else{
      Pump_err_Status[2] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP4_PS_ERR_GPIO_Port, PUMP4_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status[3] = 1;
    }
    else{
      Pump_err_Status[3] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP5_PS_ERR_GPIO_Port, PUMP5_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status[4] = 1;
    }
    else{
      Pump_err_Status[4] = 0;
    }
    if(HAL_GPIO_ReadPin(PUMP6_PS_ERR_GPIO_Port, PUMP6_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status[5] = 1;
    }
    else{
      Pump_err_Status[5] = 0;
    }
}
*/

void Read_Pump_Err(void){

	uint8_t Pump_err_Status_Load[6];
	uint8_t Pump_err_Status_Avr[6];

    if(HAL_GPIO_ReadPin(PUMP1_PS_ERR_GPIO_Port, PUMP1_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status_Load[0] = 1;
    }
    else{
      Pump_err_Status_Load[0] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP2_PS_ERR_GPIO_Port, PUMP2_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status_Load[1] = 1;
    }
    else{
      Pump_err_Status_Load[1] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP3_PS_ERR_GPIO_Port, PUMP3_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status_Load[2] = 1;
    }
    else{
      Pump_err_Status_Load[2] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP4_PS_ERR_GPIO_Port, PUMP4_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status_Load[3] = 1;
    }
    else{
      Pump_err_Status_Load[3] = 0;
    }

    if(HAL_GPIO_ReadPin(PUMP5_PS_ERR_GPIO_Port, PUMP5_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status_Load[4] = 1;
    }
    else{
      Pump_err_Status_Load[4] = 0;
    }
    if(HAL_GPIO_ReadPin(PUMP6_PS_ERR_GPIO_Port, PUMP6_PS_ERR_Pin) == GPIO_PIN_RESET){
      Pump_err_Status_Load[5] = 1;
    }
    else{
      Pump_err_Status_Load[5] = 0;
    }

    for(int i=0; i< 6; i++){
    	Pump_err_Status_tmp[i][Pump_err_Load_Num] = Pump_err_Status_Load[i];
    	Pump_err_Status_Avr[i] = 0;
    }

    for(int i=0; i<6 ; i++){
    	for(int j=0; j< Save_Num; j++){
    		Pump_err_Status_Avr[i] = Pump_err_Status_Avr[i] + Pump_err_Status_tmp[i][j];
    	}
    }

    for(int i=0; i< 6; i++){
    	if(Pump_err_Status_Avr[i] >  (Save_Num / 2)){
    		Pump_err_Status[i] = 1;
    	}
    	else{
    		Pump_err_Status[i] = 0;
    	}
    }

    if(Pump_err_Load_Num ==  Save_Num){
    	Pump_err_Load_Num = 0;
    }
    else{
    	Pump_err_Load_Num++;
    }

}

void Set_Relay_Avr(void){
  int ck_val;
  if(RY_Status[0] == 1){
    ck_val = 0;
  }
  for(int i=0 ; i< (RY_Status_Avr_Num - 1); i++){
    for(int j=0; j<8 ;j++){
      RY_Status_Avr[j][i] = RY_Status_Avr[j][i+1];
    }
  }
  for(int j=0; j<8 ;j++){
    RY_Status_Avr[j][RY_Status_Avr_Num - 1 ] = RY_Status[j];
  }


  for(int j=0; j<8 ;j++){
    ck_val = 0;
    for(int i=0 ; i< RY_Status_Avr_Num; i++){
    	ck_val += RY_Status_Avr[j][i];
    }

    if(ck_val > (RY_Status_Avr_Num/ 2)){
      RY_Status_Avr_Val[j] = 1;
    }
    else{
      RY_Status_Avr_Val[j] = 0;
    }

  }



  if(RY_Status_Avr_Val[0] == 1){
    PUMP1_ON();
  }
  else{
    PUMP1_OFF();
  }

  if(RY_Status_Avr_Val[1] == 1){
    PUMP2_ON();
  }
  else{
    PUMP2_OFF();
  }

  if(RY_Status_Avr_Val[2] == 1){
    PUMP3_ON();
  }
  else{
    PUMP3_OFF();
  }

  if(RY_Status_Avr_Val[3] == 1){
    PUMP4_ON();
  }
  else{
    PUMP4_OFF();
  }

  if(RY_Status_Avr_Val[4] == 1){
    PUMP5_ON();
  }
  else{
    PUMP5_OFF();
  }

  if(RY_Status_Avr_Val[5] == 1){
    PUMP6_ON();
  }
  else{
    PUMP6_OFF();
  }


}
void Set_Relay(void){

  if(RY_Status[0] == 1){
      PUMP1_ON();
    }
    else{
      PUMP1_OFF();
    }

    if(RY_Status[0] == 1){
      PUMP1_ON();
    }
    else{
      PUMP1_OFF();
    }

    if(RY_Status[1] == 1){
      PUMP2_ON();
    }
    else{
      PUMP2_OFF();
    }

    if(RY_Status[2] == 1){
      PUMP3_ON();
    }
    else{
      PUMP3_OFF();
    }

    if(RY_Status[3] == 1){
      PUMP4_ON();
    }
    else{
      PUMP4_OFF();
    }

    if(RY_Status[4] == 1){
      PUMP5_ON();
    }
    else{
      PUMP5_OFF();
    }

    if(RY_Status[5] == 1){
      PUMP6_ON();
    }
    else{
      PUMP6_OFF();
    }
}

int Uart2_crc = 0;
int Uart_crc = 0;


void SW_Com(void){

  // CRC 체크
  // 전송할 중계기 정보를 확인한다.

  Uart2_crc = 0;

  for(int i = 0; i <SUB_UART_buf_count_Save - 5 ; i++){
	Uart2_crc = Uart2_crc ^ SUB_UART_RX_buf[i+2];
  }

  if(Uart2_crc == SUB_UART_RX_buf[SUB_UART_buf_count_Save - 3]){
	//0x51 , 'Q' ,중계기 정보 요청 (UI에서 설정한 값만)

	  if(SUB_UART_RX_buf[3] == 0x51){    // Report request : 'Q' ,'0x51'
		  if(SUB_UART_RX_buf[4] == 0x52){        // Relay board : '0x52'
			  if(SUB_UART_RX_buf[5] == MCC_R_Address){      // MCC_R_Address[0]
				  Relay_board_Q();

			}
		  }
	  }
	  else if(SUB_UART_RX_buf[3] == 0x53){       //Setting : 'S' '0x53'
		  if(SUB_UART_RX_buf[4] == 0x52){        // Relay board : '0x52'
			  if(SUB_UART_RX_buf[5] == MCC_R_Address){      // MCC_R_Address[0]
				  Relay_board_S();
			  }
		  }
		}
	  else if(SUB_UART_RX_buf[3] == 0x56){       //Version : 'V' '0x56'
		  if(SUB_UART_RX_buf[4] == 0x52){        // Relay board : '0x52'
			  if(SUB_UART_RX_buf[5] == MCC_R_Address){      // MCC_R_Address[0]
				  SUB_Com_SW_V();
			  }
		  }
		}

  }

  SUB_UART_Receive_complete = 0;

}

void Relay_board_Q(void){

	  SUB_UART_TX_buf[0] = 0x53;    //S
	  SUB_UART_TX_buf[1] = 0x54;    //T
	  SUB_UART_TX_buf[2] = 0x52;    //R
	  SUB_UART_TX_buf[3] = 0x52;    //R
	  SUB_UART_TX_buf[4] = MCC_R_Address;    //address
	  SUB_UART_TX_buf[5] = 0x0A;    //version 10 -> 1.0
	  SUB_UART_TX_buf[6] = 0x00;    //Relay 정보 1 ~8
	  SUB_UART_TX_buf[7] = 0x00;    //Pump_ps 정보 1 ~8
	  SUB_UART_TX_buf[8] = 0x00;    //Pump_ck 정보 1 ~8
	  SUB_UART_TX_buf[9] = 0x00;    //Pump_err 정보 1 ~8
	  SUB_UART_TX_buf[10] = 0x00;   //dummy1
	  SUB_UART_TX_buf[11] = 0x00;   //dummy2
	  SUB_UART_TX_buf[12] = 0x00;   //CRC
	  SUB_UART_TX_buf[13] = 0x45;   //E
	  SUB_UART_TX_buf[14] = 0x44;   //D

	  SUB_UART_TX_buf[6] = (RY_Status[0] << 7)|(RY_Status[1] << 6)|(RY_Status[2] << 5)|
						(RY_Status[3] << 4)|(RY_Status[4] << 3)|(RY_Status[5] << 2)|(RY_Status[6] << 1);
	  SUB_UART_TX_buf[7] = (Pump_ps_Status[0] << 7)|(Pump_ps_Status[1] << 6)|(Pump_ps_Status[2] << 5)|
						(Pump_ps_Status[3] << 4)|(Pump_ps_Status[4] << 3)|(Pump_ps_Status[5] << 2);
	  SUB_UART_TX_buf[8] = (Pump_ck_Status[0] << 7)|(Pump_ck_Status[1] << 6)|(Pump_ck_Status[2] << 5)|
						(Pump_ck_Status[3] << 4)|(Pump_ck_Status[4] << 3)|(Pump_ck_Status[5] << 2);
	  SUB_UART_TX_buf[9] = (Pump_err_Status[0] << 7)|(Pump_err_Status[1] << 6)|(Pump_err_Status[2] << 5)|
						(Pump_err_Status[3] << 4)|(Pump_err_Status[4] << 3)|(Pump_err_Status[5] << 2);
	  Uart_crc = 0;

	  for(int i = 0; i <MCC_Relay_tx_buf_len - 5 ; i++){
		Uart_crc = Uart_crc ^ SUB_UART_TX_buf[i+2];
	  }
	  SUB_UART_TX_buf[MCC_Relay_tx_buf_len - 3] = Uart_crc;

	  HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_SET);
	  HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_SET);

	  HAL_Delay(1);
	  if(HAL_UART_Transmit(&hlpuart1, SUB_UART_TX_buf, MCC_Relay_tx_buf_len, 1000)!= HAL_OK)
		  {
			Error_Handler();
		  }

	  HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_RESET);

}

void Relay_board_S(void){
	for(int i=0;i<8;i++){
		RY_Status[i] = ( SUB_UART_RX_buf[6] >> (7 - i)) & 0x01;
	}
}

void SUB_Com_SW_V(void){

	uint8_t Uart_crc;

    SUB_UART_TX_buf[0] = 0x53;    //S
    SUB_UART_TX_buf[1] = 0x54;    //T
    SUB_UART_TX_buf[2] = 0x53;    //S
    SUB_UART_TX_buf[3] = 0x76;    //v
    SUB_UART_TX_buf[4] = 0x52;    //R
    SUB_UART_TX_buf[5] = MCC_R_Address;    //address
    SUB_UART_TX_buf[6] = F_Version_Year;
    SUB_UART_TX_buf[7] = F_Version_Month;
    SUB_UART_TX_buf[8] = F_Version_Day;
    SUB_UART_TX_buf[9] = F_Version_Hour;
    SUB_UART_TX_buf[10] = F_Version_Min;
    SUB_UART_TX_buf[11] = F_Version_Sec;
    SUB_UART_TX_buf[12] = 0x00;   //CRC
    SUB_UART_TX_buf[13] = 0x45;   //E
    SUB_UART_TX_buf[14] = 0x44;   //D


    for(int i = 0; i <Sub_MCC_V_length - 5 ; i++){
      Uart_crc = Uart_crc ^ SUB_UART_TX_buf[i+2];
    }
    SUB_UART_TX_buf[Sub_MCC_V_length-3] = Uart_crc;


	HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_SET);
	HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_SET);

	HAL_Delay(1);
	if(HAL_UART_Transmit(&hlpuart1, SUB_UART_TX_buf, Sub_MCC_V_length, 1000)!= HAL_OK)
	  {
		Error_Handler();
	  }

	HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_RESET);

}
/* USER CODE END 4 */

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
#ifdef USE_FULL_ASSERT
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
