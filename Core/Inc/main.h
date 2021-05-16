/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_gpio.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DI5_Pin GPIO_PIN_13
#define DI5_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOA
#define AI1_Pin GPIO_PIN_2
#define AI1_GPIO_Port GPIOA
#define AI2_Pin GPIO_PIN_3
#define AI2_GPIO_Port GPIOA
#define AI3_Pin GPIO_PIN_4
#define AI3_GPIO_Port GPIOA
#define VREF_Pin GPIO_PIN_5
#define VREF_GPIO_Port GPIOA
#define I2_Pin GPIO_PIN_6
#define I2_GPIO_Port GPIOA
#define I1_Pin GPIO_PIN_7
#define I1_GPIO_Port GPIOA
#define VO_Pin GPIO_PIN_0
#define VO_GPIO_Port GPIOB
#define VTMP_Pin GPIO_PIN_1
#define VTMP_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_10
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_11
#define SDA_GPIO_Port GPIOB
#define CLK1_Pin GPIO_PIN_12
#define CLK1_GPIO_Port GPIOB
#define CLK2_Pin GPIO_PIN_13
#define CLK2_GPIO_Port GPIOB
#define D165_Pin GPIO_PIN_14
#define D165_GPIO_Port GPIOB
#define D595_Pin GPIO_PIN_15
#define D595_GPIO_Port GPIOB
#define REDE_Pin GPIO_PIN_8
#define REDE_GPIO_Port GPIOA
#define DO1_Pin GPIO_PIN_15
#define DO1_GPIO_Port GPIOA
#define DO2_Pin GPIO_PIN_3
#define DO2_GPIO_Port GPIOB
#define DO3_Pin GPIO_PIN_4
#define DO3_GPIO_Port GPIOB
#define DO4_Pin GPIO_PIN_5
#define DO4_GPIO_Port GPIOB
#define DI1_Pin GPIO_PIN_6
#define DI1_GPIO_Port GPIOB
#define DI2_Pin GPIO_PIN_7
#define DI2_GPIO_Port GPIOB
#define DI3_Pin GPIO_PIN_8
#define DI3_GPIO_Port GPIOB
#define DI4_Pin GPIO_PIN_9
#define DI4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
