#include "Applications.h"

#define board_x 200
#define board_y 100

extern uint8_t turn;
extern uint8_t CenterTurn;
extern uint8_t state;
extern int chesscolor;
int Chess_position[9][2]=
{
    board_x+50,board_y+50,    board_x+150,board_y+50,   board_x+250,board_y+50,
    board_x+50,board_y+150,   board_x+150,board_y+150,  board_x+250,board_y+150,
    board_x+50,board_y+250,   board_x+150,board_y+250,  board_x+250,board_y+250
}; 
uint8_t Chess_info[10][9]={0x00};
uint8_t Screen_Chess[9]={0x00};//0x00表示没有棋子，0x01表示黑棋，0x02表示白棋
/**
 * @brief       清空屏幕并在右上角显示"RST"
 * @param       无
 * @retval      无
 */
void load_draw_dialog(void)
{
    lcd_clear(WHITE);                                                /* 清屏 */
    lcd_show_string(lcddev.width - 24, 0, 200, 16, 16, "RST", BLUE); /* 显示清屏区域 */
}

/**
 * @brief       画粗线
 * @param       x1,y1: 起点坐标
 * @param       x2,y2: 终点坐标
 * @param       size : 线条粗细程度
 * @param       color: 线的颜色
 * @retval      无
 */
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    if (x1 < size || x2 < size || y1 < size || y2 < size)
        return;

    delta_x = x2 - x1;                        /* 计算坐标增量 */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)
    {
        incx = 1;                             /* 设置单步方向 */
    }
    else if (delta_x == 0)
    {
        incx = 0;                             /* 垂直线 */
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;                               /* 水平线 */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
        distance = delta_x;                     /* 选取基本增量坐标轴 */
    else
        distance = delta_y;

    for (t = 0; t <= distance + 1; t++)         /* 画线输出 */
    {
        lcd_fill_circle(row, col, size, color); /* 画点 */
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}
/* 10个触控点的颜色(电容触摸屏用) */
const uint16_t POINT_COLOR_TBL[10] = {RED, GREEN, BLUE, BROWN, YELLOW, MAGENTA, CYAN, LIGHTBLUE, BRRED, GRAY};

/**
 * @brief       电容触摸屏测试函数
 * @param       无
 * @retval      无
 */
void ctp_test(void)
{
    uint8_t t = 0;
    uint8_t i = 0;
    uint16_t lastpos[10][2];                                                                                     /* 最后一次的数据 */
    uint8_t maxp = 5;

    if (lcddev.id == 0x1018)maxp = 10;

    while (1)
    {
        tp_dev.scan(0);

        for (t = 0; t < maxp; t++)
        {
            if ((tp_dev.sta) & (1 << t))
            {
                if (tp_dev.x[t] < lcddev.width && tp_dev.y[t] < lcddev.height)                                    /* 坐标在屏幕范围内 */
                {
                    if (lastpos[t][0] == 0xFFFF)
                    {
                        lastpos[t][0] = tp_dev.x[t];
                        lastpos[t][1] = tp_dev.y[t];
                    }

                    lcd_draw_bline(lastpos[t][0], lastpos[t][1], tp_dev.x[t], tp_dev.y[t], 2, POINT_COLOR_TBL[t]); /* 画线 */
                    lastpos[t][0] = tp_dev.x[t];
                    lastpos[t][1] = tp_dev.y[t];

                    if (tp_dev.x[t] > (lcddev.width - 24) && tp_dev.y[t] < 20)
                    {
                        load_draw_dialog();                                                                        /* 清除 */
                    }
                }
            }
            else 
            {
                lastpos[t][0] = 0xFFFF;
            }
        }

        delay_ms(5);
        i++;

        if (i % 20 == 0)LED0_TOGGLE();
    }
}


void draw_board(void)
{
    lcd_clear(BOARD_COLOR);
    lcd_draw_line(board_x+100,board_y,board_x+100,board_y+300,BLACK);
    lcd_draw_line(board_x+200,board_y,board_x+200,board_y+300,BLACK);
    lcd_draw_line(board_x,board_y+100,board_x+300,board_y+100,BLACK);
    lcd_draw_line(board_x,board_y+200,board_x+300,board_y+200,BLACK);
    lcd_draw_rectangle(board_x,board_y,board_x+300,board_y+300,BLACK);
}
/**
 * @brief 绘制棋子
 * 
 * @param num 
 * 
 * @param color 1黑色 0白色
 */
void draw_chess(int num,int color)
{
    if(!color)
    lcd_fill_circle(Chess_position[num-1][0],Chess_position[num-1][1],45,WHITE);
    else if(color)
    lcd_fill_circle(Chess_position[num-1][0],Chess_position[num-1][1],45,BLACK);

}
void Clear_chess(int num)
{
    lcd_fill_circle(Chess_position[num-1][0],Chess_position[num-1][1],45,BOARD_COLOR);
}
void USART_Receive(void)
{
    for(int k=0;k<10;++k)
    {
        for(int i=0;i<13;++i)
        {
            if(g_usart_rx_buf[(i+10)%13] == 'E' && g_usart_rx_buf[(i+11)%13] == 'O' && g_usart_rx_buf[(i+12)%13] =='M')
            {
              turn = g_usart_rx_buf[i%13];
              Chess_info[k][0] = g_usart_rx_buf[(i+1)%13];
              Chess_info[k][1] = g_usart_rx_buf[(i+2)%13];
              Chess_info[k][2] = g_usart_rx_buf[(i+3)%13];
              Chess_info[k][3] = g_usart_rx_buf[(i+4)%13];
              Chess_info[k][4] = g_usart_rx_buf[(i+5)%13];
              Chess_info[k][5] = g_usart_rx_buf[(i+6)%13];
              Chess_info[k][6] = g_usart_rx_buf[(i+7)%13];
              Chess_info[k][7] = g_usart_rx_buf[(i+8)%13];
              Chess_info[k][8] = g_usart_rx_buf[(i+9)%13];
              if(turn == 0xf1)
              {
                for(int j=0;j<9;++j) Screen_Chess[i]=Chess_info[0][i];
                state = 0x01;
                return;
              }
              else if(turn == 0xf2)
              {
                for(int j=0;j<9;++j) Screen_Chess[i]=Chess_info[1][i];
                state = 0x02;
                return;
              }
              else if(turn == 0xf3)
              {
                for(int j=0;j<9;++j) Screen_Chess[i]=Chess_info[2][i];
                state = 0x01;
                return;
              }
              else if(turn == 0xf0)
              {
                for(int j=0;j<9;++j) Screen_Chess[i]=Chess_info[2][i];
                chesscolor = 1;
                return;
              }
              break;
            }
        }
      
    }
}

void update_board(void)
{
    for(int i=0;i<9;++i)
    {
			if(	    Chess_info[0][i] == Chess_info[1][i] && 
					Chess_info[1][i] == Chess_info[2][i] &&
					Chess_info[2][i] == Chess_info[3][i] &&
					Chess_info[3][i] == Chess_info[4][i] &&
					Chess_info[4][i] == Chess_info[5][i] &&
					Chess_info[5][i] == Chess_info[6][i] &&
					Chess_info[6][i] == Chess_info[7][i] &&
					Chess_info[7][i] == Chess_info[8][i] &&
					Chess_info[8][i] == Chess_info[9][i] )
			{
                Screen_Chess[i]=Chess_info[0][i];
                if(Screen_Chess[i]==0x00)
                    Clear_chess(i+1);
                else if(Screen_Chess[i]==0x01)
                    draw_chess(i+1,1);
                else if(Screen_Chess[i]==0x02)
                    draw_chess(i+1,0);
			}
    }
}

void dragger_gen(drager_t *drager, int x, int y,int color,int size)
{
    drager->x = x;
    drager->y = y;
    drager->color = color;
    drager->size = size;
}

void dragger_draw(drager_t *drager)
{
    lcd_draw_bline(drager->x,drager->y,drager->x+drager->size,drager->y,drager->size,drager->color);
}

void block_gen(block_t *block, int x, int y,uint16_t color,int size,char *str)
{
    block->position.x = x;
    block->position.y = y;
    block->color = color;
    block->size = size;
    block->str = str;

    lcd_fill_circle(block->position.x,block->position.y,block->size,block->color);
    lcd_show_string(block->position.x-10,block->position.y-10,block->size,50,32,block->str,BLACK);
}

int choose_detect(block_t *block,int delta_v)
{
    uint8_t t = 0;                                                                           
    uint8_t maxp = 5;

    if (lcddev.id == 0x1018)maxp = 10;

    
        tp_dev.scan(0);

        for (t = 0; t < maxp; t++)
        {
            if ((tp_dev.sta) & (1 << t))
            {
                if (tp_dev.x[t] < lcddev.width && tp_dev.y[t] < lcddev.height)                                    /* 坐标在屏幕范围内 */
                {
                    if( 
                        (tp_dev.y[t]>=block->position.y-block->size)&&
                        (tp_dev.y[t]<=block->position.y+block->size)&&
                        (tp_dev.x[t]-block->position.x)*(tp_dev.x[t]-block->position.x)<=(block->size*block->size)&&
                        (tp_dev.y[t]-block->position.y)*(tp_dev.y[t]-block->position.y)<=(block->size*block->size)
                        )
                    return delta_v;
                }
            }
            else return 0;
        }

        delay_ms(10);
    

    return 0;
    
}

int touch_detect(int delta_v,int x1,int y1,int x2,int y2)
{
    uint8_t t = 0;                                                                           
    uint8_t maxp = 5;

    if (lcddev.id == 0x1018)maxp = 10;

    
        tp_dev.scan(0);

        for (t = 0; t < maxp; t++)
        {
            if ((tp_dev.sta) & (1 << t))
            {
                if (tp_dev.x[t] < lcddev.width && tp_dev.y[t] < lcddev.height)                                    /* 坐标在屏幕范围内 */
                {
                    if( (tp_dev.x[t]>=x1)&&
                        (tp_dev.x[t]<=x2)&&
                        (tp_dev.y[t]>=y1)&&
                        (tp_dev.y[t]<=y2)
                        )
                    return delta_v;
                }
            }
            else return 0;
        }

        delay_ms(10);
    

    return 0;
    
}
int block_detect(block_t *block,int delta_v)
{
    uint8_t t = 0;                                                                           
    uint8_t maxp = 5;

    if (lcddev.id == 0x1018)maxp = 10;

    
        tp_dev.scan(0);

        for (t = 0; t < maxp; t++)
        {
            if ((tp_dev.sta) & (1 << t))
            {
                if (tp_dev.x[t] < lcddev.width && tp_dev.y[t] < lcddev.height)                                    /* 坐标在屏幕范围内 */
                {
                    if( (tp_dev.x[t]>=block->position.x-block->size)&&
                        (tp_dev.x[t]<=block->position.x+block->size)&&
                        (tp_dev.y[t]>=block->position.y-block->size)&&
                        (tp_dev.y[t]<=block->position.y+block->size)&&
                        (tp_dev.x[t]-block->position.x)*(tp_dev.x[t]-block->position.x)<=(block->size*block->size)&&
                        (tp_dev.y[t]-block->position.y)*(tp_dev.y[t]-block->position.y)<=(block->size*block->size)
                        )
                    return delta_v;
                }
            }
            else return 0;
        }

        delay_ms(10);
    

    return 0;
    
}
/*探测下棋位置*/
int Chess_detect(void)
{
    uint8_t t = 0;
    uint8_t i = 0;
    uint16_t lastpos[10][2];                                                                                     /* 最后一次的数据 */
    uint8_t maxp = 5;
    int x,y;
    if (lcddev.id == 0x1018)maxp = 10;

  
        tp_dev.scan(0);

        for (t = 0; t < maxp; t++)
        {
            if ((tp_dev.sta) & (1 << t))
            {
                if (tp_dev.x[t] < lcddev.width && tp_dev.y[t] < lcddev.height)                                    /* 坐标在屏幕范围内 */
                {

                    while(tp_dev.x[0] != 0x00 && tp_dev.x[0] != 0xFFFF && tp_dev.y[0] != 0x00 && tp_dev.y[0] != 0xFFFF)
                    {   
                        
                        x = tp_dev.x[0];
                        y = tp_dev.y[0];
                        tp_dev.scan(0);
                    }
                
                   
                   if(      x>=Chess_position[0][0]-50&&x<=Chess_position[0][0]+50&&
                            y>=Chess_position[0][1]-50&&y<=Chess_position[0][1]+50
                    )
                            return 1;
                   else if( x>=Chess_position[1][0]-50&&x<=Chess_position[1][0]+50&&
                            y>=Chess_position[1][1]-50&&y<=Chess_position[1][1]+50
                    )
                            return 2;
                   else if( x>=Chess_position[2][0]-50&&x<=Chess_position[2][0]+50&&
                            y>=Chess_position[2][1]-50&&y<=Chess_position[2][1]+50
                    )
                            return 3;
                   else if( x>=Chess_position[3][0]-50&&x<=Chess_position[3][0]+50&&
                            y>=Chess_position[3][1]-50&&y<=Chess_position[3][1]+50
                    )
                            return 4;
                   else if( x>=Chess_position[4][0]-50&&x<=Chess_position[4][0]+50&&
                            y>=Chess_position[4][1]-50&&y<=Chess_position[4][1]+50
                    )
                            return 5;
                   else if( x>=Chess_position[5][0]-50&&x<=Chess_position[5][0]+50&&
                            y>=Chess_position[5][1]-50&&y<=Chess_position[5][1]+50
                    )
                            return 6;
                   else if( x>=Chess_position[6][0]-50&&x<=Chess_position[6][0]+50&&
                            y>=Chess_position[6][1]-50&&y<=Chess_position[6][1]+50
                    )
                            return 7;
                   else if( x>=Chess_position[7][0]-50&&x<=Chess_position[7][0]+50&&
                            y>=Chess_position[7][1]-50&&y<=Chess_position[7][1]+50
                    )
                            return 8;
                   else if( x>=Chess_position[8][0]-50&&x<=Chess_position[8][0]+50&&
                            y>=Chess_position[8][1]-50&&y<=Chess_position[8][1]+50
                    )
                            return 9;
                        

                    
  

                }
            }
            else 
            {
                lastpos[t][0] = 0xFFFF;
            }
        }

        delay_ms(5);

        return 0;
}

void lay_chess(int usr)
{
    if(Chess_info[0][Chess_detect()-1] == 0x00)
    {
        if(usr==0)
        {
            draw_chess(Chess_detect(),0);
        }
        if(usr==1)//白子
        {
            draw_chess(Chess_detect(),1);
        }
    }
}
/**
 * @brief 
 * 
 * @param press 
 * @param color 0白 1黑
 */
void Send_Chess_info(int press,int color)
{
    for(int i=0;i<500;i++)
        if(press != 0)
        {
            if(color != 3)
            {
            if(!color)
                Screen_Chess[press-1] = 0x02;
            else
                Screen_Chess[press-1] = 0x01;
            
            }
            printf("chess");
            printf("%c",turn+49);
            for(int i=0;i<9;++i)
            {
                printf("%d",Screen_Chess[i]);
            }
            printf("EOM");
            
        }
//    if(turn <0xf0)
//		//turn++;
//    if(turn==10) 
//		turn=0;
    // delay_ms(1000);
}

// void show_menu(void)
// {
//     lcd_clear(WHITE);
//     block_t menu[2];
//     block_gen(menu,)
//     block_detect()
// }
