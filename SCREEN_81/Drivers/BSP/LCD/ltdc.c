

#include "./BSP/LCD/lcd.h"
#include "./BSP/LCD/ltdc.h"


#if !(__ARMCC_VERSION >= 6010050)   /* ����AC6����������ʹ��AC5������ʱ */

/* ���ݲ�ͬ����ɫ��ʽ,����֡�������� */
#if LTDC_PIXFORMAT == LTDC_PIXFORMAT_ARGB8888 || LTDC_PIXFORMAT == LTDC_PIXFORMAT_RGB888
    uint32_t ltdc_lcd_framebuf[1280][800] __attribute__((at(LTDC_FRAME_BUF_ADDR)));     /* ����������ֱ���ʱ,LCD�����֡���������С */
#else
    uint16_t ltdc_lcd_framebuf[1280][800] __attribute__((at(LTDC_FRAME_BUF_ADDR)));     /* ����������ֱ���ʱ,LCD�����֡���������С */
    //uint16_t ltdc_lcd_framebuf1[1280][800] __attribute__((at(LTDC_FRAME_BUF_ADDR + 1280 * 800 * 2)));  /* ʹ��LTDC��2ʱʹ��(Ĭ��ֻʹ��LTDC��1) */
#endif

#else   /* ʹ��AC6������ʱ */

/* ���ݲ�ͬ����ɫ��ʽ,����֡�������� */
#if LTDC_PIXFORMAT == LTDC_PIXFORMAT_ARGB8888 || LTDC_PIXFORMAT == LTDC_PIXFORMAT_RGB888
    uint32_t ltdc_lcd_framebuf[1280][800] __attribute__((section(".bss.ARM.__at_0XC0000000")));     /* ����������ֱ���ʱ,LCD�����֡���������С */
#else
    uint16_t ltdc_lcd_framebuf[1280][800] __attribute__((section(".bss.ARM.__at_0XC0000000")));     /* ����������ֱ���ʱ,LCD�����֡���������С */
#endif

#endif


uint32_t *g_ltdc_framebuf[2];       /* LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ����� */
_ltdc_dev lcdltdc;                  /* ����LCD LTDC����Ҫ���� */


/**
 * @brief       LTDC����
 * @param       sw          : 1,��; 0,�ر�;
 * @retval      ��
 */
void ltdc_switch(uint8_t sw)
{
    if (sw)
    {
        LTDC->GCR |= 1 << 0;    /* ��LTDC */
    }
    else
    {
        LTDC->GCR &= ~(1 << 0); /* �ر�LTDC */
    }
}

/**
 * @brief       LTDC����ָ����
 * @param       layerx      : 0,��һ��; 1,�ڶ���;
 * @param       sw          : 1,��;   0,�ر�;
 * @retval      ��
 */
void ltdc_layer_switch(uint8_t layerx, uint8_t sw)
{
    if (sw)
    {
        if (layerx == 0)LTDC_Layer1->CR |= 1 << 0;      /* ������1 */
        else LTDC_Layer2->CR |= 1 << 0;                 /* ������2 */
    }
    else
    {
        if (layerx == 0)LTDC_Layer1->CR &= ~(1 << 0);   /* �رղ�1 */
        else LTDC_Layer2->CR &= ~(1 << 0);              /* �رղ�2 */
    }

    LTDC->SRCR |= 1 << 0;                               /* ���¼������� */
}

/**
 * @brief       LTDCѡ���
 * @param       layerx      : 0,��һ��; 1,�ڶ���;
 * @retval      ��
 */
void ltdc_select_layer(uint8_t layerx)
{
    lcdltdc.activelayer = layerx;
}

/**
 * @brief       LTDC��ʾ��������
 * @param       dir         : 0,����; 1,����;
 * @retval      ��
 */
void ltdc_display_dir(uint8_t dir)
{
    lcdltdc.dir = dir;      /* ��ʾ���� */

    if (dir == 0)           /* ���� */
    {
        lcdltdc.width = lcdltdc.pheight;
        lcdltdc.height = lcdltdc.pwidth;
    }
    else if (dir == 1)      /* ���� */
    {
        lcdltdc.width = lcdltdc.pwidth;
        lcdltdc.height = lcdltdc.pheight;
    }
}

/**
 * @brief       LTDC���㺯��
 * @param       x,y         : ����
 * @param       color       : ��ɫֵ
 * @retval      ��
 */
void ltdc_draw_point(uint16_t x, uint16_t y, uint32_t color)
{
#if LTDC_PIXFORMAT == LTDC_PIXFORMAT_ARGB8888 || LTDC_PIXFORMAT == LTDC_PIXFORMAT_RGB888

    if (lcdltdc.dir)    /* ���� */
    {
        *(uint32_t *)((uint32_t)g_ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
    }
    else                /* ���� */
    {
        *(uint32_t *)((uint32_t)g_ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color;
    }

#else

    if (lcdltdc.dir)    /* ���� */
    {
        *(uint16_t *)((uint32_t)g_ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
    }
    else                /* ���� */
    {
        *(uint16_t *)((uint32_t)g_ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color;
    }

#endif
}

/**
 * @brief       LTDC���㺯��
 * @param       x,y         : ����
 * @retval      ��ɫֵ
 */
uint32_t ltdc_read_point(uint16_t x, uint16_t y)
{
#if LTDC_PIXFORMAT == LTDC_PIXFORMAT_ARGB8888 || LTDC_PIXFORMAT == LTDC_PIXFORMAT_RGB888

    if (lcdltdc.dir)    /* ���� */
    {
        return *(uint32_t *)((uint32_t)g_ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
    }
    else                /* ���� */
    {
        return *(uint32_t *)((uint32_t)g_ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y));
    }

#else

    if (lcdltdc.dir)    /* ���� */
    {
        return *(uint16_t *)((uint32_t)g_ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
    }
    else                /* ���� */
    {
        return *(uint16_t *)((uint32_t)g_ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y));
    }

#endif
}

/**
 * @brief       LTDC������, DMA2D���
 * @note       (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex - sx + 1) * (ey - sy + 1)
 *              ע��:sx,ex,���ܴ���lcddev.width - 1; sy,ey,���ܴ���lcddev.height - 1
 * @param       sx,sy       : ��ʼ����
 * @param       ex,ey       : ��������
 * @param       color       : ������ɫ
 * @retval      ��
 */
void ltdc_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint32_t psx, psy, pex, pey;    /* ��LCD���Ϊ��׼������ϵ,����������仯���仯 */
    uint32_t timeout = 0;
    uint16_t offline;
    uint32_t addr;

    /* ����ϵת�� */
    if (lcdltdc.dir)    /* ���� */
    {
        psx = sx;
        psy = sy;
        pex = ex;
        pey = ey;
    }
    else                /* ���� */
    {
        if(ex >= lcdltdc.pheight)ex = lcdltdc.pheight - 1;  /* ���Ʒ�Χ */
        if(sx >= lcdltdc.pheight)sx = lcdltdc.pheight - 1;  /* ���Ʒ�Χ */
        
        psx = sy;
        psy = lcdltdc.pheight - ex - 1;
        pex = ey;
        pey = lcdltdc.pheight - sx - 1;
    }

    offline = lcdltdc.pwidth - (pex - psx + 1);
    addr = ((uint32_t)g_ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
    RCC->AHB1ENR |= 1 << 23;        /* ʹ��DM2Dʱ�� */
    DMA2D->CR &= ~(1 << 0);         /* ��ֹͣDMA2D */
    DMA2D->CR = 3 << 16;            /* �Ĵ������洢��ģʽ */
    DMA2D->OPFCCR = LTDC_PIXFORMAT; /* ������ɫ��ʽ */
    DMA2D->OOR = offline;           /* ������ƫ�� */
    DMA2D->OMAR = addr;             /* ����洢����ַ */
    DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16); /* �趨�����Ĵ��� */
    DMA2D->OCOLR = color;           /* �趨�����ɫ�Ĵ��� */
    DMA2D->CR |= 1 << 0;            /* ����DMA2D */

    while ((DMA2D->ISR & (1 << 1)) == 0)    /* �ȴ�������� */
    {
        timeout++;

        if (timeout > 0X1FFFFF)break;       /* ��ʱ�˳� */
    }

    DMA2D->IFCR |= 1 << 1;          /* ���������ɱ�־ */
}

/**
 * @brief       ��ָ�����������ָ����ɫ��, DMA2D���
 * @note        �˺�����֧��uint16_t,RGB565��ʽ����ɫ�������.
 *              (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex - sx + 1) * (ey - sy + 1)
 *              ע��:sx,ex,���ܴ���lcddev.width - 1; sy,ey,���ܴ���lcddev.height - 1
 * @param       sx,sy       : ��ʼ����
 * @param       ex,ey       : ��������
 * @param       color       : ������ɫ�����׵�ַ
 * @retval      ��
 */
void ltdc_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
    uint32_t psx, psy, pex, pey;    /* ��LCD���Ϊ��׼������ϵ,����������仯���仯 */
    uint32_t timeout = 0;
    uint16_t offline;
    uint32_t addr;

    /* ����ϵת�� */
    if (lcdltdc.dir)    /* ���� */
    {
        psx = sx;
        psy = sy;
        pex = ex;
        pey = ey;
    }
    else                /* ���� */
    {
        psx = sy;
        psy = lcdltdc.pheight - ex - 1;
        pex = ey;
        pey = lcdltdc.pheight - sx - 1;
    }

    offline = lcdltdc.pwidth - (pex - psx + 1);
    addr = ((uint32_t)g_ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
    RCC->AHB1ENR |= 1 << 23;            /* ʹ��DM2Dʱ�� */
    DMA2D->CR &= ~(1 << 0);             /* ��ֹͣDMA2D */
    DMA2D->CR = 0 << 16;                /* �洢�����洢��ģʽ */
    DMA2D->FGPFCCR = LTDC_PIXFORMAT;    /* ������ɫ��ʽ */
    DMA2D->FGOR = 0;                    /* ǰ������ƫ��Ϊ0 */
    DMA2D->OOR = offline;               /* ������ƫ�� */
    DMA2D->FGMAR = (uint32_t)color;     /* Դ��ַ */
    DMA2D->OMAR = addr;                 /* ����洢����ַ */
    DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16); /* �趨�����Ĵ��� */
    DMA2D->CR |= 1 << 0;                /* ����DMA2D */

    while ((DMA2D->ISR & (1 << 1)) == 0)/* �ȴ�������� */
    {
        timeout++;

        if (timeout > 0X1FFFFF)break;   /* ��ʱ�˳� */
    }

    DMA2D->IFCR |= 1 << 1;              /* ���������ɱ�־ */
}

/**
 * @brief       LTDC����
 * @param       color       : ��ɫֵ
 * @retval      ��
 */
void ltdc_clear(uint32_t color)
{
    ltdc_fill(0, 0, lcdltdc.width - 1, lcdltdc.height - 1, color);
}

/**
 * @brief       LTDCʱ��(Fdclk)���ú���
 * @param       pllsain     : SAIʱ�ӱ�Ƶϵ��N,   ȡֵ��Χ:50~432.
 * @param       pllsair     : SAIʱ�ӵķ�Ƶϵ��R, ȡֵ��Χ:2~7
 * @param       pllsaidivr  : LCDʱ�ӷ�Ƶϵ��,    ȡֵ��Χ:0~3, ��Ӧ��Ƶ 2^(pllsaidivr + 1)
 *
 * @note        Fvco  = Fin * pllsain;
 *              Fdclk = Fvco / pllsair / 2 * 2^pllsaidivr = Fin * pllsain / pllsair / 2 * 2^pllsaidivr;
 *              ����:
 *              Fvco:VCOƵ��
 *              Fin:����ʱ��Ƶ��һ��Ϊ1Mhz(����ϵͳʱ��PLLM��Ƶ���ʱ��,��ʱ����ͼ)
 *              ����:�ⲿ����Ϊ25M, pllm = 25 ��ʱ��, Fin = 1Mhz.
 *              ����:Ҫ�õ�33M��LTDCʱ��, ���������: pllsain = 396, pllsair = 3, pllsaidivr = 1
 *              Fdclk= 1 * 396 / 3 / 2 * 2^1 = 396 / 12 = 33Mhz
 * @retval      0, �ɹ�;
 *              ����, ʧ��;
 */
uint8_t ltdc_clk_set(uint32_t pllsain, uint32_t pllsair, uint32_t pllsaidivr)
{
    uint16_t retry = 0;
    uint8_t status = 0;
    uint32_t tempreg = 0;
    
    RCC->CR &= ~(1 << 28);  /* �ر�SAIʱ�� */

    while (((RCC->CR & (1 << 29))) && (retry < 0X1FFF))retry++; /* �ȴ�SAIʱ��ʧ�� */

    if (retry == 0X1FFF)status = 1;         /* LTDCʱ�ӹر�ʧ�� */
    else
    {
        tempreg |= pllsain << 6;
        tempreg |= pllsair << 28;
        RCC->PLLSAICFGR = tempreg;          /* ����LTDC�ı�Ƶ�ͷ�Ƶ */
        RCC->DCKCFGR &= ~(3 << 16);         /* ���ԭ��������. */
        RCC->DCKCFGR |= pllsaidivr << 16;   /* ����fdclk��Ƶ */

        RCC->CR |= 1 << 28;                 /* ����SAIʱ�� */

        while (((RCC->CR & (1 << 29)) == 0) && (retry < 0X1FFF))retry++; /* �ȴ�SAIʱ������ */

        if (retry == 0X1FFF)status = 2;
    }

    return status;
}

/**
 * @brief       LTDC�㴰������, ������LCD�������ϵΪ��׼
 * @note        �˺���������ltdc_layer_parameter_config֮��������.����,�����õĴ���ֵ���������ĳ�
 *              ��ʱ,GRAM�Ĳ���(��/д�㺯��),ҲҪ���ݴ��ڵĿ���������޸�,������ʾ������(�����̾�δ���޸�).
 * @param       layerx      : 0,��һ��; 1,�ڶ���;
 * @param       sx, sy      : ��ʼ����
 * @param       width,height: ��Ⱥ͸߶�
 * @retval      ��
 */
void ltdc_layer_window_config(uint8_t layerx, uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint32_t temp;
    uint8_t pixformat = 0;

    if (layerx == 0)
    {
        temp = (sx + width + ((LTDC->BPCR & 0X0FFF0000) >> 16)) << 16;
        LTDC_Layer1->WHPCR = (sx + ((LTDC->BPCR & 0X0FFF0000) >> 16) + 1) | temp;   /* ��������ʼ�ͽ���λ�� */
        temp = (sy + height + (LTDC->BPCR & 0X7FF)) << 16;
        LTDC_Layer1->WVPCR = (sy + (LTDC->BPCR & 0X7FF) + 1) | temp;    /* ��������ʼ�ͽ���λ�� */
        pixformat = LTDC_Layer1->PFCR & 0X07;                           /* �õ���ɫ��ʽ */

        if (pixformat == 0)temp = 4;                                    /* ARGB8888,һ����4���ֽ� */
        else if (pixformat == 1)temp = 3;                               /* RGB888,һ����3���ֽ� */
        else if (pixformat == 5 || pixformat == 6)temp = 1;             /* L8/AL44,һ����1���ֽ� */
        else temp = 2;                                                  /* ������ʽ,һ����2���ֽ� */

        LTDC_Layer1->CFBLR = (width * temp << 16) | (width * temp + 3); /* ֡��������������(�ֽ�Ϊ��λ) */
        LTDC_Layer1->CFBLNR = height;                                   /* ֡�������������� */
    }
    else
    {
        temp = (sx + width + ((LTDC->BPCR & 0X0FFF0000) >> 16)) << 16;
        LTDC_Layer2->WHPCR = (sx + ((LTDC->BPCR & 0X0FFF0000) >> 16) + 1) | temp;   /* ��������ʼ�ͽ���λ�� */
        temp = (sy + height + (LTDC->BPCR & 0X7FF)) << 16;
        LTDC_Layer2->WVPCR = (sy + (LTDC->BPCR & 0X7FF) + 1) | temp;    /* ��������ʼ�ͽ���λ�� */
        pixformat = LTDC_Layer2->PFCR & 0X07;                           /* �õ���ɫ��ʽ */

        if (pixformat == 0)temp = 4;                                    /* ARGB8888,һ����4���ֽ� */
        else if (pixformat == 1)temp = 3;                               /* RGB888,һ����3���ֽ� */
        else if (pixformat == 5 || pixformat == 6)temp = 1;             /* L8/AL44,һ����1���ֽ� */
        else temp = 2;                                                  /* ������ʽ,һ����2���ֽ� */

        LTDC_Layer2->CFBLR = (width * temp << 16) | (width * temp + 3); /* ֡��������������(�ֽ�Ϊ��λ) */
        LTDC_Layer2->CFBLNR = height;                                   /* ֡�������������� */
    }

    ltdc_layer_switch(layerx, 1);                                       /* ��ʹ�� */
}


/**
 * @brief       LTDC�������������
 * @note        �˺���,������ltdc_layer_window_config֮ǰ����.
 * @param       layerx      : 0,��һ��; 1,�ڶ���;
 * @param       bufaddr     : ����ɫ֡������ʼ��ַ
 * @param       pixformat   : ��ɫ��ʽ. 0,ARGB8888; 1,RGB888; 2,RGB565; 3,ARGB1555; 4,ARGB4444; 5,L8; 6;AL44; 7;AL88
 * @param       alpha       : ����ɫAlphaֵ, 0,ȫ͸��;255,��͸��
 * @param       alpha0      : Ĭ����ɫAlphaֵ, 0,ȫ͸��;255,��͸��
 * @param       bfac1       : ���ϵ��1, 4(100),�㶨��Alpha; 6(101),����Alpha*�㶨Alpha
 * @param       bfac2       : ���ϵ��2, 5(101),�㶨��Alpha; 7(111),����Alpha*�㶨Alpha
 * @param       bkcolor     : ��Ĭ����ɫ,32λ,��24λ��Ч,RGB888��ʽ
 * @retval      ��
 */
void ltdc_layer_parameter_config(uint8_t layerx, uint32_t bufaddr, uint8_t pixformat, uint8_t alpha, uint8_t alpha0, uint8_t bfac1, uint8_t bfac2, uint32_t bkcolor)
{
    if (layerx == 0)
    {
        LTDC_Layer1->CFBAR = bufaddr;                           /* ���ò���ɫ֡������ʼ��ַ */
        LTDC_Layer1->PFCR = pixformat;                          /* ���ò���ɫ��ʽ */
        LTDC_Layer1->CACR = alpha;                              /* ���ò���ɫAlphaֵ,255��Ƶ;����255,��͸�� */
        LTDC_Layer1->DCCR = ((uint32_t)alpha0 << 24) | bkcolor; /* ����Ĭ����ɫAlphaֵ,�Լ�Ĭ����ɫ */
        LTDC_Layer1->BFCR = ((uint32_t)bfac1 << 8) | bfac2;     /* ���ò���ϵ�� */
    }
    else
    {
        LTDC_Layer2->CFBAR = bufaddr;                           /* ���ò���ɫ֡������ʼ��ַ */
        LTDC_Layer2->PFCR = pixformat;                          /* ���ò���ɫ��ʽ */
        LTDC_Layer2->CACR = alpha;                              /* ���ò���ɫAlphaֵ,255��Ƶ;����255,��͸�� */
        LTDC_Layer2->DCCR = ((uint32_t)alpha0 << 24) | bkcolor; /* ����Ĭ����ɫAlphaֵ,�Լ�Ĭ����ɫ */
        LTDC_Layer2->BFCR = ((uint32_t)bfac1 << 8) | bfac2;     /* ���ò���ϵ�� */
    }
}

/**
 * @brief       LTDC��ȡ���ID
 * @note        ����LCD RGB�ߵ����λ(R7,G7,B7)��ʶ�����ID
 *              PG6 = R7(M0); PI2 = G7(M1); PI7 = B7(M2);
 *              M2:M1:M0
 *              0 :0 :0     4.3 ��480*272  RGB��,ID = 0X4342
 *              0 :0 :1     7   ��800*480  RGB��,ID = 0X7084
 *              0 :1 :0     7   ��1024*600 RGB��,ID = 0X7016
 *              0 :1 :1     7   ��1280*800 RGB��,ID = 0X7018
 *              1 :0 :0     4.3 ��800*480  RGB��,ID = 0X4348
 *              1 :0 :1     10.1��1280*800 RGB��,ID = 0X1018
 * @param       ��
 * @retval      0, �Ƿ�; 
 *              ����, LCD ID
 */
uint16_t ltdc_panelid_read(void)
{
    uint8_t idx = 0;
    
    RCC->AHB1ENR |= 1 << 6 | 1 << 8;    /* ʹ��PG/PIʱ�� */

    sys_gpio_set(GPIOG, SYS_GPIO_PIN6,
                 SYS_GPIO_MODE_IN, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);   /* R7������������ */
    
    sys_gpio_set(GPIOI, SYS_GPIO_PIN2 | SYS_GPIO_PIN7,
                 SYS_GPIO_MODE_IN, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);   /* G7, B7������������ */

    idx  = sys_gpio_pin_get(GPIOG, SYS_GPIO_PIN6);      /* ��ȡM0 */
    idx |= sys_gpio_pin_get(GPIOI, SYS_GPIO_PIN2) << 1; /* ��ȡM1 */
    idx |= sys_gpio_pin_get(GPIOI, SYS_GPIO_PIN7) << 2; /* ��ȡM2 */
	
   idx=4;  ////Ĭ��800*480�ֱ�����
	
    switch (idx)
    {
        case 0:
            return 0X4342;      /* 4.3����,480*272�ֱ��� */

        case 1:
            return 0X7084;      /* 7����,800*480�ֱ��� */

        case 2:
            return 0X7016;      /* 7����,1024*600�ֱ��� */

        case 3:
            return 0X7018;      /* 7����,1280*800�ֱ��� */

        case 4:
            return 0X4384;      /* 4.3����,800*480�ֱ��� */

        case 5:
            return 0X1018;      /* 10.1����,1280*800�ֱ��� */

        default:
            return 0;
    }
}

/**
 * @brief       LTDC��ʼ������
 * @param       ��
 * @retval      ��
 */
void ltdc_init(void)
{
    uint32_t tempreg = 0;
    uint16_t lcdid = 0;
    
    lcdid = ltdc_panelid_read();    /* ��ȡLCD���ID */

    /* ������LTDC�źſ������� BL/DE/VSYNC/HSYNC/CLK�ȵ����� */
    LTDC_BL_GPIO_CLK_ENABLE();      /* LTDC_BL��ʱ��ʹ�� */
    LTDC_DE_GPIO_CLK_ENABLE();      /* LTDC_DE��ʱ��ʹ�� */
    LTDC_VSYNC_GPIO_CLK_ENABLE();   /* LTDC_VSYNC��ʱ��ʹ�� */
    LTDC_HSYNC_GPIO_CLK_ENABLE();   /* LTDC_HSYNC��ʱ��ʹ�� */
    LTDC_CLK_GPIO_CLK_ENABLE();     /* LTDC_CLK��ʱ��ʹ�� */

    sys_gpio_set(LTDC_BL_GPIO_PORT, LTDC_BL_GPIO_PIN,
                 SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);  /* LTDC_BL����ģʽ����(�������) */
          
    sys_gpio_set(LTDC_DE_GPIO_PORT, LTDC_DE_GPIO_PIN,
                 SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);   /* LTDC_DE����ģʽ���� */

    sys_gpio_set(LTDC_VSYNC_GPIO_PORT, LTDC_VSYNC_GPIO_PIN,
                 SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);   /* LTDC_VSYNC����ģʽ���� */

    sys_gpio_set(LTDC_HSYNC_GPIO_PORT, LTDC_HSYNC_GPIO_PIN,
                 SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);   /* LTDC_HSYNC����ģʽ���� */

    sys_gpio_set(LTDC_CLK_GPIO_PORT, LTDC_CLK_GPIO_PIN,
                 SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);   /* LTDC_CLK����ģʽ���� */

    sys_gpio_af_set(LTDC_DE_GPIO_PORT, LTDC_DE_GPIO_PIN, 14);       /* LTDC_DE��, AF14 */
    sys_gpio_af_set(LTDC_VSYNC_GPIO_PORT, LTDC_VSYNC_GPIO_PIN, 14); /* LTDC_VSYNC��, AF14 */
    sys_gpio_af_set(LTDC_HSYNC_GPIO_PORT, LTDC_HSYNC_GPIO_PIN, 14); /* LTDC_HSYNC��, AF14 */
    sys_gpio_af_set(LTDC_CLK_GPIO_PORT, LTDC_CLK_GPIO_PIN, 14);     /* LTDC_CLK��, AF14 */

    /* ������LTDC �������ŵ����� */
    RCC->APB2ENR |= 1 << 26;            /* ����LTDCʱ�� */
    RCC->AHB1ENR |= 0X7 << 6;           /* ʹ��PG/PH/PIʱ�� */
    
    sys_gpio_set(GPIOG, SYS_GPIO_PIN6 | SYS_GPIO_PIN11,
                 SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);   /* LTDC ��������ģʽ���� */
                 
    sys_gpio_set(GPIOH, 0X7F << 9,
                 SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);   /* LTDC ��������ģʽ���� */

    sys_gpio_set(GPIOI, 7 << 0 | 0XF << 4,
                 SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);   /* LTDC ��������ģʽ���� */

    sys_gpio_af_set(GPIOG, SYS_GPIO_PIN6 | SYS_GPIO_PIN11, 14);     /* LTDC ���ݽ�, AF14 */
    sys_gpio_af_set(GPIOH, 0X7F << 9, 14);                          /* LTDC ���ݽ�, AF14 */
    sys_gpio_af_set(GPIOI, 7 << 0 | 0XF << 4, 14);                  /* LTDC ���ݽ�, AF14 */

    if (lcdid == 0X4342)
    {
        lcdltdc.pwidth = 480;       /* �����,��λ:���� */
        lcdltdc.pheight = 272;      /* ���߶�,��λ:���� */
        lcdltdc.hsw = 1;            /* ˮƽͬ����� */
        lcdltdc.vsw = 1;            /* ��ֱͬ����� */
        lcdltdc.hbp = 40;           /* ˮƽ���� */
        lcdltdc.vbp = 8;            /* ��ֱ���� */
        lcdltdc.hfp = 5;            /* ˮƽǰ�� */
        lcdltdc.vfp = 8;            /* ��ֱǰ�� */
        ltdc_clk_set(288, 4, 2);    /* ��������ʱ��  9Mhz */
    }
    else if (lcdid == 0X7084)
    {
        lcdltdc.pwidth = 800;       /* �����,��λ:���� */
        lcdltdc.pheight = 480;      /* ���߶�,��λ:���� */
        lcdltdc.hsw = 1;            /* ˮƽͬ����� */
        lcdltdc.vsw = 1;            /* ��ֱͬ����� */
        lcdltdc.hbp = 46;           /* ˮƽ���� */
        lcdltdc.vbp = 23;           /* ��ֱ���� */
        lcdltdc.hfp = 210;          /* ˮƽǰ�� */
        lcdltdc.vfp = 22;           /* ��ֱǰ�� */
        ltdc_clk_set(396, 3, 1);    /* ��������ʱ�� 33M(�����˫��,��Ҫ����DCLK��:18.75Mhz  300/4/4,�Ż�ȽϺ�) */
    }
    else if (lcdid == 0X7016)
    {
        lcdltdc.pwidth = 1024;      /* �����,��λ:���� */
        lcdltdc.pheight = 600;      /* ���߶�,��λ:���� */
        lcdltdc.hsw = 20;           /* ˮƽͬ����� */
        lcdltdc.vsw = 3;            /* ��ֱͬ����� */
        lcdltdc.hbp = 140;          /* ˮƽ���� */
        lcdltdc.vbp = 20;           /* ��ֱ���� */
        lcdltdc.hfp = 160;          /* ˮƽǰ�� */
        lcdltdc.vfp = 12;           /* ��ֱǰ�� */
        ltdc_clk_set(360, 2, 1);    /* ��������ʱ��  45Mhz */
    }
    else if (lcdid == 0X7018)
    {
        lcdltdc.pwidth = 1280;      /* �����,��λ:���� */
        lcdltdc.pheight = 800;      /* ���߶�,��λ:���� */
        /* ������������ */
    }
    else if (lcdid == 0X4384)
    {
        lcdltdc.pwidth = 800;       /* �����,��λ:���� */
        lcdltdc.pheight = 480;      /* ���߶�,��λ:���� */
        lcdltdc.hbp = 88;           /* ˮƽ���� */
        lcdltdc.hfp = 40;           /* ˮƽǰ�� */
        lcdltdc.hsw = 48;           /* ˮƽͬ����� */
        lcdltdc.vbp = 32;           /* ��ֱ���� */
        lcdltdc.vfp = 13;           /* ��ֱǰ�� */
        lcdltdc.vsw = 3;            /* ��ֱͬ����� */
        ltdc_clk_set(396, 3, 1);    /* ��������ʱ�� 33M */ 
    }
    else if (lcdid == 0X1018)       /* 10.1��1280*800 RGB�� */
    {
        lcdltdc.pwidth = 1280;      /* �����,��λ:���� */
        lcdltdc.pheight = 800;      /* ���߶�,��λ:���� */
        lcdltdc.hbp = 140;          /* ˮƽ���� */
        lcdltdc.hfp = 10;           /* ˮƽǰ�� */
        lcdltdc.hsw = 10;           /* ˮƽͬ����� */
        lcdltdc.vbp = 10;           /* ��ֱ���� */
        lcdltdc.vfp = 10;           /* ��ֱǰ�� */
        lcdltdc.vsw = 3;            /* ��ֱͬ����� */
        ltdc_clk_set(360, 2, 1);    /* ��������ʱ��  45Mhz */
    }

    if (lcdid == 0X1018)
    {
        tempreg = 1 << 28;          /* ����ʱ�Ӽ���:���� */
    }else
    {
        tempreg = 0 << 28;          /* ����ʱ�Ӽ���:������ */
    }

    tempreg |= 0 << 29;             /* ����ʹ�ܼ���:�͵�ƽ��Ч */
    tempreg |= 0 << 30;             /* ��ֱͬ������:�͵�ƽ��Ч */
    tempreg |= 0 << 31;             /* ˮƽͬ������:�͵�ƽ��Ч */
    LTDC->GCR = tempreg;            /* ����ȫ�ֿ��ƼĴ��� */
    tempreg = (lcdltdc.vsw - 1) << 0;   /* ��ֱ����-1 */
    tempreg |= (lcdltdc.hsw - 1) << 16; /* ˮƽ����-1 */
    LTDC->SSCR = tempreg;               /* ����ͬ����С���üĴ��� */

    tempreg = (lcdltdc.vsw + lcdltdc.vbp - 1) << 0;     /* �ۼӴ�ֱ����=��ֱ����+��ֱ����-1 */
    tempreg |= (lcdltdc.hsw + lcdltdc.hbp - 1) << 16;   /* �ۼ�ˮƽ����=ˮƽ����+ˮƽ����-1 */
    LTDC->BPCR = tempreg;                               /* ���ú������üĴ��� */

    tempreg = (lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight - 1) << 0;   /* �ۼ���Ч�߶�=��ֱ����+��ֱ����+��ֱ�ֱ���-1 */
    tempreg |= (lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth - 1) << 16;  /* �ۼ���Ч���=ˮƽ����+ˮƽ����+ˮƽ�ֱ���-1 */
    LTDC->AWCR = tempreg;                                               /* ������Ч������üĴ��� */

    tempreg = (lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight + lcdltdc.vfp - 1) << 0;     /* �ܸ߶�=��ֱ����+��ֱ����+��ֱ�ֱ���+��ֱǰ��-1 */
    tempreg |= (lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth + lcdltdc.hfp - 1) << 16;    /* �ܿ��=ˮƽ����+ˮƽ����+ˮƽ�ֱ���+ˮƽǰ��-1 */
    LTDC->TWCR = tempreg;   /* �����ܿ�����üĴ��� */

    LTDC->BCCR = LTDC_BACKLAYERCOLOR;   /* ���ñ�������ɫ�Ĵ���(RGB888��ʽ) */
    ltdc_switch(1);         /* ����LTDC */

#if LTDC_PIXFORMAT == LTDC_PIXFORMAT_ARGB8888 || LTDC_PIXFORMAT == LTDC_PIXFORMAT_RGB888
    g_ltdc_framebuf[0] = (uint32_t *)&ltdc_lcd_framebuf;
    lcdltdc.pixsize = 4;    /* ÿ������ռ4���ֽ� */
#else
    g_ltdc_framebuf[0] = (uint32_t *)&ltdc_lcd_framebuf;
    //g_ltdc_framebuf[1]=(uint32_t*)&ltdc_lcd_framebuf1;
    lcdltdc.pixsize = 2;    /* ÿ������ռ2���ֽ� */
#endif

    /* ������ */
    ltdc_layer_parameter_config(0, (uint32_t)g_ltdc_framebuf[0], LTDC_PIXFORMAT, 255, 0, 6, 7, 0X000000);   /* ��������� */
    ltdc_layer_window_config(0, 0, 0, lcdltdc.pwidth, lcdltdc.pheight);                                     /* �㴰������,��LCD�������ϵΪ��׼,��Ҫ����޸�! */

    //ltdc_layer_parameter_config(1,(uint32_t)g_ltdc_framebuf[1],LTDC_PIXFORMAT,127,0,6,7,0X000000);        /* ��������� */
    //ltdc_layer_window_config(1,0,0,lcdltdc.pwidth,lcdltdc.pheight);                                       /* �㴰������,��LCD�������ϵΪ��׼,��Ҫ����޸�! */

    lcddev.width = lcdltdc.pwidth;      /* ����lcddev�Ŀ�Ȳ��� */
    lcddev.height = lcdltdc.pheight;    /* ����lcddev�Ŀ�Ȳ��� */
    //ltdc_display_dir(0);              /* Ĭ����������LCD_Init������������ */
    ltdc_select_layer(0);               /* ѡ���1�� */
    LTDC_BL(1);                         /* �������� */
    ltdc_clear(0XFFFFFFFF);             /* ���� */
}
























