
#ifndef __LTDC_H
#define __LTDC_H

#include "./SYSTEM/sys/sys.h"


/* LCD LTDC��Ҫ������ */
typedef struct
{
    uint32_t pwidth;        /* LCD���Ŀ��,�̶�����,������ʾ����ı�,���Ϊ0,˵��û���κ�RGB������ */
    uint32_t pheight;       /* LCD���ĸ߶�,�̶�����,������ʾ����ı� */
    uint16_t hsw;           /* ˮƽͬ����� */
    uint16_t vsw;           /* ��ֱͬ����� */
    uint16_t hbp;           /* ˮƽ���� */
    uint16_t vbp;           /* ��ֱ���� */
    uint16_t hfp;           /* ˮƽǰ�� */
    uint16_t vfp;           /* ��ֱǰ�� */
    uint8_t activelayer;    /* ��ǰ����:0/1 */
    uint8_t dir;            /* 0,����;1,����; */
    uint16_t width;         /* LCD��� */
    uint16_t height;        /* LCD�߶� */
    uint32_t pixsize;       /* ÿ��������ռ�ֽ��� */
} _ltdc_dev;

extern _ltdc_dev lcdltdc;   /* ����LCD LTDC���� */



#define LTDC_PIXFORMAT_ARGB8888      0X00    /* ARGB8888��ʽ */
#define LTDC_PIXFORMAT_RGB888        0X01    /* RGB888��ʽ */
#define LTDC_PIXFORMAT_RGB565        0X02    /* RGB565��ʽ */
#define LTDC_PIXFORMAT_ARGB1555      0X03    /* ARGB1555��ʽ */
#define LTDC_PIXFORMAT_ARGB4444      0X04    /* ARGB4444��ʽ */
#define LTDC_PIXFORMAT_L8            0X05    /* L8��ʽ */
#define LTDC_PIXFORMAT_AL44          0X06    /* AL44��ʽ */
#define LTDC_PIXFORMAT_AL88          0X07    /* AL88��ʽ */

/******************************************************************************************/
/* LTDC_BL/DE/VSYNC/HSYNC/CLK ���� ���� 
 * LTDC_R3~R7, G2~G7, B3~B7,��������̫��,�Ͳ������ﶨ����,ֱ����ltcd_init�����޸�.��������ֲ��ʱ��,
 * ���˸���5��IO��, ���ø�ltcd_init�����R3~R7, G2~G7, B3~B7���ڵ�IO��.
 */

/* BL��MCU������Ź��� �������ﲻ�ö��� BL���� */
#define LTDC_BL_GPIO_PORT               GPIOB
#define LTDC_BL_GPIO_PIN                SYS_GPIO_PIN5
#define LTDC_BL_GPIO_CLK_ENABLE()       do{ RCC->AHB1ENR |= 1 << 1; }while(0)   /* ����IO��ʱ��ʹ�� */

#define LTDC_DE_GPIO_PORT               GPIOF
#define LTDC_DE_GPIO_PIN                SYS_GPIO_PIN10
#define LTDC_DE_GPIO_CLK_ENABLE()       do{ RCC->AHB1ENR |= 1 << 5; }while(0)   /* ����IO��ʱ��ʹ�� */

#define LTDC_VSYNC_GPIO_PORT            GPIOI
#define LTDC_VSYNC_GPIO_PIN             SYS_GPIO_PIN9
#define LTDC_VSYNC_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* ����IO��ʱ��ʹ�� */

#define LTDC_HSYNC_GPIO_PORT            GPIOI
#define LTDC_HSYNC_GPIO_PIN             SYS_GPIO_PIN10
#define LTDC_HSYNC_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* ����IO��ʱ��ʹ�� */

#define LTDC_CLK_GPIO_PORT              GPIOG
#define LTDC_CLK_GPIO_PIN               SYS_GPIO_PIN7
#define LTDC_CLK_GPIO_CLK_ENABLE()      do{ RCC->AHB1ENR |= 1 << 6; }while(0)   /* ����IO��ʱ��ʹ�� */



/* ������ɫ���ظ�ʽ,һ����RGB565 */
#define LTDC_PIXFORMAT              LTDC_PIXFORMAT_RGB565

/* ����Ĭ�ϱ�������ɫ */
#define LTDC_BACKLAYERCOLOR         0X00000000

/* LCD֡�������׵�ַ,���ﶨ����SDRAM���� */
#define LTDC_FRAME_BUF_ADDR         0XC0000000

/* LTDC������� */
#define LTDC_BL(x)                  sys_gpio_pin_set(LTDC_BL_GPIO_PORT, LTDC_BL_GPIO_PIN, x)

/******************************************************************************************/



void ltdc_switch(uint8_t sw);
void ltdc_layer_switch(uint8_t layerx, uint8_t sw);
void ltdc_select_layer(uint8_t layerx);
void ltdc_display_dir(uint8_t dir);
void ltdc_draw_point(uint16_t x, uint16_t y, uint32_t color);
uint32_t ltdc_read_point(uint16_t x, uint16_t y);
void ltdc_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color);
void ltdc_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);
void ltdc_clear(uint32_t color);
uint8_t ltdc_clk_set(uint32_t pllsain, uint32_t pllsair, uint32_t pllsaidivr);
void ltdc_layer_window_config(uint8_t layerx, uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);
void ltdc_layer_parameter_config(uint8_t layerx, uint32_t bufaddr, uint8_t pixformat, uint8_t alpha, uint8_t alpha0, uint8_t bfac1, uint8_t bfac2, uint32_t bkcolor);
uint16_t ltdc_panelid_read(void);
void ltdc_init(void);

#endif







