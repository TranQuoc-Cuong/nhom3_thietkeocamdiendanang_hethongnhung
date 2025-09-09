/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "string.h"
#include "math.h"
#include "uart.h"
#include "Ringbuffer.h"
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
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

#define CMD_LIGHT "light"

uint8_t data_rx;
uint8_t flag = 0;


void get_adc_value_channel(uint32_t channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = channel;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
	
}
double current_test;

float Current_getValue(){
	get_adc_value_channel(ADC_CHANNEL_1);
  uint16_t adc_current_max = 0;
  uint16_t adc_current_min = 4095;
  uint16_t time_loop =0;
  uint16_t adc_current_value;
  HAL_ADC_Start(&hadc1);
  while (time_loop < 500)
  {
    HAL_ADC_PollForConversion(&hadc1, 1000);
    adc_current_value = HAL_ADC_GetValue(&hadc1);

    if (adc_current_value  > adc_current_max){
      adc_current_max = adc_current_value;
    }

    if (adc_current_value  < adc_current_min){
      adc_current_min = adc_current_value;
    }
    time_loop ++;
  }
  HAL_ADC_Stop(&hadc1);
  
  uint16_t adc_currrent_pp;
  float current_rms;

  adc_currrent_pp = adc_current_max - adc_current_min;
  current_rms = (adc_currrent_pp * 3.3 /4095)*150.553/180;  // RMS equation
  return current_rms;
}


float voltage_test;

uint16_t period = 20000;
uint16_t sensitivity = 920;

int getZeroPoint()
{
	get_adc_value_channel(ADC_CHANNEL_2);
	uint32_t Vsum = 0;
	uint32_t measurements_count = 0;
	HAL_ADC_Start(&hadc1);
	while(measurements_count < period)
	{

		HAL_ADC_PollForConversion(&hadc1, 100);
		Vsum += HAL_ADC_GetValue(&hadc1);
		measurements_count++;
	}
	HAL_ADC_Stop(&hadc1);
	
	return Vsum / measurements_count;
}

float getRmsVoltage()
{
	get_adc_value_channel(ADC_CHANNEL_2);
	double readingVoltage = 0.0;
	
		int zeroPoint = getZeroPoint();
		
		int32_t Vnow = 0;
		uint32_t Vsum = 0;
		uint32_t measurements_count = 0;
		HAL_ADC_Start(&hadc1);
		while(measurements_count < period)
		{
			HAL_ADC_PollForConversion(&hadc1, 100);
			Vnow = HAL_ADC_GetValue(&hadc1) - zeroPoint;
			Vsum += (Vnow * Vnow);
			measurements_count++;
		}
		HAL_ADC_Stop(&hadc1);
		readingVoltage = sqrt(Vsum / measurements_count) / 4095 * 3.3 * sensitivity;

	return readingVoltage;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart -> Instance == huart1.Instance)
	{
		receive_rx(data_rx);
		HAL_UART_Receive_IT(&huart1, &data_rx, 1);
	}
	flag = 1;
}

uint8_t data_esp32[64];
uint8_t data_esp32_index = 0;
uint8_t time;

void readLine_Uart()
{
	uint8_t data = uart_read();
	while(data != '\n')
	{
		data_esp32[data_esp32_index] = data;
		data = uart_read();
		data_esp32_index++;
	}
	data_esp32[data_esp32_index] = '\0';
	data_esp32_index = 0;
	flag = 0;
}
uint16_t light_turn;

uint16_t cmd_handler(char *str, char *manhandang)
{
	char *pCmd = strstr(str, manhandang);
	if(pCmd != NULL)
	{
		pCmd += strlen(manhandang);
		if(*pCmd == ':')
		{
			pCmd++;
			uint32_t number = NULL;
			while(1)
			{
				if(*pCmd >= '0' && *pCmd <= '9')
				{
					number *= 10;
					number += *pCmd - '0';
				}
				else if(*pCmd == ';')
				{
					break;
				}
				pCmd++;
			}
	return number; 		
		}
	}
	return -1;
}




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
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1, &data_rx, 1);
	uart_init();
	HAL_ADCEx_Calibration_Start(&hadc1);
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		current_test = Current_getValue();
		voltage_test = getRmsVoltage();
		if(flag)
		{
			readLine_Uart();
			light_turn = cmd_handler((char *)data_esp32, CMD_LIGHT);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, (GPIO_PinState)light_turn);
		}
		if(HAL_GetTick() - time >= 4000)
		{
		char trans[32];
		int len = sprintf(trans, "voltage:%d;current:%.2f;\n",(int)voltage_test, current_test);
		HAL_UART_Transmit(&huart1, (uint8_t *)trans, len, 300);
		time = HAL_GetTick();
		}
			
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
