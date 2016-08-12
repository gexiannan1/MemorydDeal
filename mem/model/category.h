/******************************************************************
** �ļ���:      category.h
** �� ��:       0.1
** ������:      Song Fang
**              zhaodj
** �� ��:       2005.9.9
** �� ��:       ֤ȯƷ���б���Ϣ�����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/category.h,v 1.1 2012/03/29 01:19:51 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:51 $
 */

#ifndef _MODEL_CATEGORY_H
#define _MODEL_CATEGORY_H


#include    "model.h"
#include    "model_mon.h"
#include    "../util/symbol.h"


/*
 * Ʒ�ִ��붨��
 */
#define     CATEGORY_ID_ASHARE              0       /* Ʒ�ִ���-A�� */
#define     CATEGORY_ID_BSHARE              1       /* Ʒ�ִ���-B�� */
#define     CATEGORY_ID_CLOSEDEND_FUND      2       /* Ʒ�ִ���-���ʽ���� */
#define     CATEGORY_ID_ETF                 3       /* Ʒ�ִ���-ETF */
#define     CATEGORY_ID_OPENED_FUND         4       /* Ʒ�ִ���-����ʽ���� */
#define     CATEGORY_ID_MONETARY_FUND       5       /* Ʒ�ִ���-���һ��� */
#define     CATEGORY_ID_WARRANT             6       /* Ʒ�ִ���-Ȩ֤ */
#define     CATEGORY_ID_BOND                7       /* Ʒ�ִ���-��ͨծȯ */
#define     CATEGORY_ID_CONVERTIBLE         8       /* Ʒ�ִ���-��ת��ծȯ */
#define     CATEGORY_ID_REPO                9       /* Ʒ�ִ���-�ع� */


/*
 * �ṹ�嶨��
 */

/*
 * ֤ȯƷ���б�
 */
typedef struct {
    /*
     * ����������ϵͳά������Ϣ
     * Ϊ���Ͻṹ�����ֱ��, δֱ������ CATEGORY_MON �ṹ
     * <b>
     * �� CATEGORY_MON �ṹ�����ı�, ��Ҫͬ���޸ĸ�����
     * </b>
     */
    int     	id;                                     	/* Ʒ�ִ��� */
    char    	name[CATEGORY_NAME_LENGTH];             	/* Ʒ������ */
    
    char		subType[CATEGORY_SUB_TYPE_NUMBER][CATEGORY_SUB_TYPE_LENGTH];/* �����Ĳ�Ʒ������ */
    /* -------------------------           */

    int     	securitiesNumber;                       	/* ���ڸ�ȯ�ֵ�֤ȯ���� */
    long long   tradeAmount;                            	/* �ܳɽ����� */
    long long   tradeMoney;                             	/* �ܳɽ���� */	

    /*
     * �ṹָ��
     */
    int     	firstStock;                             	/* ��ȯ�ֵĵ�һֻ��Ʊ */
    int     	lastStock;                              	/* ��ȯ�ֵ����һֻ��Ʊ */
    /* -------------------------           */

} CATEGORY_CS;


/*
 * ��������
 */
CATEGORY_CS*    GetCategoryList();                      /* ����Category�������ʼλ�� */
CATEGORY_CS*    GetCategory(int index);                 /* ����ָ��λ�õ�CATEGORY_CS */
CATEGORY_CS*    GetCategoryById(int id);                /* ����Ʒ�ִ����Ӧ��CATEGORY_CS */
int				GetStockFromSubType(char* pSubType);	/* ͨ����Ʒ�����ͻ�ò�Ʒ���� */
int             GetCategoryNumber();                    /* ����Category���� */
int             AddCategory(CATEGORY_CS*);              /* ��Category����������Category��Ŀ */


int             AddCategoryStock(int, STOCK_CS*);       /* ��ָ��ȯ�����������֤ȯ��Ϣ */

void            PrintCategoryInfoToFile(int fp);        /* ��ӡȯ����Ϣ��ָ�����ļ��� */


#endif  /* _MODEL_CATEGORY_H */

