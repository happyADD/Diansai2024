
 
#ifndef __CTIIC_H
#define __CTIIC_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* CT_IIC 引脚 定义 */

#define CT_IIC_SCL_GPIO_PORT            GPIOH
#define CT_IIC_SCL_GPIO_PIN             SYS_GPIO_PIN6
#define CT_IIC_SCL_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* PH口时钟使能 */

#define CT_IIC_SDA_GPIO_PORT            GPIOI
#define CT_IIC_SDA_GPIO_PIN             SYS_GPIO_PIN3
#define CT_IIC_SDA_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* PI口时钟使能 */

/******************************************************************************************/

/* IO操作函数 */
#define CT_IIC_SCL(x)      sys_gpio_pin_set(CT_IIC_SCL_GPIO_PORT, CT_IIC_SCL_GPIO_PIN, x)   /* SCL */
#define CT_IIC_SDA(x)      sys_gpio_pin_set(CT_IIC_SDA_GPIO_PORT, CT_IIC_SDA_GPIO_PIN, x)   /* SDA */
#define CT_READ_SDA        sys_gpio_pin_get(CT_IIC_SDA_GPIO_PORT, CT_IIC_SDA_GPIO_PIN)      /* 读取SDA */


/* IIC所有操作函数 */
void ct_iic_init(void);             /* 初始化IIC的IO口 */
void ct_iic_stop(void);             /* 发送IIC停止信号 */
void ct_iic_start(void);            /* 发送IIC开始信号 */

void ct_iic_ack(void);              /* IIC发送ACK信号 */
void ct_iic_nack(void);             /* IIC不发送ACK信号 */
uint8_t ct_iic_wait_ack(void);      /* IIC等待ACK信号 */

void ct_iic_send_byte(uint8_t txd);         /* IIC发送一个字节 */
uint8_t ct_iic_read_byte(unsigned char ack);/* IIC读取一个字节 */

#endif







