
 #include "./BSP/LED/led.h"


/**
 * @brief       初始化LED相关IO口, 并使能时钟
 * @param       无
 * @retval      无
 */
void led_init(void)
{
    LED0_GPIO_CLK_ENABLE(); /* LED0时钟使能 */
    LED1_GPIO_CLK_ENABLE(); /* LED1时钟使能 */

    sys_gpio_set(LED0_GPIO_PORT, LED0_GPIO_PIN,
                 SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_MID, SYS_GPIO_PUPD_PU);   /* LED0引脚模式设置 */

    sys_gpio_set(LED1_GPIO_PORT, LED1_GPIO_PIN,
                 SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_MID, SYS_GPIO_PUPD_PU);   /* LED1引脚模式设置 */

    LED0(1);    /* 关闭 LED0 */
    LED1(1);    /* 关闭 LED1 */
}













