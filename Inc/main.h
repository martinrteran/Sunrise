/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TimeHandler.h"
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWM_Pin GPIO_PIN_2
#define PWM_GPIO_Port GPIOA
#define PLUS_Pin GPIO_PIN_6
#define PLUS_GPIO_Port GPIOD
#define PLUS_EXTI_IRQn EXTI9_5_IRQn
#define MINUS_Pin GPIO_PIN_7
#define MINUS_GPIO_Port GPIOD
#define MINUS_EXTI_IRQn EXTI9_5_IRQn
#define STOP_Pin GPIO_PIN_3
#define STOP_GPIO_Port GPIOB
#define STOP_EXTI_IRQn EXTI3_IRQn
#define DOWN_Pin GPIO_PIN_4
#define DOWN_GPIO_Port GPIOB
#define DOWN_EXTI_IRQn EXTI4_IRQn
#define UP_Pin GPIO_PIN_5
#define UP_GPIO_Port GPIOB
#define UP_EXTI_IRQn EXTI9_5_IRQn
#define SCL_Pin GPIO_PIN_6
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
