
#ifndef __USMART_PORT_H
#define __USMART_PORT_H

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"




/******************************************************************************************/
/* �û����ò��� */

#define MAX_FNAME_LEN           30      /* ��������󳤶ȣ�Ӧ������Ϊ��С����������ĳ��ȡ� */
#define MAX_PARM                10      /* ���Ϊ10������ ,�޸Ĵ˲���,�����޸�usmart_exe��֮��Ӧ. */
#define PARM_LEN                200     /* ���в���֮�͵ĳ��Ȳ�����PARM_LEN���ֽ�,ע�⴮�ڽ��ղ���Ҫ��֮��Ӧ(��С��PARM_LEN) */


#define USMART_ENTIMX_SCAN      1       /* ʹ��TIM�Ķ�ʱ�ж���ɨ��SCAN����,�������Ϊ0,��Ҫ�Լ�ʵ�ָ�һ��ʱ��ɨ��һ��scan����.
                                         * ע��:���Ҫ��runtimeͳ�ƹ���,��������USMART_ENTIMX_SCANΪ1!!!!
                                         */

#define USMART_USE_HELP         1       /* ʹ�ð�������ֵ��Ϊ0�����Խ�ʡ��700���ֽڣ����ǽ������޷���ʾ������Ϣ�� */
#define USMART_USE_WRFUNS       1       /* ʹ�ö�д����,ʹ������,���Զ�ȡ�κε�ַ��ֵ,������д�Ĵ�����ֵ. */

#define USMART_PRINTF           printf  /* ����printf��� */

/******************************************************************************************/
/* USMART��ʱ�� ���� */

# if USMART_ENTIMX_SCAN == 1    /* ������ʹ�ܶ�ʱ��ɨ��,����Ҫ���¶��� */

/* TIMX �ж϶��� 
 * ���ڶ�ʱ����usmart.scan����ɨ�贮������,��ִ����ز���
 * ע��: ͨ���޸���4���궨��,����֧��TIM1~TIM17����һ����ʱ��.
 */
#define USMART_TIMX                     TIM4
#define USMART_TIMX_IRQn                TIM4_IRQn
#define USMART_TIMX_IRQHandler          TIM4_IRQHandler
#define USMART_TIMX_CLK_ENABLE()        do{ RCC->APB1ENR |= 1 << 2; }while(0)   /* TIMX ʱ��ʹ�� */

#endif

/******************************************************************************************/


/* ���û�ж���uint32_t,���� */
#ifndef uint32_t
typedef unsigned           char uint8_t;
typedef unsigned short     int  uint16_t;
typedef unsigned           int  uint32_t;
#endif



char * usmart_get_input_string(void);   /* ��ȡ���������� */
void usmart_timx_reset_time(void);      /* ��λ����ʱ�� */
uint32_t usmart_timx_get_time(void);    /* ��ȡ����ʱ�� */
void usmart_timx_init(uint16_t tclk);   /* ��ʼ����ʱ�� */

#endif



























