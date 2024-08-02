/**
 ****************************************************************************************************
 * @file        usmart_port.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V3.5
 * @date        2022-12-24
 * @brief       USMART ���ڵ������
 *
 *              ͨ���޸ĸ��ļ�,���Է���Ľ�USMART��ֲ����������
 *              ��:USMART_ENTIMX_SCAN == 0ʱ,����Ҫʵ��: usmart_get_input_string����.
 *              ��:USMART_ENTIMX_SCAN == 1ʱ,��Ҫ��ʵ��4������:
 *              usmart_timx_reset_time
 *              usmart_timx_get_time
 *              usmart_timx_init
 *              USMART_TIMX_IRQHandler
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ F429������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵�� 
 * 
 * V3.4֮ǰ�汾��ϸ�޸�˵����USMART�ļ����µ�:readme.txt
 * 
 * V3.4 20200324
 * 1, ����usmart_port.c��usmart_port.h,���ڹ���USMART����ֲ,�����޸�
 * 2, �޸ı���������ʽΪ: uint8_t, uint16_t, uint32_t
 * 3, �޸�usmart_reset_runtimeΪusmart_timx_reset_time
 * 4, �޸�usmart_get_runtimeΪusmart_timx_get_time
 * 5, �޸�usmart_scan����ʵ�ַ�ʽ,�ĳ���usmart_get_input_string��ȡ������
 * 6, �޸�printf����ΪUSMART_PRINTF�궨��
 * 7, �޸Ķ�ʱɨ����غ���,���ú궨�巽ʽ,������ֲ
 *
 * V3.5 20201220
 * 1���޸Ĳ��ִ�����֧��AC6������
 *
 ****************************************************************************************************
 */

#include "./USMART/usmart.h"
#include "./USMART/usmart_port.h"

/**
 * @brief       ��ȡ����������(�ַ���)
 * @note        USMARTͨ�������ú������ص��ַ����Ի�ȡ����������������Ϣ
 * @param       ��
 * @retval      0,  û�н��յ�����
 *              ����,�������׵�ַ(������0)
 */
char *usmart_get_input_string(void)
{
    uint8_t len;
    char *pbuf = 0;

    if (g_usart_rx_sta & 0x8000)        /* ���ڽ�����ɣ� */
    {
        len = g_usart_rx_sta & 0x3fff;  /* �õ��˴ν��յ������ݳ��� */
        g_usart_rx_buf[len] = '\0';     /* ��ĩβ���������. */
        pbuf = (char*)g_usart_rx_buf;
        g_usart_rx_sta = 0;             /* ������һ�ν��� */
    }

    return pbuf;
}

/* ���ʹ���˶�ʱ��ɨ��, ����Ҫ�������º��� */
#if USMART_ENTIMX_SCAN == 1

/**
 * ��ֲע��:��������stm32Ϊ��,���Ҫ��ֲ������mcu,������Ӧ�޸�.
 * usmart_reset_runtime,�����������ʱ��,��ͬ��ʱ���ļ����Ĵ����Լ���־λһ������.��������װ��ֵΪ���,������޶ȵ��ӳ���ʱʱ��.
 * usmart_get_runtime,��ȡ��������ʱ��,ͨ����ȡCNTֵ��ȡ,����usmart��ͨ���жϵ��õĺ���,���Զ�ʱ���жϲ�����Ч,��ʱ����޶�
 * ֻ��ͳ��2��CNT��ֵ,Ҳ���������+���һ��,���������2��,û������,���������ʱ,������:2*������CNT*0.1ms.��STM32��˵,��:13.1s����
 * ������:USMART_TIMX_IRQHandler��Timer4_Init,��Ҫ����MCU�ص������޸�.ȷ������������Ƶ��Ϊ:10Khz����.����,��ʱ����Ҫ�����Զ���װ�ع���!!
 */

/**
 * @brief       ��λruntime
 * @note        ��Ҫ��������ֲ����MCU�Ķ�ʱ�����������޸�
 * @param       ��
 * @retval      ��
 */
void usmart_timx_reset_time(void)
{
    USMART_TIMX->SR &= ~(1 << 0);   /* ����жϱ�־λ */
    USMART_TIMX->ARR = 0XFFFF;      /* ����װ��ֵ���õ���� */
    USMART_TIMX->CNT = 0;           /* ��ն�ʱ����CNT */
    usmart_dev.runtime = 0;
}

/**
 * @brief       ���runtimeʱ��
 * @note        ��Ҫ��������ֲ����MCU�Ķ�ʱ�����������޸�
 * @param       ��
 * @retval      ִ��ʱ��,��λ:0.1ms,�����ʱʱ��Ϊ��ʱ��CNTֵ��2��*0.1ms
 */
uint32_t usmart_timx_get_time(void)
{
    if (USMART_TIMX->SR & 0X0001)   /* �������ڼ�,�����˶�ʱ����� */
    {
        usmart_dev.runtime += 0XFFFF;
    }

    usmart_dev.runtime += USMART_TIMX->CNT;
    return usmart_dev.runtime;      /* ���ؼ���ֵ */
}

/**
 * @brief       ��ʱ����ʼ������
 * @param       tclk: ��ʱ���Ĺ���Ƶ��(��λ:Mhz)
 * @retval      ��
 */
void usmart_timx_init(uint16_t tclk)
{
    USMART_TIMX_CLK_ENABLE();   /* TIMX ʱ��ʹ�� */
    USMART_TIMX->ARR = 1000;    /* �趨�������Զ���װֵ */

    /* ����tclk��ֵ(��λ:Mhz),����ʱ��������ʱ�ӷ�Ƶ��10KHz ,100ms�ж�һ��
     * ע��,����Ƶ�ʱ���Ϊ10Khz,�Ժ�runtime��λ(0.1ms)ͬ��.
     */
    USMART_TIMX->PSC = (tclk * 100) - 1;/* �����Ƶϵ�� */

    USMART_TIMX->DIER |= 1 << 0;/* ��������ж� */
    USMART_TIMX->CR1 |= 0x01;   /* ʹ�ܶ�ʱ��4 */
    sys_nvic_init(3, 3, USMART_TIMX_IRQn, 2);   /* ��ռ3�������ȼ�3����2(��2�����ȼ���͵�) */
}

/**
 * @brief       USMART��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
 */
void USMART_TIMX_IRQHandler(void)
{
    if (USMART_TIMX->SR & 0X0001)   /* ����ж� */
    {
        usmart_dev.scan();          /* ִ��usmartɨ�� */
        USMART_TIMX->CNT = 0;       /* ��ն�ʱ����CNT */
        USMART_TIMX->ARR = 1000;    /* �ָ�ԭ�������� */
    }

    USMART_TIMX->SR &= ~(1 << 0);   /* ����жϱ�־λ */
}

#endif
















