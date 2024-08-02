
 
#ifndef __CTIIC_H
#define __CTIIC_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* CT_IIC ���� ���� */

#define CT_IIC_SCL_GPIO_PORT            GPIOH
#define CT_IIC_SCL_GPIO_PIN             SYS_GPIO_PIN6
#define CT_IIC_SCL_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* PH��ʱ��ʹ�� */

#define CT_IIC_SDA_GPIO_PORT            GPIOI
#define CT_IIC_SDA_GPIO_PIN             SYS_GPIO_PIN3
#define CT_IIC_SDA_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* PI��ʱ��ʹ�� */

/******************************************************************************************/

/* IO�������� */
#define CT_IIC_SCL(x)      sys_gpio_pin_set(CT_IIC_SCL_GPIO_PORT, CT_IIC_SCL_GPIO_PIN, x)   /* SCL */
#define CT_IIC_SDA(x)      sys_gpio_pin_set(CT_IIC_SDA_GPIO_PORT, CT_IIC_SDA_GPIO_PIN, x)   /* SDA */
#define CT_READ_SDA        sys_gpio_pin_get(CT_IIC_SDA_GPIO_PORT, CT_IIC_SDA_GPIO_PIN)      /* ��ȡSDA */


/* IIC���в������� */
void ct_iic_init(void);             /* ��ʼ��IIC��IO�� */
void ct_iic_stop(void);             /* ����IICֹͣ�ź� */
void ct_iic_start(void);            /* ����IIC��ʼ�ź� */

void ct_iic_ack(void);              /* IIC����ACK�ź� */
void ct_iic_nack(void);             /* IIC������ACK�ź� */
uint8_t ct_iic_wait_ack(void);      /* IIC�ȴ�ACK�ź� */

void ct_iic_send_byte(uint8_t txd);         /* IIC����һ���ֽ� */
uint8_t ct_iic_read_byte(unsigned char ack);/* IIC��ȡһ���ֽ� */

#endif







