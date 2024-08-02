

#ifndef __USMART_H
#define __USMART_H

#include "./USMART/usmart_port.h"


#define USMART_OK               0       /* �޴��� */
#define USMART_FUNCERR          1       /* �������� */
#define USMART_PARMERR          2       /* �������� */
#define USMART_PARMOVER         3       /* ������� */
#define USMART_NOFUNCFIND       4       /* δ�ҵ�ƥ�亯�� */

#define SP_TYPE_DEC             0       /* 10���Ʋ�����ʾ */
#define SP_TYPE_HEX             1       /* 16���Ʋ�����ʾ */



/* �������б� */
struct _m_usmart_nametab
{
    void *func;             /* ����ָ�� */
    const char *name;       /* ������(���Ҵ�) */
};

/* usmart���ƹ����� */
struct _m_usmart_dev
{
    struct _m_usmart_nametab *funs;     /* ������ָ�� */

    void (*init)(uint16_t tclk);        /* ��ʼ�� */
    uint8_t (*cmd_rec)(char *str);      /* ʶ������������ */
    void (*exe)(void);                  /* ִ��  */
    void (*scan)(void);                 /* ɨ�� */
    uint8_t fnum;                       /* �������� */
    uint8_t pnum;                       /* �������� */
    uint8_t id;                         /* ����id */
    uint8_t sptype;                     /* ������ʾ����(���ַ�������):0,10����;1,16����; */
    uint16_t parmtype;                  /* ���������� */
    uint8_t  plentbl[MAX_PARM];         /* ÿ�������ĳ����ݴ�� */
    uint8_t  parm[PARM_LEN];            /* �����Ĳ��� */
    uint8_t runtimeflag;                /* 0,��ͳ�ƺ���ִ��ʱ��;1,ͳ�ƺ���ִ��ʱ��,ע��:�˹��ܱ�����USMART_ENTIMX_SCANʹ�ܵ�ʱ��,������ */
    uint32_t runtime;                   /* ����ʱ��,��λ:0.1ms,�����ʱʱ��Ϊ��ʱ��CNTֵ��2��*0.1ms */
};

extern struct _m_usmart_nametab usmart_nametab[];   /* ��usmart_config.c���涨�� */
extern struct _m_usmart_dev usmart_dev;             /* ��usmart_config.c���涨�� */


void usmart_init(uint16_t tclk);             /* ��ʼ�� */
uint8_t usmart_cmd_rec(char * str);          /* ʶ�� */
void usmart_exe(void);                       /* ִ�� */
void usmart_scan(void);                      /* ɨ�� */
uint32_t read_addr(uint32_t addr);           /* ��ȡָ����ַ��ֵ */
void write_addr(uint32_t addr, uint32_t val);/* ��ָ����ַд��ָ����ֵ */

#endif






























