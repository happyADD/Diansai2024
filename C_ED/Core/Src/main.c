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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Variable_Includes.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
Servo_Init();
  setup_kinematics(105,173,130,0);
  tx_data[0]=0;
//  HAL_UART_Receive_DMA(&huart1, rx_data, 8);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  	  HAL_UART_Receive_DMA(&huart1, rx_data, 10);
	 
	  if(t)
	  {
		  Position.x = 0;
		  Position.y = 150;
		  Position.z = 0;
		  HAL_Delay(1000);
		  Position.x = X1;
		  Position.y = Y1;
		  Position.z = 0;
		  HAL_Delay(1500);
		  if(((Position.y<211)&&(Position.y>89))&&((Position.x<61)&&(Position.x>-61))) Position.z = 40;
		  else if(((Position.y<231)&&(Position.y>74))&&((Position.x<111)&&(Position.x>-111))) Position.z = 55;
		  else Position.z = 0;
		  HAL_Delay(500);
		  PWM5 = 20000;
		  HAL_Delay(500);
		  Position.z = 0;
		  HAL_Delay(500);
		  Position.x = 0;
		  Position.y = 150;
		  Position.z = 0;
		  HAL_Delay(1000);
		  Position.x = X2;
		  Position.y = Y2;
		  HAL_Delay(1000);
		  if(((Position.y<211)&&(Position.y>89))&&((Position.x<61)&&(Position.x>-61))) Position.z  = 40;
			  else if(((Position.y<231)&&(Position.y>74))&&((Position.x<111)&&(Position.x>-111))) Position.z = 55;
		  else Position.z = 0;
		  HAL_Delay(500);
		  PWM5 = 0;
		  HAL_Delay(100);
		  Position.z = 0;
		  HAL_Delay(500);
		  Position.x = -100;
		  Position.y = 0;
		  HAL_Delay(1000);
		  t=0;
	  }
	HAL_Delay(10);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	if(htim == &htim3)//计数定时器中断
	{
		caculate(Position.x,Position.y,z);
		Servo_Set();
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, PWM1);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, PWM2);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, PWM3);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, PWM4);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, PWM5);

	}
}

//// 串口中断接收回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{			 
		if(!t)
		{
			for(int i=0;i<10;++i)
			{
				if(rx_data[(i+7)%10] == 'E' && rx_data[(i+8)%10] == 'O' && rx_data[(i+9)%10] == 'M')
				{
					x = 155;
					y = 88;
					tt2 = rx_data[(i+2)%8];
					X1 = ( rx_data[(i+4)%8] - y ) *3 / 4 ;
					if(X1>100) X1 = -97;
					Y1 = 150 + ( x - rx_data[(i+3)%8] ) *3 / 4 ;
					X2 = ( rx_data[(i+6)%8] - y ) *3 / 4 ;
					if(X2>100) X2 = -97;
					Y2 = 150 + ( x - rx_data[(i+5)%8] ) *3 / 4 ;
					if(tt1 != tt2) {t=1; tt1 = tt2;}
					break;
				}
			}
		}
	}
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
