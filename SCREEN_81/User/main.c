
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/SDRAM/sdram.h"
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TOUCH/touch.h"
#include "./bitmap.h"
#include "./BSP/Applications/Applications.h"
#include "string.h"
int mode = 0;
int chesscolor = 0; // 0:白棋 1:黑棋
int val = 0;
uint8_t turn = 0;
uint8_t CenterTurn = 0;
uint8_t state = 0;//0xf0 交换黑白棋 0xf1黑胜 0xf2白胜 0xf3平局
extern uint8_t Screen_Chess[9];
extern uint8_t Chess_info[10][9];
int menuflag = 1;
int main(void)
{
  sys_stm32_clock_init(360, 25, 2, 8); /* ����ʱ��,180Mhz */
  delay_init(180);                     /* ��ʱ��ʼ�� */
  usart_init(90, 115200);              /* ��ʼ��USART */
  led_init();                          /* ��ʼ��LED */
  sdram_init();                        /* ��ʼ��SDRAM */
  lcd_init();                          /* ��ʼ��LCD */
  key_init();                          /* ��ʼ������ */
  tp_dev.init();                       /* ��������ʼ�� */
  lcd_display_dir(1);                  /* ��ʾ���� */
  if (lcddev.dir)
    tp_dev.touchtype = 1;
  else
    tp_dev.touchtype = 0;
  block_t mode1, mode2, mode3, mode4, mode5;

  display_bitmap_on_lcd(san_zi_qi, 352, 96, BLACK, 200, 50);
  block_gen(&mode1, 200, 200, WHITE, 32, "1");
  display_bitmap_on_lcd(zi_dong_xia_qi, 128, 32, BLACK, 200 + 32, 200 - 16);
  block_gen(&mode2, 200, 250, WHITE, 32, "2");
  display_bitmap_on_lcd(ji_qi_zhi_hei, 128, 32, BLACK, 200 + 32, 250 - 16);
  display_bitmap_on_lcd(zi_dong, 128, 32, BLACK, 232 + 128, 250 - 16);
  block_gen(&mode3, 200, 300, WHITE, 32, "3");
  display_bitmap_on_lcd(ji_qi_zhi_bai, 128, 32, BLACK, 200 + 32, 300 - 16);
  display_bitmap_on_lcd(zi_dong, 128, 32, BLACK, 232 + 128, 300 - 16);
  block_gen(&mode4, 200, 350, WHITE, 32, "4");
  display_bitmap_on_lcd(ji_qi_zhi_hei, 128, 32, BLACK, 200 + 32, 350 - 16);
  display_bitmap_on_lcd(shou_dong, 128, 32, BLACK, 232 + 128, 350 - 16);
  block_gen(&mode5, 200, 400, WHITE, 32, "5");
  display_bitmap_on_lcd(ji_qi_zhi_hei, 128, 32, BLACK, 200 + 32, 400 - 16);
  display_bitmap_on_lcd(shou_dong, 128, 32, BLACK, 232 + 128, 400 - 16);

  while (!mode)
  {
    if (menuflag == 1)
    {
      display_bitmap_on_lcd(san_zi_qi, 352, 96, BLACK, 200, 50);
      block_gen(&mode1, 200, 200, WHITE, 32, "1");
      display_bitmap_on_lcd(zi_dong_xia_qi, 128, 32, BLACK, 200 + 32, 200 - 16);
      block_gen(&mode2, 200, 250, WHITE, 32, "2");
      display_bitmap_on_lcd(ji_qi_zhi_hei, 128, 32, BLACK, 200 + 32, 250 - 16);
      display_bitmap_on_lcd(zi_dong, 128, 32, BLACK, 232 + 128, 250 - 16);
      block_gen(&mode3, 200, 300, WHITE, 32, "3");
      display_bitmap_on_lcd(ji_qi_zhi_bai, 128, 32, BLACK, 200 + 32, 300 - 16);
      display_bitmap_on_lcd(zi_dong, 128, 32, BLACK, 232 + 128, 300 - 16);
      block_gen(&mode4, 200, 350, WHITE, 32, "4");
      display_bitmap_on_lcd(ji_qi_zhi_hei, 128, 32, BLACK, 200 + 32, 350 - 16);
      display_bitmap_on_lcd(shou_dong, 128, 32, BLACK, 232 + 128, 350 - 16);
      block_gen(&mode5, 200, 400, WHITE, 32, "5");
      display_bitmap_on_lcd(ji_qi_zhi_hei, 128, 32, BLACK, 200 + 32, 400 - 16);
      display_bitmap_on_lcd(shou_dong, 128, 32, BLACK, 232 + 128, 400 - 16);
      menuflag = 0;
    }

    if (choose_detect(&mode1, 1))
    {
      mode = 1;
      Send_Chess_info(mode,3);
      break;
    }
    else if (choose_detect(&mode2, 2))
    {
      mode = 2;
      Send_Chess_info(mode,3);
      break;
    }
    else if (choose_detect(&mode3, 3))
    {
      mode = 3;
      Send_Chess_info(mode,3);
      break;
    }
    else if (choose_detect(&mode4, 4))
    {
      mode = 4;
      Send_Chess_info(mode,3);
      break;
    }
    else if (choose_detect(&mode5, 5))
    {
      mode = 5;
      Send_Chess_info(mode,3);
      break;
    }
  }

  draw_board();

  lcd_show_string(0, 0, 800, 50, 32, "Mode:", BLACK);
  lcd_show_num(80, 0, mode, 1, 32, BLACK);
  lcd_show_string(0, 480 - 40, 800, 50, 32, "Back", BLACK);
  while (1)
  {
    switch (mode)
    {
    case 1:
    {
      turn = 0xe1 - 49;
      val = 1;
      for (int i = 0; i < 5; i++)
        Send_Chess_info(val,3);
      val = 0;
      lcd_show_string(200,0,800,50,32,"InitDone!",BLACK);
      while (1)
      {
        int cnt = 0; // 0,1下黑棋，2,3下白棋
        if (touch_detect(1, 0, 440, 700, 800))
        {
          menuflag = 1;
          mode = 0;
          break;
        } // 返回
        if (cnt < 4)
          val = Chess_detect();
        if (val != 0)
        {
          if (cnt == 0 || cnt == 1)
          {
            draw_chess(val, 1);
            Send_Chess_info(val,1);
            
          }
          else if (cnt == 2 || cnt == 3)
           {
            draw_chess(val, 0);
            Send_Chess_info(val,0);
          }
          cnt++;
          val = 0;
          delay_ms(500);
        }

        update_board();

        USART_Receive();

        if (state == 1)
        {
          lcd_clear(BOARD_COLOR);
          draw_board();
          while (1)
          {
            USART_Receive();
            update_board();
            for (int i = 0; i < 9; i++)
            {
              if (Screen_Chess[i] == 0x00)
                Clear_chess(i + 1);
              else if (Screen_Chess[i] == 0x01)
                draw_chess(i + 1, 1);
              else if (Screen_Chess[i] == 0x02)
                draw_chess(i + 1, 0);
            }
            lcd_show_string(200, 0, 800, 50, 32, "Game Over:White Win!", BLACK);
          }
        }
        else if (state == 2)
        {
          USART_Receive();
          update_board();
          lcd_clear(BOARD_COLOR);

          draw_board();
          while (1)
          {

            for (int i = 0; i < 9; i++)
            {
              if (Screen_Chess[i] == 0x00)
                Clear_chess(i + 1);
              else if (Screen_Chess[i] == 0x01)
                draw_chess(i + 1, 1);
              else if (Screen_Chess[i] == 0x02)
                draw_chess(i + 1, 0);
            }
            lcd_show_string(200, 0, 800, 50, 32, "Game Over:Black Win!", BLACK);
          }
        }
      }
      break;
    }
    case 2:
    case 3:
    case 4:
	  case 5:
    {
      turn = 0xe0+mode -49;
      val = 1;
      for (int i = 0; i < 5; i++)
        Send_Chess_info(val,3);
      val = 0;

			lcd_show_string(0,32,800,32,32,"Touch:",BLACK);
            lcd_show_string(200,0,800,50,32,"InitDone!",BLACK);
      while(1)
      {
        if (touch_detect(1, 0, 440, 700, 800))
        {
          menuflag = 1;
          mode = 0;
          break;
        } // 返回
        if (turn % 2 == !chesscolor && val == 0)
          val = Chess_detect();
        if (val != 0)
        {
          draw_chess(val, chesscolor);
          Send_Chess_info(val,chesscolor);
          val = 0;
          delay_ms(1000);
        }

        update_board();
        lcd_show_num(80, 0, mode, 1, 32, BLACK);
				lcd_show_num(90, 32,val,1,32,BLACK);
        USART_Receive();

        if (state == 1)
        {
          lcd_clear(BOARD_COLOR);
          draw_board();
            USART_Receive();
            update_board();
            for (int i = 0; i < 9; i++)
            {
              if (Screen_Chess[i] == 0x00)
                Clear_chess(i + 1);
              else if (Screen_Chess[i] == 0x01)
                draw_chess(i + 1, 1);
              else if (Screen_Chess[i] == 0x02)
                draw_chess(i + 1, 0);
            }
            lcd_show_string(200, 0, 800, 50, 32, "Game Over:White Win!", BLACK);
            delay_ms(2000);

          break;
          
        }
        else if (state == 2)
        {
          USART_Receive();
          update_board();
          lcd_clear(BOARD_COLOR);

          draw_board();

            for (int i = 0; i < 9; i++)
            {
              if (Screen_Chess[i] == 0x00)
                Clear_chess(i + 1);
              else if (Screen_Chess[i] == 0x01)
                draw_chess(i + 1, 1);
              else if (Screen_Chess[i] == 0x02)
                draw_chess(i + 1, 0);
            }
            lcd_show_string(200, 0, 800, 50, 32, "Game Over:Black Win!", BLACK);
          delay_ms(2000);

          break;
        }
        else if(state == 3)
        {
USART_Receive();
          update_board();
          lcd_clear(BOARD_COLOR);

          draw_board();

            for (int i = 0; i < 9; i++)
            {
              if (Screen_Chess[i] == 0x00)
                Clear_chess(i + 1);
              else if (Screen_Chess[i] == 0x01)
                draw_chess(i + 1, 1);
              else if (Screen_Chess[i] == 0x02)
                draw_chess(i + 1, 0);
            }
            lcd_show_string(200, 0, 800, 50, 32, "Game Win:World Peace!", BLACK);
          delay_ms(2000);

          break;
        }
        
      }
    }
     default:
      break;
    }
    break;
  }
}