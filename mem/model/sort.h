/******************************************************************
** �ļ���:      sort.h
** �� ��:       0.1
** ������:      Song Fang
**              zhaodj
** �� ��:       2005.9.6
** �� ��:       ί�������ȹ�Ʊ����������Ϣ��ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/sort.h,v 1.1 2012/03/29 01:19:53 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:53 $
 */

#ifndef _MODEL_SORT_H
#define _MODEL_SORT_H


#include    "model_length_define.h"
#include    "stock.h"


/*
 * �ṹ�嶨��
 */

/*
 * ����ί�����볷������������ί����������10��ί�򳷵���
 */
typedef struct {
    int     withdrawNo[WITHDRAW_SORT_LENGTH];
} WITHDRAW_BUY_SORT_LIST;

/*
 * ����ί��������������������ί����������10��ί��������
 */
typedef WITHDRAW_BUY_SORT_LIST          WITHDRAW_SELL_SORT_LIST;
/* -------------------------           */


/*
 * ��������
 */
WITHDRAW_BUY_SORT_LIST*     GetAShareWithdrawBuySortList();				/* ���ص���ί�����볷������ */
WITHDRAW_SELL_SORT_LIST*    GetAShareWithdrawSellSortList();			/* ���ص���ί�������������� */
WITHDRAW_BUY_SORT_LIST*     GetBShareWithdrawBuySortList();				/* ���ص���ί�����볷������ */
WITHDRAW_SELL_SORT_LIST*    GetBShareWithdrawSellSortList();			/* ���ص���ί�������������� */

STOCK_CS**  GetShareOrderBuyRanking(int shareType);						/* �����Ĺ�Ʊί�����������б� */
STOCK_CS**  GetShareOrderSellRanking(int shareType);					/* �����Ĺ�Ʊί�����������б� */
STOCK_CS**  GetShareWithdrawBuyRanking(int shareType);					/* �����Ĺ�Ʊ�ۼ�ί�����볷������ */
STOCK_CS**  GetShareWithdrawSellRanking(int shareType);					/* �����Ĺ�Ʊ�ۼ�ί������������� */

void        PrintShareOrderBuySortListToFile(int fp,int shareType);		/* ��ӡ��Ʊί������������Ϣ��ָ�����ļ��� */
void        PrintShareOrderSellSortListToFile(int fp,int shareType);	/* ��ӡ��Ʊί������������Ϣ��ָ�����ļ��� */
void        PrintWithdrawBuySortListToFile(int fp,int shareType);		/* ��ӡ���ʳ���������Ϣ��ָ�����ļ��� */
void        PrintWithdrawSellSortListToFile(int fp,int shareType);		/* ��ӡ���ʳ���������Ϣ��ָ�����ļ��� */
void        PrintShareWithdrawBuySortListToFile(int fp,int shareType);	/* ��ӡ��Ʊ�ۼƳ���������Ϣ��ָ�����ļ��� */
void        PrintShareWithdrawSellSortListToFile(int fp,int shareType);	/* ��ӡ��Ʊ�ۼƳ���������Ϣ��ָ�����ļ��� */


#endif  /* _MODEL_SORT_H */
