
#ifndef __KEY_H
#define __KEY_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* ���� ���� */

#define KEY0_GPIO_PORT                  GPIOH
#define KEY0_GPIO_PIN                   SYS_GPIO_PIN3
#define KEY0_GPIO_CLK_ENABLE()          do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* PH��ʱ��ʹ�� */

#define KEY1_GPIO_PORT                  GPIOH
#define KEY1_GPIO_PIN                   SYS_GPIO_PIN2
#define KEY1_GPIO_CLK_ENABLE()          do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* PH��ʱ��ʹ�� */

#define KEY2_GPIO_PORT                  GPIOC
#define KEY2_GPIO_PIN                   SYS_GPIO_PIN13
#define KEY2_GPIO_CLK_ENABLE()          do{ RCC->AHB1ENR |= 1 << 2; }while(0)   /* PC��ʱ��ʹ�� */

#define WKUP_GPIO_PORT                  GPIOA
#define WKUP_GPIO_PIN                   SYS_GPIO_PIN0
#define WKUP_GPIO_CLK_ENABLE()          do{ RCC->AHB1ENR |= 1 << 0; }while(0)   /* PA��ʱ��ʹ�� */

/******************************************************************************************/

#define KEY0        sys_gpio_pin_get(KEY0_GPIO_PORT, KEY0_GPIO_PIN)      /* ��ȡKEY0���� */
#define KEY1        sys_gpio_pin_get(KEY1_GPIO_PORT, KEY1_GPIO_PIN)      /* ��ȡKEY1���� */
#define KEY2        sys_gpio_pin_get(KEY2_GPIO_PORT, KEY2_GPIO_PIN)      /* ��ȡKEY2���� */
#define WK_UP       sys_gpio_pin_get(WKUP_GPIO_PORT, WKUP_GPIO_PIN)      /* ��ȡWKUP���� */

#define KEY0_PRES    1              /* KEY0���� */
#define KEY1_PRES    2              /* KEY1���� */
#define KEY2_PRES    3              /* KEY2���� */
#define WKUP_PRES    4              /* KEY_UP����(��WK_UP) */

void key_init(void);                /* ������ʼ������ */
uint8_t key_scan(uint8_t mode);     /* ����ɨ�躯�� */

#endif


















