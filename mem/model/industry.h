/******************************************************************
** �ļ���:      industry.h
** �� ��:       0.1
** ������:      Song Fang
**              zhaodj
** �� ��:       2005.9.9
** �� ��:       ��ҵ�б���Ϣ�����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
** 2006.1.4     Song Fang   ���ӷ��ص���ģʽ�ĳɽ���Ϣ�Ľӿ�
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/industry.h,v 1.1 2012/03/29 01:19:52 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:52 $
 */

#ifndef _MODEL_INDUSTRY_H
#define _MODEL_INDUSTRY_H


#include    "model.h"
#include    "model_mon.h"
#include    "../util/symbol.h"



/*
 * �ṹ�嶨��
 */

/*
 * ��ҵ���ɳɽ���������б���ҵ�ɽ����ǰ��λ�ĸ��ɣ�
 */
typedef struct {
    int     stockNo[INDUSTRY_STOCK_TRADE_SORT_LENGTH];
} INDUSTRY_SHARE_TRADE_SORT_LIST;
/* -------------------------           */


/*
 * ��ҵ�б�
 */
typedef struct {
    /*
     * ����������ϵͳά������Ϣ
     * Ϊ���Ͻṹ�����ֱ��, δֱ������ INDUSTRY_MON �ṹ
     * <b>
     * �� INDUSTRY_MON �ṹ�����ı�, ��Ҫͬ���޸ĸ�����
     * </b>
     */
    int                             industryNo;                         /* ��ҵ��� */
    char                            industryId[INDUSTRY_ID_LENGTH];     /* ��ҵ���� */
    char                            industryName[INDUSTRY_NAME_LENGTH]; /* ��ҵ���� */
    /* -------------------------           */

    int                             tradeNumber;                        /* ����ҵ�ܳɽ����� */
    long long                       tradeAmount;                        /* ����ҵ�ܳɽ����� */
    long long                       tradeMoney;                         /* ����ҵ�ܳɽ���� */
    INDUSTRY_SHARE_TRADE_SORT_LIST  tradeSortList;                      /* ��ҵ���ɳɽ���������б���ҵ�ɽ����ǰ��λ�ĸ��ɣ� */

} INDUSTRY_CS;


/*
 * ��������
 */
INDUSTRY_CS*    GetIndustryList(int shareType);                 /* ����Industry�������ʼλ�� */
INDUSTRY_CS*    GetIndustry(int index,int shareType);           /* ����ָ��λ�õ�INDUSTRY_CS */
int             GetIndustryNumber(int shareType);               /* ����Industry���� */
int             AddIndustry(INDUSTRY_MON*,int shareType);                     /* ��Industry����������Industry��Ŀ */

INDUSTRY_SHARE_TRADE_SORT_LIST*
GetIndustryShareTradeSortList(int index,int shareType);                       /* ������ҵ���ɳɽ���������б� */


void            InitSortedIndustryListA();          			/* ��ʼ��������Industry���� */
void            InitSortedIndustryListB();          			/* ��ʼ��������Industry���� */
INDUSTRY_CS*    GetSortedIndustry(int index,int shareType);     /* ����������ָ��λ�õ�INDUSTRY_CS */

int             AddIndustryStock(char*, STOCK_CS*,int);             /* ��ָ����ҵ���������֤ȯ��Ϣ */

void            PrintIndustryInfoToFile(int fp,int shareType);                  				/* ��ӡ��ҵ��Ϣ��ָ�����ļ��� */
void            PrintSortedIndustryInfoToFile(int fp,int shareType);            /* ��ӡ��������ҵ��Ϣ��ָ�����ļ��� */


#endif  /* _MODEL_INDUSTRY_H */
