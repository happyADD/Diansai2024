
 #include "./BSP/LED/led.h"


/**
 * @brief       ��ʼ��LED���IO��, ��ʹ��ʱ��
 * @param       ��
 * @retval      ��
 */
void led_init(void)
{
    LED0_GPIO_CLK_ENABLE(); /* LED0ʱ��ʹ�� */
    LED1_GPIO_CLK_ENABLE(); /* LED1ʱ��ʹ�� */

    sys_gpio_set(LED0_GPIO_PORT, LED0_GPIO_PIN,
                 SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_MID, SYS_GPIO_PUPD_PU);   /* LED0����ģʽ���� */

    sys_gpio_set(LED1_GPIO_PORT, LED1_GPIO_PIN,
                 SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_MID, SYS_GPIO_PUPD_PU);   /* LED1����ģʽ���� */

    LED0(1);    /* �ر� LED0 */
    LED1(1);    /* �ر� LED1 */
}













