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
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "game.h"
#include "Functions.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/**
 * mode 0 待�?�择模式
 * mode 1 指定位置下棋模式
 * mode 2 人机对弈模式,人先手（LCD�??
 * mode 3 人机对弈模式,电脑先手（LCD�??
 * mode 4 人机对弈模式,人先手（手动�??
 * mode 5 人机对弈模式,电脑先手（手动）
 */
int mode = -1;
int key = 0;
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
int ADC_Value = 0;
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  Send2LCD_Play();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    Receive_OpenMV();
    Center_point[0] = OpenMV_rx_buffer[13];
    Center_point[1] = OpenMV_rx_buffer[14];
    // 对齐坐标

    //模式选择
    Receive_LCD_mode();
    switch (mode)
    {
    case 0:
    {
      HAL_ADC_Start(&hadc1);
      ADC_Value = HAL_ADC_GetValue(&hadc1);
      break;
    }
    case 1:
    {
			Chess_Clear();
      
      while (1)
      {
        Receive_OpenMV();
        HAL_UART_Receive(&huart1, LCD_rx_buffer, 18, 1000);
        for (int i = 0; i < 18; ++i)
        {
          if (LCD_rx_buffer[(i + 15) % 18] == 'E' && LCD_rx_buffer[(i + 16) % 18] == 'O' && LCD_rx_buffer[(i + 17) % 18] == 'M')
          {
            turn = LCD_rx_buffer[(i + 5) % 18] - 0x30;

            if (LCD_rx_buffer[i % 18] == 'c' && LCD_rx_buffer[(i + 1) % 18] == 'h' && LCD_rx_buffer[(i + 2) % 18] == 'e' && LCD_rx_buffer[(i + 3) % 18] == 's' && LCD_rx_buffer[(i + 4) % 18] == 's')
            {
              chess_board[0] = LCD_rx_buffer[(i + 6) % 18] - 0x30;
              chess_board[1] = LCD_rx_buffer[(i + 7) % 18] - 0x30;
              chess_board[2] = LCD_rx_buffer[(i + 8) % 18] - 0x30;
              chess_board[3] = LCD_rx_buffer[(i + 9) % 18] - 0x30;
              chess_board[4] = LCD_rx_buffer[(i + 10) % 18] - 0x30;
              chess_board[5] = LCD_rx_buffer[(i + 11) % 18] - 0x30;
              chess_board[6] = LCD_rx_buffer[(i + 12) % 18] - 0x30;
              chess_board[7] = LCD_rx_buffer[(i + 13) % 18] - 0x30;
              chess_board[8] = LCD_rx_buffer[(i + 14) % 18] - 0x30;
            }
            break;
          }
        }
        if (is_board_changed())
        {
          
          int cnt = 0;
            for (int i = 0; i < 9; i++)
              if(chess_board_old[i]) cnt++; 
            for(int i = 0; i < 9; i++)
            {
              if(chess_board[i] != chess_board_old[i])
              { 
               
                chess_board_old[i] = chess_board[i];
                if(cnt == 0 || cnt == 1) 
                Send2RoboArm(Black_chess[Black_chess_num][0],Black_chess[Black_chess_num++][1],OpenMV_rx_save[3*i+1],OpenMV_rx_save[3*i+2]);
                else if (cnt == 2 || cnt == 3)
                Send2RoboArm(White_chess[White_chess_num][0],White_chess[White_chess_num++][1],OpenMV_rx_save[3*i+1],OpenMV_rx_save[3*i+2]);
                Send2LCD_Play();
              }
            }
						
          if(cnt == 4)  
            break;
        }
        
      }
    }
    case 2: // 人机对弈，装置黑�??
    {
      Chess_Clear();
      /*默认下中间位�??*/
      pos[0] = 50;
      pos[1] = 50;
      pos[2] = 50;
      pos[3] = 50; // 下一个黑棋到中间

      Send2RoboArm(pos[0], pos[1], pos[2], pos[3]);
      Send2LCD_Play();

      if (turn < 0xf0)
        turn++;
      if (turn == 10)
        turn = 0;
      game(chess_board, best_position);
      chess_board[best_position[0] * 3 + best_position[1]] = 0x01;
      for (int j = 0; j < 9; j++)
      {
        chess_board_old[j] = chess_board[j];
      }
      for (int i = 0; i < 100; ++i)
        Send2LCD_Play();



      while (1)
      {
        /*玩家下棋*/
        if (!is_board_changed())
        {
          HAL_UART_Receive(&huart1, LCD_rx_buffer, 18, 1000);
          for (int i = 0; i < 18; ++i)
          {
            if (LCD_rx_buffer[(i + 15) % 18] == 'E' && LCD_rx_buffer[(i + 16) % 18] == 'O' && LCD_rx_buffer[(i + 17) % 18] == 'M')
            {

              if (LCD_rx_buffer[i % 18] == 'c' && LCD_rx_buffer[(i + 1) % 18] == 'h' && LCD_rx_buffer[(i + 2) % 18] == 'e' && LCD_rx_buffer[(i + 3) % 18] == 's' && LCD_rx_buffer[(i + 4) % 18] == 's')
              {
                chess_board[0] = LCD_rx_buffer[(i + 6) % 18] - 0x30;
                chess_board[1] = LCD_rx_buffer[(i + 7) % 18] - 0x30;
                chess_board[2] = LCD_rx_buffer[(i + 8) % 18] - 0x30;
                chess_board[3] = LCD_rx_buffer[(i + 9) % 18] - 0x30;
                chess_board[4] = LCD_rx_buffer[(i + 10) % 18] - 0x30;
                chess_board[5] = LCD_rx_buffer[(i + 11) % 18] - 0x30;
                chess_board[6] = LCD_rx_buffer[(i + 12) % 18] - 0x30;
                chess_board[7] = LCD_rx_buffer[(i + 13) % 18] - 0x30;
                chess_board[8] = LCD_rx_buffer[(i + 14) % 18] - 0x30;
              }
              break;
            }
          }
        }
        if (is_board_changed())
        {
          turn++;
          if (turn == 10)
            turn = 0;
          for (int j = 0; j < 9; j++)
          {
            chess_board_old[j] = chess_board[j];
          }
        }

        if (turn % 2 == 0)
        {

          if (turn < 0xf0)
            turn++;
          if (turn == 10)
            turn = 0;
          game(chess_board, best_position);
          chess_board[best_position[0] * 3 + best_position[1]] = 0x01;
          for (int j = 0; j < 9; j++)
          {
            chess_board_old[j] = chess_board[j];
          }
          for (int i = 0; i < 100; ++i)
            Send2LCD_Play();
        }
        if (turn % 2 == 1)
          Send2LCD_Play();
        if (mode != 2)

          break;

        /*结束�??�??*/
        if (checkWin1(chess_board) == 1)
        {
          turn = 0xf1;
          for (int i = 0; i < 100; ++i)
            Send2LCD_Play();
          for (int i = 0; i < 50; ++i)
            Send2LCD_Message(0x01);

          break;
        }
        else if (checkWin1(chess_board) == 2)
        {
          turn = 0xf2;
          for (int i = 0; i < 100; ++i)
            Send2LCD_Play();
          for (int i = 0; i < 50; ++i)
            Send2LCD_Message(0x02);

          break;
        }
      }
      if(is_equal())//平局
      {
        turn = 0xf3;
        for(int i=0;i<50;++i) Send2LCD_Message(0x03);
        for(int i=0;i<100;++i) Send2LCD_Play();
        break;
      }
      Receive_LCD_mode();
      if (mode != 2)
        break;
    }
    case 3: // 人机对弈，装置白�??
    {
      Chess_Clear();
      for (int i = 0; i < 50; ++i)
        Send2LCD_Message(0xf0); // 交换黑白�??
      Send2LCD_Play();
      while (1)
      {
        /*玩家下棋*/
        if (!is_board_changed())
        {
          HAL_UART_Receive(&huart1, LCD_rx_buffer, 18, 1000);
          for (int i = 0; i < 18; ++i)
          {
            if (LCD_rx_buffer[(i + 15) % 18] == 'E' && LCD_rx_buffer[(i + 16) % 18] == 'O' && LCD_rx_buffer[(i + 17) % 18] == 'M')
            {

              if (LCD_rx_buffer[i % 18] == 'c' && LCD_rx_buffer[(i + 1) % 18] == 'h' && LCD_rx_buffer[(i + 2) % 18] == 'e' && LCD_rx_buffer[(i + 3) % 18] == 's' && LCD_rx_buffer[(i + 4) % 18] == 's')
              {
                chess_board[0] = LCD_rx_buffer[(i + 6) % 18] - 0x30;
                chess_board[1] = LCD_rx_buffer[(i + 7) % 18] - 0x30;
                chess_board[2] = LCD_rx_buffer[(i + 8) % 18] - 0x30;
                chess_board[3] = LCD_rx_buffer[(i + 9) % 18] - 0x30;
                chess_board[4] = LCD_rx_buffer[(i + 10) % 18] - 0x30;
                chess_board[5] = LCD_rx_buffer[(i + 11) % 18] - 0x30;
                chess_board[6] = LCD_rx_buffer[(i + 12) % 18] - 0x30;
                chess_board[7] = LCD_rx_buffer[(i + 13) % 18] - 0x30;
                chess_board[8] = LCD_rx_buffer[(i + 14) % 18] - 0x30;
              }
              break;
            }
          }
        }
        if (is_board_changed())
        {
          turn++;
          if (turn == 10)
            turn = 0;
          for (int j = 0; j < 9; j++)
          {
            chess_board_old[j] = chess_board[j];
          }
        }
        //
        if (turn % 2 == 1)
        {

          if (turn < 0xf0)
            turn++;
          if (turn == 10)
            turn = 0;
          game(chess_board, best_position);
          chess_board[best_position[0] * 3 + best_position[1]] = 0x02;
          for (int j = 0; j < 9; j++)
          {
            chess_board_old[j] = chess_board[j];
          }
          for (int i = 0; i < 100; ++i)
            Send2LCD_Play();
        }
        if (turn % 2 == 0)
          Send2LCD_Play();

        /*结束�??�??*/
        if (checkWin1(chess_board) == 1)
        {
          turn = 0xf1;
          for (int i = 0; i < 100; ++i)
            Send2LCD_Play();
          for (int i = 0; i < 50; ++i)
            Send2LCD_Message(0x01);

          break;
        }
        else if (checkWin1(chess_board) == 2)
        {
          turn = 0xf2;
          for (int i = 0; i < 100; ++i)
            Send2LCD_Play();
          for (int i = 0; i < 50; ++i)
            Send2LCD_Message(0x02);

          break;
        }
        if(is_equal())//平局
        {
          turn = 0xf3;
          for(int i=0;i<50;++i) Send2LCD_Message(0x03);
          for(int i=0;i<100;++i) Send2LCD_Play();
          break;
        }
        if (mode != 3)
          break;
      }
      Receive_LCD_mode();
      if (mode != 3)
        break;
    }
    case 4: // 人机对弈，装置黑�??(手动)
    {
      Chess_Clear();
      /*默认下中间位�??*/
      Receive_OpenMV();

      pos[0] = Black_chess[Black_chess_num][0];
      pos[1] = Black_chess[Black_chess_num++][1];
      pos[2] = OpenMV_rx_save[13];
      pos[3] = OpenMV_rx_save[14]; // 下一个黑棋到中间

      Send2RoboArm(pos[0], pos[1], pos[2], pos[3]);
      Send2LCD_Play();

      if (turn < 0xf0)
        turn++;
      if (turn == 10)
        turn = 0;
      game(chess_board, best_position);
      chess_board[best_position[0] * 3 + best_position[1]] = 0x01;
      for (int j = 0; j < 9; j++)
      {
        chess_board_old[j] = chess_board[j];
      }
      for (int i = 0; i < 100; ++i)
        Send2LCD_Play();

      while (1)
      {
        /*玩家下棋*/
				Receive_OpenMV();
				Send2LCD_Play();
         
        if (is_button_pressed())
        {
          Receive_OpenMV();
          if (is_board_changed())
          {
            turn++;

            if (turn == 10)
              turn = 0;

            for (int j = 0; j < 9; j++)
            {
              chess_board_old[j] = chess_board[j];
            }

            for (int i = 0; i < 5; ++i)
              Send2LCD_Play();
						Receive_OpenMV();
          }
          else if(is_board_changed() == 2)
          {
            return_board();
          }
					if (turn % 2 == 0)
          {
            if (turn < 0xf0)
              turn++;
            if (turn == 10)
              turn = 0;
            game(chess_board, best_position);
            chess_board[best_position[0] * 3 + best_position[1]] = 0x01;

            RoboArm_Lay_Chess(); //
    
            Receive_OpenMV();
            for (int j = 0; j < 9; j++)
            {
              chess_board_old[j] = chess_board[j];
            }

            for (int i = 0; i < 100; ++i)
              Send2LCD_Play();
          }
          if (turn % 2 == 1)
            Send2LCD_Play();
          if (mode != 4)

            break;

          /*结束�??�??*/
          if (checkWin1(chess_board) == 1)
          {
            turn = 0xf1;
            for (int i = 0; i < 10; ++i)
              Send2LCD_Play();
            for (int i = 0; i < 5; ++i)
              Send2LCD_Message(0x01);

            break;
          }
          else if (checkWin1(chess_board) == 2)
          {
            turn = 0xf2;
            for (int i = 0; i < 10; ++i)
              Send2LCD_Play();
            for (int i = 0; i < 5; ++i)
              Send2LCD_Message(0x02);

            break;
          }
          if (is_equal()) // 平局
          {
            turn = 0xf3;
            for (int i = 0; i < 5; ++i)
              Send2LCD_Message(0x03);
            for (int i = 0; i < 10; ++i)
              Send2LCD_Play();
            break;
          }
        }
        Receive_LCD_mode();
        if (mode != 4)
          break;
      }
      Receive_LCD_mode();
      if (mode != 4)
        break;
    }
    case 5: // 人机对弈，装置白�??(手动)
    {
      Chess_Clear();
      for (int i = 0; i < 5; ++i)
        Send2LCD_Message(0xf0); // 交换黑白�??
      Send2LCD_Play();
      while (1)
      {
        /*玩家下棋*/
        if (is_button_pressed())
        {

            HAL_UART_Receive(&huart2, OpenMV_rx_buffer, 30, 1000);
            for (int i = 0; i < 30; ++i)
            {
              if (OpenMV_rx_buffer[(i + 27) % 30] == 'E' && OpenMV_rx_buffer[(i + 28) % 30] == 'O' && OpenMV_rx_buffer[(i + 29) % 30] == 'M')
              {
                for (int j = 0; j < 9; j++)
                  chess_board[j] = OpenMV_rx_buffer[(i + j * 3) % 30];
                break;
              }
            }
          
          if (is_board_changed() == 1)
          {
            turn++;
            if (turn == 10)
              turn = 0;
            for (int j = 0; j < 9; j++)
            {
              chess_board_old[j] = chess_board[j];
            }

            for (int i = 0; i < 5; ++i)
              Send2LCD_Play();
            RoboArm_Lay_Chess();
          }
          else if(is_board_changed() == 2)
          {
            return_board();
          }
          //
          if (turn % 2 == 1)
          {

            if (turn < 0xf0)
              turn++;
            if (turn == 10)
              turn = 0;
            game(chess_board, best_position);
            chess_board[best_position[0] * 3 + best_position[1]] = 0x02;
            for (int j = 0; j < 9; j++)
            {
              chess_board_old[j] = chess_board[j];
            }
            RoboArm_Lay_Chess();
            
            for (int i = 0; i < 10; ++i)
              Send2LCD_Play();
          }
          if (turn % 2 == 0)
            Send2LCD_Play();

          /*结束�??�??*/
          if (checkWin1(chess_board) == 1)
          {
            turn = 0xf1;
            for (int i = 0; i < 10; ++i)
              Send2LCD_Play();
            for (int i = 0; i < 5; ++i)
              Send2LCD_Message(0x01);

            break;
          }
          else if (checkWin1(chess_board) == 2)
          {
            turn = 0xf2;
            for (int i = 0; i < 10; ++i)
              Send2LCD_Play();
            for (int i = 0; i < 5; ++i)
              Send2LCD_Message(0x02);

            break;
          }
          if (is_equal()) // 平局
          {
            turn = 0xf3;
            for (int i = 0; i < 5; ++i)
              Send2LCD_Message(0x03);
            for (int i = 0; i < 10; ++i)
              Send2LCD_Play();
            break;
          }
          Receive_LCD_mode();
          if (mode != 5)
            break;
        }
      }
      Receive_LCD_mode();
      if (mode != 5)
        break;
    }
    default:
    {
//            while (1)
//            {
//      				Send2RoboArm(pos[0],pos[1],pos[2],pos[3]);
//      				HAL_Delay(5000);
//            }
      break;
    }
    }
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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
