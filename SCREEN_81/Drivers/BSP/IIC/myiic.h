
 
#ifndef __MYIIC_H
#define __MYIIC_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* ���� ���� */

#define IIC_SCL_GPIO_PORT               GPIOH
#define IIC_SCL_GPIO_PIN                SYS_GPIO_PIN4
#define IIC_SCL_GPIO_CLK_ENABLE()       do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* PH��ʱ��ʹ�� */

#define IIC_SDA_GPIO_PORT               GPIOH
#define IIC_SDA_GPIO_PIN                SYS_GPIO_PIN5
#define IIC_SDA_GPIO_CLK_ENABLE()       do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* PH��ʱ��ʹ�� */

/******************************************************************************************/

/* IO�������� */
#define IIC_SCL(x)      sys_gpio_pin_set(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, x)    /* SCL */
#define IIC_SDA(x)      sys_gpio_pin_set(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, x)    /* SDA */
#define IIC_READ_SDA    sys_gpio_pin_get(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)       /* ��ȡSDA */

/* IIC���в������� */
void iic_init(void);                     /* ��ʼ��IIC��IO�� */
void iic_start(void);                    /* ����IIC��ʼ�ź� */
void iic_stop(void);                     /* ����IICֹͣ�ź� */
void iic_ack(void);                      /* IIC����ACK�ź� */
void iic_nack(void);                     /* IIC������ACK�ź� */
uint8_t iic_wait_ack(void);              /* IIC�ȴ�ACK�ź� */
void iic_send_byte(uint8_t data);        /* IIC����һ���ֽ� */
uint8_t iic_read_byte(unsigned char ack);/* IIC��ȡһ���ֽ� */
#endif
















