

#ifndef __TOUCH_H
#define __TOUCH_H

#include "./SYSTEM/sys/sys.h"
#include "./BSP/TOUCH/gt9xxx.h"
#include "./BSP/TOUCH/ft5206.h"


/******************************************************************************************/
/* ���败��������IC T_PEN/T_CS/T_MISO/T_MOSI/T_SCK ���� ���� */

#define T_PEN_GPIO_PORT                 GPIOH
#define T_PEN_GPIO_PIN                  SYS_GPIO_PIN7
#define T_PEN_GPIO_CLK_ENABLE()         do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* ����IO��ʱ��ʹ�� */

#define T_CS_GPIO_PORT                  GPIOI
#define T_CS_GPIO_PIN                   SYS_GPIO_PIN8
#define T_CS_GPIO_CLK_ENABLE()          do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* ����IO��ʱ��ʹ�� */

#define T_MISO_GPIO_PORT                GPIOG
#define T_MISO_GPIO_PIN                 SYS_GPIO_PIN3
#define T_MISO_GPIO_CLK_ENABLE()        do{ RCC->AHB1ENR |= 1 << 6; }while(0)   /* ����IO��ʱ��ʹ�� */

#define T_MOSI_GPIO_PORT                GPIOI
#define T_MOSI_GPIO_PIN                 SYS_GPIO_PIN3
#define T_MOSI_GPIO_CLK_ENABLE()        do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* ����IO��ʱ��ʹ�� */

#define T_CLK_GPIO_PORT                 GPIOH
#define T_CLK_GPIO_PIN                  SYS_GPIO_PIN6
#define T_CLK_GPIO_CLK_ENABLE()         do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* ����IO��ʱ��ʹ�� */

/******************************************************************************************/

/* ���败������������ */
#define T_PEN           sys_gpio_pin_get(T_PEN_GPIO_PORT, T_PEN_GPIO_PIN)           /* T_PEN */
#define T_MISO          sys_gpio_pin_get(T_MISO_GPIO_PORT, T_MISO_GPIO_PIN)         /* T_MISO */

#define T_MOSI(x)       sys_gpio_pin_set(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN, x)      /* T_MOSI */
#define T_CLK(x)        sys_gpio_pin_set(T_CLK_GPIO_PORT, T_CLK_GPIO_PIN, x)        /* T_CLK */
#define T_CS(x)         sys_gpio_pin_set(T_CS_GPIO_PORT, T_CS_GPIO_PIN, x)          /* T_CS */


#define TP_PRES_DOWN    0x8000      /* ���������� */
#define TP_CATH_PRES    0x4000      /* �а��������� */
#define CT_MAX_TOUCH    10          /* ������֧�ֵĵ���,�̶�Ϊ5�� */

/* ������������ */
typedef struct
{
    uint8_t (*init)(void);      /* ��ʼ�������������� */
    uint8_t (*scan)(uint8_t);   /* ɨ�败����.0,��Ļɨ��;1,��������; */
    void (*adjust)(void);       /* ������У׼ */
    uint16_t x[CT_MAX_TOUCH];   /* ��ǰ���� */
    uint16_t y[CT_MAX_TOUCH];   /* �����������10������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
                                 * x[9],y[9]�洢��һ�ΰ���ʱ������.
                                 */

    uint16_t sta;               /* �ʵ�״̬
                                 * b15:����1/�ɿ�0;
                                 * b14:0,û�а�������;1,�а�������.
                                 * b13~b10:����
                                 * b9~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
                                 */

    /* 5��У׼������У׼����(����������ҪУ׼) */
    float xfac;                 /* 5��У׼��x����������� */
    float yfac;                 /* 5��У׼��y����������� */
    short xc;                   /* ����X��������ֵ(ADֵ) */
    short yc;                   /* ����Y��������ֵ(ADֵ) */

    /* �����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
     * b0:0, ����(�ʺ�����ΪX����,����ΪY�����TP)
     *    1, ����(�ʺ�����ΪY����,����ΪX�����TP)
     * b1~6: ����.
     * b7:0, ������
     *    1, ������
     */
    uint8_t touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;        /* ������������touch.c���涨�� */


/* ���������� */

static void tp_write_byte(uint8_t data);                /* �����оƬд��һ������ */
static uint16_t tp_read_ad(uint8_t cmd);                /* ��ȡADת��ֵ */
static uint16_t tp_read_xoy(uint8_t cmd);               /* ���˲��������ȡ(X/Y) */
static void tp_read_xy(uint16_t *x, uint16_t *y);       /* ˫�����ȡ(X+Y) */
static uint8_t tp_read_xy2(uint16_t *x, uint16_t *y);   /* ����ǿ�˲���˫���������ȡ */
static void tp_draw_touch_point(uint16_t x, uint16_t y, uint16_t color);    /* ��һ������У׼�� */
static void tp_adjust_info_show(uint16_t xy[5][2], double px, double py);   /* ��ʾУ׼��Ϣ */

uint8_t tp_init(void);              /* ��ʼ�� */
uint8_t tp_scan(uint8_t mode);      /* ɨ�� */
void tp_adjust(void);               /* ������У׼ */
void tp_save_adjust_data(void);     /* ����У׼���� */
uint8_t tp_get_adjust_data(void);   /* ��ȡУ׼���� */
void tp_draw_big_point(uint16_t x, uint16_t y, uint16_t color); /* ��һ����� */

#endif

















