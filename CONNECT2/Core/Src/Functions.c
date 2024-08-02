#include "Functions.h"

const uint8_t Black_chess[5][2]={
  {245,255},
  {200,255},
  {155,255},
  {115,255},
  {70,255}
};
int Black_chess_num = 0;
const uint8_t White_chess[5][2]=
{
  {245,200},
  {200,200},
  {155,200},
  {115,200},
  {70,200}
};
int White_chess_num = 0;
uint8_t Center_point[2] = {0,0};

uint8_t LCD_turn = 0;
uint8_t turn = 0;

uint8_t LCD_RX_DMA_BUFFER[17];
uint8_t OpenMV_RX_DMA_BUFFER[17];
uint8_t RoboArm_RX_DMA_BUFFER[17];
/*0x00 无棋子，0x01 黑子，0x02 白子*/
uint8_t chess_board[9] = {0};
uint8_t chess_board_old[9] = {0};

/**
 * 格式位： "param"表示调参
 *          "chess"表示棋盘
 * 数据位：调参时：6个uint8_t+操作位�?�LST,NST,DNE”，棋盘时：9个uint8_t
 * 帧尾：�?�EOM�????
 * �????17字节
 */
uint8_t LCD_rx_buffer[18];
uint8_t OpenMV_rx_buffer[30]; //(1状�??+2坐标) *9 +3帧尾“EOM�????= 30
int OpenMV_rx_save[30];
uint8_t OpenMV_tx_buffer[12];
uint8_t RoboArm_rx_buffer[12];
uint8_t RoboArm_tx_buffer[12];

int ArmFlag = 0; // �????要下棋时，取反�??

uint8_t Send2LCD_buffer[14]; // 发�?�到LCD的数�???? 2字节对局状�??+9字节棋盘数据+3字节帧尾

int Send2RoboArm_flag = 0;  // 发�?�到RoboArm的标志位
int Send2RoboArm_buffer[7]; // 发�?�到RoboArm的数�???? 3字节帧头+4字节数据：起点坐标：x，y终点坐标：x，y
int Receive_RoboArm_buffer; // 接收到RoboArm的数据，是否结束运动

int best_position[2] = {0};

int pos[4] = {0};

void swaps_uint8_t(uint8_t *a, uint8_t *b)
{
  uint8_t temp = *a;
  *a = *b;
  *b = temp;
}
void swaps_int(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void Receive_OpenMV(void)
{
  for (int k = 0; k < 2; k++)
  {
    HAL_UART_Receive(&huart2, OpenMV_rx_buffer, 30, 1000);
    for (int i = 0; i < 30; ++i)
    {
      if (OpenMV_rx_buffer[(i + 27) % 30] == 'E' && OpenMV_rx_buffer[(i + 28) % 30] == 'O' && OpenMV_rx_buffer[(i + 29) % 30] == 'M')
      {
        for (int j = 0; j < 9; j++)
        {
          chess_board[j] = OpenMV_rx_buffer[(i + j * 3) % 30];
          OpenMV_rx_save[j*3] = OpenMV_rx_buffer[(i + j * 3) % 30];
          OpenMV_rx_save[j*3+1] = OpenMV_rx_buffer[(i + j * 3) % 30 + 1];
          OpenMV_rx_save[j*3+2] = OpenMV_rx_buffer[(i + j * 3) % 30 + 2];
        }
        break;
      }
    }
  }
  // for(int i=0;i<3;i++)
  // {
  //   swaps_uint8_t(&chess_board[i],&chess_board[i+6]);
  //   swaps_int(&OpenMV_rx_save[i*3],&OpenMV_rx_save[(i+6)*3]);
  //   swaps_int(&OpenMV_rx_save[i*3+1],&OpenMV_rx_save[(i+6)*3+1]);
  //   swaps_int(&OpenMV_rx_save[i*3+2],&OpenMV_rx_save[(i+6)*3+2]);
  // }
}

uint8_t is_equal(void)
{
  int re;
  int flag = checkWin1(chess_board);
  for (int i = 0; i < 9; i++)
  {
    if (chess_board[i] != 0x00 && !flag)
      re++;
  }
  if (re == 9)
    return 1;
  return 0;
}
/*按钮是否被摁下*/
uint8_t is_button_pressed(void)
{
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
  {
    HAL_Delay(50);
    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
      ;
    return 1;
  }
  return 0;
}
/**
 * @brief 物理上跟新棋盘
 *
 * 调用地方：在探测到物理按钮被摁下时调用，此函数之前需要更新openmv数据且保持一致
 * 理论上，在更新棋盘的时候调用
 * 第0步：检查OpenMv信息，是否与本地保护成员数据一致
 * 一致则出函数
 * 如果不一致，再次更新
 * 第一步：接收OpenMV传来的棋盘信息
 * 第二部：将坐标信息输入到RoboArm的控制函数中
 ******####!!!应当使用chess_board_old
 */
void RoboArm_Lay_Chess(void)
{
  for (int i = 0; i < 2; i++)
    Receive_OpenMV();
  if(chess_board[best_position[0] * 3 + best_position[1]] == 0x00)
  {
    if(mode == 2||4) 
    {
      Send2RoboArm(Black_chess[Black_chess_num][0], Black_chess[Black_chess_num++][1], OpenMV_rx_save[(best_position[0] * 3 + best_position[1]) * 3 + 1], OpenMV_rx_save[(best_position[0] * 3 + best_position[1]) * 3 + 2]);
      chess_board_old[best_position[0] * 3 + best_position[1]] = 0x01;
    }
    else if(mode == 3||5) 
    {
      Send2RoboArm(White_chess[White_chess_num][0], White_chess[White_chess_num++][1], OpenMV_rx_save[(best_position[0] * 3 + best_position[1]) * 3 + 1], OpenMV_rx_save[(best_position[0] * 3 + best_position[1]) * 3 + 2]);
      chess_board_old[best_position[0] * 3 + best_position[1]] = 0x02;
      }
  }
 
  
}

void return_board(void)
{
  for (int i = 0; i < 9; i++)
  {
    if (chess_board[i] != chess_board_old[i])
    {
      if (chess_board_old[i] == 0x01 && chess_board[i] == 0x00)
      {
        for (int j = 0; j < 9; j++)
        {
          if (chess_board_old[j] == 0x00 && chess_board[j] == 0x01)
            Send2RoboArm(OpenMV_rx_save[j * 3 + 1], OpenMV_rx_save[j * 3 + 2], OpenMV_rx_save[i * 3 + 1], OpenMV_rx_save[i * 3 + 2]);
        }
      }
      else if (chess_board_old[i] == 0x02 && chess_board[i] == 0x00)
      {
        for (int j = 0; j < 9; j++)
        {
          if (chess_board_old[j] == 0x00 && chess_board[j] == 0x02)
            Send2RoboArm(OpenMV_rx_save[j * 3 + 1], OpenMV_rx_save[j * 3 + 2], OpenMV_rx_save[i * 3 + 1], OpenMV_rx_save[i * 3 + 2]);
        }
      }
    }
  }
}

void Send2RoboArm(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  uint8_t send_buffer[10] = {0};
  send_buffer[0] = Center_point[0];
  send_buffer[1] = Center_point[1];
  send_buffer[2] = ArmFlag;
  send_buffer[3] = x1;
  send_buffer[4] = y1;
  send_buffer[5] = x2;
  send_buffer[6] = y2;
  send_buffer[7] = 'E';
  send_buffer[8] = 'O';
  send_buffer[9] = 'M';
  HAL_UART_Transmit(&huart3, send_buffer, 10, 100);
  ArmFlag = !ArmFlag;
  // HAL_UART_Receive_DMA(&huart3,RoboArm_RX_DMA_BUFFER,20);
}
void OpenMV_JudgeParam_Send(param_t param, uint8_t flag)
{
  uint8_t send_buffer[10];
  send_buffer[0] = flag; // 0表示结束�????1表示继续调参
  send_buffer[1] = param.Lmax;
  send_buffer[2] = param.Lmin;
  send_buffer[3] = param.Amax;
  send_buffer[4] = param.Amin;
  send_buffer[5] = param.Bmax;
  send_buffer[6] = param.Bmin;
  send_buffer[7] = 'E';
  send_buffer[8] = 'O';
  send_buffer[9] = 'M';

  HAL_UART_Transmit(&huart2, send_buffer, 10, 1000);
}

int is_board_changed(void)
{
  uint8_t is_Changed = 0;
  int cnt1 = 0, cnt2 = 0;
  for (int i = 0; i < 9; i++)
  {
    if (chess_board[i] != chess_board_old[i])
    {
      is_Changed = 1;
    }
    if (chess_board[i] >= 0x03)
      return 0;
    if (chess_board[i] != 0x00)
      cnt1++;
    if (chess_board_old[i] != 0x00)
      cnt2++;
  }
  if (cnt1 < cnt2)
    is_Changed = 0;
    else if(cnt1 == cnt2)
    is_Changed = 2;
  return is_Changed;
}
void Send2LCD_Message(uint8_t info)
{
  uint8_t send_buffer[13];
  if (turn > 0xf0)
    info = turn;
  send_buffer[0] = info;
  for (int i = 1; i < 10; i++)
  {
    send_buffer[i] = chess_board[i - 1];
  }
  send_buffer[10] = 'E';
  send_buffer[11] = 'O';
  send_buffer[12] = 'M';
  HAL_UART_Transmit(&huart1, send_buffer, 13, 1000);
}
void Send2LCD_Play(void)
{
  uint8_t send_buffer[13];
  send_buffer[0] = turn;
  for (int i = 1; i < 10; i++)
  {
    send_buffer[i] = chess_board_old[i - 1];
  }
  send_buffer[10] = 'E';
  send_buffer[11] = 'O';
  send_buffer[12] = 'M';
  HAL_UART_Transmit(&huart1, send_buffer, 13, 1000);
}
void Chess_Clear(void)
{
  for (int i = 0; i < 9; i++)
  {
    chess_board[i] = 0x00;
    chess_board_old[i] = 0x00;
  }
}
void Receive_LCD_mode(void)
{
   HAL_UART_Receive(&huart1, LCD_rx_buffer, 18, 1000);
    for (int i = 0; i < 18; ++i)
    {
      if (LCD_rx_buffer[(i + 15) % 18] == 'E' && LCD_rx_buffer[(i + 16) % 18] == 'O' && LCD_rx_buffer[(i + 17) % 18] == 'M')
      {

        if (LCD_rx_buffer[i % 18] == 'c' && LCD_rx_buffer[(i + 1) % 18] == 'h' && LCD_rx_buffer[(i + 2) % 18] == 'e' && LCD_rx_buffer[(i + 3) % 18] == 's' && LCD_rx_buffer[(i + 4) % 18] == 's')
        {
          int flag = LCD_rx_buffer[(i + 5) % 18];
          switch (flag)
          {
          case 0xe1:
            mode = 1;
					Chess_Clear();
            break;
          case 0xe2:
            mode = 2;
					Chess_Clear();
            break;
          case 0xe3:
            mode = 3;
					Chess_Clear();
            break;
          case 0xe4:
            mode = 4;
					Chess_Clear();
            break;
          case 0xe5:
            mode = 5;
					Chess_Clear();
            break;
          default:
						Chess_Clear();
            break;
          }
        }
        break;
      }
    }

}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1) // LCD
  {
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
      turn++;
      if (turn == 10)
        turn = 0;
    }
  }
  else if (huart->Instance == USART2) // OpenMV
  {
    for (int i = 0; i < 30; ++i)
    {
      if (OpenMV_rx_buffer[(i + 27) % 30] == 'E' && OpenMV_rx_buffer[(i + 28) % 30] == 'O' && OpenMV_rx_buffer[(i + 29) % 30] == 'M')
      {
        for (int j = 0; j < 9; j++)
          chess_board[j] = OpenMV_rx_buffer[(i + j * 3) % 30];
        break;
      }
    }
  }
  else if (huart->Instance == USART3) // RoboArm
  {
  }
}
