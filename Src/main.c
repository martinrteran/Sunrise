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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define MAX_SECONDS 60
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim10;

/* USER CODE BEGIN PV */
volatile static uint8_t second=0;
volatile static uint8_t triggerUp = 0;
volatile static uint8_t triggerDown = 0;
volatile static uint8_t triggerMinus = 0;
volatile static uint8_t triggerPlus = 0;
volatile static uint8_t triggerStop = 0;
volatile static uint8_t activeFlag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM10_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int debouncer(volatile uint8_t* button_int, GPIO_TypeDef* GPIO_port, uint16_t GPIO_number);
typedef enum DISPLAY_STATE {CLOCK,ALARM} display_state_t;
void ToggleClockPointer(clock_t** pointer, clock_t* clk, clock_t* alarm, display_state_t* disp);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	clock_t clock = Clock(23,50); // 12h 00m
	clock_t alarm = Clock(23,00);
	clock_t* clck_p = &clock;
	display_state_t disp_state=CLOCK;

	uint8_t isAlarmActive = 0;
	uint8_t isTime = 0;
	int8_t cursor = -1;
	uint8_t editing = 0;
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
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim10);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	const int tim_PWM_Period = htim2.Init.Period;
	SSD1306_Init();
	SSD1306_Clear();
	int x=(SSD1306_HEIGHT-26)/2,y=(SSD1306_WIDTH-16*5)/2;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(second && !editing){
		  second =0;//MAX_SECONDS;
		  AddMinute(&clock);
		  if(isAlarmActive && clock==alarm)
			  isTime = 1;

	  }
	  if(debouncer(&triggerStop, STOP_GPIO_Port, STOP_Pin)){

		  if(isAlarmActive && isTime){
			  isTime = 0;
			  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		  }else
			  isAlarmActive = !isAlarmActive;
	  }
	  if(isTime){
		  disp_state = CLOCK;
		  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,tim_PWM_Period);
	  }else{
		  if(debouncer(&triggerPlus,PLUS_GPIO_Port,PLUS_Pin)){
			  if(!editing){
				  disp_state = disp_state==CLOCK? ALARM:CLOCK;
				  cursor = -1;
			  }else{
				  IncreaseOnCursor(clck_p, cursor, 1);
			  }
		  }
		  if(debouncer(&triggerMinus,MINUS_GPIO_Port,MINUS_Pin)){
			  if(!editing){
				  disp_state = disp_state==CLOCK? ALARM:CLOCK;
				  cursor = -1;
			  }else{
				  IncreaseOnCursor(clck_p, cursor, -1);
			  }
		  }
		  if(debouncer(&triggerUp,UP_GPIO_Port,UP_Pin)){
			  --cursor;

			  if(cursor<=-1){
				  editing = 0;
				  //isAlarmActive = 1;
			  }
			  else if(cursor<=4){
				  editing = 1;
				  isAlarmActive = 0;
			  }


		  }
		  if(debouncer(&triggerDown,DOWN_GPIO_Port,DOWN_Pin)){
			  ++cursor;
			  if(cursor>=4){
				  editing = 0;
				 // isAlarmActive = 1;
			  }
			  else if(cursor>=-1){
				  editing = 1;
				  isAlarmActive = 0;
			  }

		  }
	  }
	  if(cursor<-1) cursor  = 3;
	  else if(cursor>4) cursor = 0;

	  if(!isAlarmActive)
		  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);

	  time_t diff = DifferenceMinutes(alarm, clock);
	  if(isAlarmActive &&  diff>=0 && diff<=30){
		 diff = 30 - diff;
		 diff = (diff/30.0f)*tim_PWM_Period;
		 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,diff);
	  }




	  SSD1306_Fill(SSD1306_COLOR_BLACK);
	  if(isAlarmActive)
		  SSD1306_DrawFilledCircle(SSD1306_WIDTH-8, SSD1306_HEIGHT-8, 5, SSD1306_COLOR_WHITE);
	  if(isTime){
		  SSD1306_GotoXY(0, 0);
		  SSD1306_Puts("RING",&Font_7x10,SSD1306_COLOR_WHITE);
		  //SSD1306_DrawRectangle(x, y, SSD1306_WIDTH*3/4, SSD1306_HEIGHT*3/4, SSD1306_COLOR_WHITE);
	  }else if(disp_state == ALARM){
		  SSD1306_GotoXY(0, 0);
		  SSD1306_Puts("alarm",&Font_7x10,SSD1306_COLOR_WHITE);
	  }else{
		  SSD1306_GotoXY(0, 0);
		  SSD1306_Puts("clock",&Font_7x10,SSD1306_COLOR_WHITE);
	  }

	  clck_p = disp_state==ALARM ? &alarm: &clock;

	  PrintClockLED(*clck_p, cursor, x, y);

	  activeFlag = isTime;
  }
  	HAL_TIM_Base_Stop_IT(&htim10);
  	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 99;
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
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 7999;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 9999;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PLUS_Pin MINUS_Pin */
  GPIO_InitStruct.Pin = PLUS_Pin|MINUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : STOP_Pin DOWN_Pin UP_Pin */
  GPIO_InitStruct.Pin = STOP_Pin|DOWN_Pin|UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance != TIM10) return;
	++second;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(activeFlag && GPIO_Pin != STOP_Pin) return;

	switch(GPIO_Pin){
	case STOP_Pin:
		triggerStop = 1; break;
	case PLUS_Pin:
		triggerPlus = 1; break;
	case MINUS_Pin:
		triggerMinus = 1;break;
	case UP_Pin:
		triggerUp = 1; break;
	case DOWN_Pin:
		triggerDown = 1;break;
	}
}
int debouncer(volatile uint8_t* button_int, GPIO_TypeDef* GPIO_port, uint16_t GPIO_number){
	static uint8_t button_count=0;
	static int time=0;

	if (*button_int==1){
		if (button_count==0) {
			time=HAL_GetTick();
			button_count++;
		}
		if (HAL_GetTick()-time>=20){
			time=HAL_GetTick();
			if (HAL_GPIO_ReadPin(GPIO_port, GPIO_number)!=1){// If it is pressed down, keep it that way
				button_count=1;
			}
			else{// if it is not then start counting every 20 milliseconds
				button_count++;
			}
			if (button_count==4){ //Periodo antirebotes
				button_count=0;
				*button_int=0;
				return 1;
			}
		}
	}
	return 0;
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
