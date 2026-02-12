/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

#define LED_On  1
#define LED_Off 0

#define SUB_UART_buf_len 2048
extern uint8_t SUB_UART_TX_buf[SUB_UART_buf_len] ;
extern uint8_t SUB_UART_RX_buf[SUB_UART_buf_len];
extern uint8_t SUB_UART_Tx_buf_tmp[SUB_UART_buf_len];
extern uint8_t SUB_UART_RX_buf_tmp[SUB_UART_buf_len];
extern uint8_t SUB_UART_State ;					// Define reception start state
extern int SUB_UART_buf_count;					// Save the length of received data after starting reception
extern int SUB_UART_buf_count_tmp;				// Variable to store the length of the received data before initializing it when reception is completed
extern uint8_t SUB_UART_Receive_complete;		// Variable indicating that reception has been completed
extern int SUB_UART_buf_count_Save;

#define MCC_Relay_tx_buf_len 15
#define Sub_MCC_V_length      15

#define RY_Status_Avr_Num 6
extern uint8_t RY_Status_Avr[8][RY_Status_Avr_Num];
extern uint8_t RY_Status_Avr_Val[8];

extern uint8_t RY_Status[8] ;
extern uint8_t Pump_ps_Status[8] ;
extern uint8_t Pump_ck_Status[8] ;
extern uint8_t Pump_err_Status[8] ;

void Compile_Date(void);
void Read_Address(void);
void LED_Control(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin , uint16_t Staus);

void PUMP1_ON(void);
void PUMP1_OFF(void);
void PUMP2_ON(void);
void PUMP2_OFF(void);
void PUMP3_ON(void);
void PUMP3_OFF(void);
void PUMP4_ON(void);
void PUMP4_OFF(void);
void PUMP5_ON(void);
void PUMP5_OFF(void);
void PUMP6_ON(void);
void PUMP6_OFF(void);


void Read_Pump_Ps(void);
void Read_Pump_Ck(void);
void Read_Pump_Err(void);
void Set_Relay(void);
void Set_Relay_Avr(void);
void SW_Com(void);

void Relay_board_Q(void);
void Relay_board_S(void);
void SUB_Com_SW_V(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PUMP1_PS_ERR_Pin GPIO_PIN_13
#define PUMP1_PS_ERR_GPIO_Port GPIOC
#define PUMP1_CHECK_Pin GPIO_PIN_1
#define PUMP1_CHECK_GPIO_Port GPIOC
#define PUMP2_CHECK_Pin GPIO_PIN_2
#define PUMP2_CHECK_GPIO_Port GPIOC
#define PUMP3_CHECK_Pin GPIO_PIN_3
#define PUMP3_CHECK_GPIO_Port GPIOC
#define PUMP4_CHECK_Pin GPIO_PIN_0
#define PUMP4_CHECK_GPIO_Port GPIOA
#define PUMP5_CHECK_Pin GPIO_PIN_1
#define PUMP5_CHECK_GPIO_Port GPIOA
#define PUMP6_CHECK_Pin GPIO_PIN_2
#define PUMP6_CHECK_GPIO_Port GPIOA
#define PUMP1_AUTO_Pin GPIO_PIN_5
#define PUMP1_AUTO_GPIO_Port GPIOA
#define PUMP2_AUTO_Pin GPIO_PIN_6
#define PUMP2_AUTO_GPIO_Port GPIOA
#define PUMP3_AUTO_Pin GPIO_PIN_7
#define PUMP3_AUTO_GPIO_Port GPIOA
#define PUMP4_AUTO_Pin GPIO_PIN_4
#define PUMP4_AUTO_GPIO_Port GPIOC
#define PUMP5_AUTO_Pin GPIO_PIN_5
#define PUMP5_AUTO_GPIO_Port GPIOC
#define PUMP6_AUTO_Pin GPIO_PIN_0
#define PUMP6_AUTO_GPIO_Port GPIOB
#define RS485_DE_Pin GPIO_PIN_1
#define RS485_DE_GPIO_Port GPIOB
#define RS485_RE_Pin GPIO_PIN_2
#define RS485_RE_GPIO_Port GPIOB
#define ERR_LED_Pin GPIO_PIN_12
#define ERR_LED_GPIO_Port GPIOB
#define RUN_LED_Pin GPIO_PIN_13
#define RUN_LED_GPIO_Port GPIOB
#define ADDR_DIP3_Pin_Pin GPIO_PIN_7
#define ADDR_DIP3_Pin_GPIO_Port GPIOC
#define ADDR_DIP2_Pin_Pin GPIO_PIN_8
#define ADDR_DIP2_Pin_GPIO_Port GPIOC
#define ADDR_DIP1_Pin_Pin GPIO_PIN_9
#define ADDR_DIP1_Pin_GPIO_Port GPIOC
#define ADDR_DIP0_Pin_Pin GPIO_PIN_8
#define ADDR_DIP0_Pin_GPIO_Port GPIOA
#define PUMP6_PS_Pin GPIO_PIN_15
#define PUMP6_PS_GPIO_Port GPIOA
#define PUMP6_PS_ERR_Pin GPIO_PIN_10
#define PUMP6_PS_ERR_GPIO_Port GPIOC
#define PUMP5_PS_Pin GPIO_PIN_11
#define PUMP5_PS_GPIO_Port GPIOC
#define PUMP5_PS_ERR_Pin GPIO_PIN_12
#define PUMP5_PS_ERR_GPIO_Port GPIOC
#define PUMP4_PS_Pin GPIO_PIN_2
#define PUMP4_PS_GPIO_Port GPIOD
#define PUMP4_PS_ERR_Pin GPIO_PIN_4
#define PUMP4_PS_ERR_GPIO_Port GPIOB
#define PUMP3_PS_Pin GPIO_PIN_5
#define PUMP3_PS_GPIO_Port GPIOB
#define PUMP3_PS_ERR_Pin GPIO_PIN_6
#define PUMP3_PS_ERR_GPIO_Port GPIOB
#define PUMP2_PS_Pin GPIO_PIN_7
#define PUMP2_PS_GPIO_Port GPIOB
#define PUMP2_PS_ERR_Pin GPIO_PIN_8
#define PUMP2_PS_ERR_GPIO_Port GPIOB
#define PUMP1_PS_Pin GPIO_PIN_9
#define PUMP1_PS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
