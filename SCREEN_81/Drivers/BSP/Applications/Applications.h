#ifndef APPLICATIONS_H_
#define APPLICATIONS_H_

#include "stdint.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/SDRAM/sdram.h"
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TOUCH/touch.h"

extern const uint16_t POINT_COLOR_TBL[10];

#define BOARD_COLOR CYAN

typedef struct
{
    int x;
    int y;
}position_t;

typedef struct
{
    int x;
    int y;
    int color;
    int size;//0~2
}drager_t;

typedef struct 
{
    position_t position;
    int size;
    int color; 
    char *str;
}block_t;


void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color);
void load_draw_dialog(void);
void draw_board(void);
void draw_chess(int num,int color);
void Clear_chess(int num);
void USART_Receive(void);
void update_board(void);

int touch_detect(int delta_v,int x1,int y1,int x2,int y2);
int choose_detect(block_t *block,int delta_v);
void block_gen(block_t *block, int x, int y,uint16_t color,int size,char *str);
int block_detect(block_t *block,int delta_v);
int Chess_detect(void);
void lay_chess(int usr);
void Send_Chess_info(int press,int color);

#endif /* APPLICATIONS_H_ */
