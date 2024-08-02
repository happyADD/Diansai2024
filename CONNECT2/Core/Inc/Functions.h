#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "game.h"

typedef struct
{
  uint8_t Lmax; // 0~100
  uint8_t Lmin;
  uint8_t Amax; // 0~255
  uint8_t Amin;
  uint8_t Bmax;
  uint8_t Bmin;
} param_t;

extern uint8_t LCD_turn;
extern uint8_t turn;

extern uint8_t LCD_RX_DMA_BUFFER[17];
extern uint8_t OpenMV_RX_DMA_BUFFER[17];
extern uint8_t RoboArm_RX_DMA_BUFFER[17];
/*0x00 无棋子，0x01 黑子，0x02 白子*/
extern uint8_t chess_board[9];
extern uint8_t chess_board_old[9];

extern const uint8_t Black_chess[5][2];
extern int Black_chess_num;
extern const uint8_t White_chess[5][2];
extern int White_chess_num;
extern uint8_t Center_point[2];

/**
 * 格式位： "param"表示调参
 *          "chess"表示棋盘
 * 数据位：调参时：6个uint8_t+操作位�?�LST,NST,DNE”，棋盘时：9个uint8_t
 * 帧尾：�?�EOM�????
 * �????17字节
 */
extern uint8_t LCD_rx_buffer[18];
extern uint8_t OpenMV_rx_buffer[30]; //(1状�??+2坐标) *9 +3帧尾“EOM�????= 30
extern uint8_t OpenMV_tx_buffer[12];
extern int OpenMV_rx_save[30];
extern uint8_t RoboArm_rx_buffer[12];
extern uint8_t RoboArm_tx_buffer[12];

extern int ArmFlag; // �????要下棋时，取反�??

extern uint8_t Send2LCD_buffer[14]; // 发�?�到LCD的数�???? 2字节对局状�??+9字节棋盘数据+3字节帧尾

extern int Send2RoboArm_flag;  // 发�?�到RoboArm的标志位
extern int Send2RoboArm_buffer[7]; // 发�?�到RoboArm的数�???? 3字节帧头+4字节数据：起点坐标：x，y终点坐标：x，y
extern int Receive_RoboArm_buffer; // 接收到RoboArm的数据，是否结束运动

extern int best_position[2];

extern int pos[4];

void Receive_OpenMV(void);
uint8_t is_equal(void);
uint8_t is_button_pressed(void);
void RoboArm_Lay_Chess(void);
void Send2RoboArm(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void OpenMV_JudgeParam_Send(param_t param, uint8_t flag);
int is_board_changed(void);
void Send2LCD_Message(uint8_t info);
void Send2LCD_Play(void);
void Chess_Clear(void);
void Receive_LCD_mode(void);
void return_board(void);



#endif // !_FUNCTIONS_H_
