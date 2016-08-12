/******************************************************************
** �ļ���:      price.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.9.6
** �� ��:       ��λ�ṹ�����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/price.h,v 1.1 2012/03/29 01:19:53 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:53 $
 */

#ifndef _MODEL_PRICE_H
#define _MODEL_PRICE_H


#include    "./order.h"
#include    "./trade.h"
#include    "./withdraw.h"
#include    "../util/symbol.h"


/*
 * �ṹ�嶨��
 */

/*
 * BID_CS��ί����Ϣ
 */
typedef struct {
    long long  		price;							/* �۸� */
    int     		orderNumber;					/* ��ǰ����ʱ��ί�б��� */
    long long       orderAmount;					/* ��ǰ����ʱ��ί������ */

	int				tradePhaseOrderNum[4];			/* ����ʱ��ί�б���ͳ����Ϣ 0ΪOA��1ΪAU��2ΪSU��3ΪFP */
    long long		tradePhaseOrderAmount[4];		/* ����ʱ��ί������ͳ����Ϣ*/
    /*
     * �ṹָ��
     */
    int     firstOrder;                             /* ��һ��ί�� */
    int     lastOrder;                              /* ���һ��ί�� */

    int     prev;                                   /* ��һ��λ */
    int     next;                                   /* ��һ��λ */
    /* -------------------------           */

} BID_CS;


/*
 * OFFER_CS��ί������Ϣ
 */
typedef BID_CS  OFFER_CS;


/*
 * ��������
 */
BID_CS*     GetBidList();                                   /* ����ί���λ�б����ʼλ�� */
BID_CS*     GetStockBidList(int stockNo);                   /* ����ָ����Ʊ��ί���λ�б� */
BID_CS*     GetBid(int index);                              /* ����ָ��λ�õ�BID_CS */
int         GetBidNumber();                                 /* ����Bid�����������Ѷ����Ŀ���λ�ã� */

OFFER_CS*   GetOfferList();                                 /* ����ί����λ�б����ʼλ�� */
OFFER_CS*   GetStockOfferList(int stockNo);                 /* ����ָ����Ʊ��ί����λ�б� */
OFFER_CS*   GetOffer(int index);                            /* ����ָ��λ�õ�OFFER_CS */
int         GetOfferNumber();                               /* ����Offer�����������Ѷ����Ŀ���λ�ã� */

void        UpdateBidByOrder(ORDER_CS *order);              /* ί�мӹ�����ʱί���λ���� */
void        UpdateOfferByOrder(ORDER_CS *order);            /* ί�мӹ�����ʱί����λ���� */

void 		BidStatAdd(BID_CS*, ORDER_CS*);					/* ������Ϣ�����ӽ���ʱ��ͳ����Ϣ */
void 		OfferStatAdd(OFFER_CS*, ORDER_CS*);				/* ������Ϣ�����ӽ���ʱ��ͳ����Ϣ */
void 		BidStatDel(BID_CS*, ORDER_CS*, long long);		/* ������Ϣ�м��ٽ���ʱ��ͳ����Ϣ */
void 		OfferStatDel(OFFER_CS*, ORDER_CS*, long long);	/* ������Ϣ�м��ٽ���ʱ��ͳ����Ϣ */

void        UpdateBidByWithdraw(ORDER_CS*, long long);		/* �ɽ��������ӹ�����ʱ��ί���λ���� */
void        UpdateOfferByWithdraw(ORDER_CS*, long long);	/* �ɽ��������ӹ�����ʱ��ί����λ���� */

BOOL		IsHaveOpponent(ORDER_CS*);						/* �ж϶����Ƿ񵹹� *///����true ���Դ�� ������

void        PrintBidInfoToFile(int fp);                     /* ��ӡί���λ��Ϣ��ָ�����ļ��� */
void        PrintOfferInfoToFile(int fp);                   /* ��ӡί����λ��Ϣ��ָ�����ļ��� */
BOOL		PrintStockSnapshotToFile();						/* ��ӡ֤ȯ������Ϣ���ļ� */

#endif  /* _MODEL_PRICE_H */
