/******************************************************************
** �ļ���:      order.h
** �� ��:       0.1
** ������:      zhaodj
** �� ��:       2005.9.6
** �� ��:       ORDER��Ϣ�����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/order.h,v 1.1 2012/03/29 01:19:53 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:53 $
 */

#ifndef _MODEL_ORDER_H
#define _MODEL_ORDER_H

#include	"stock.h"
#include    "../util/symbol.h"


/*
 * �ṹ�嶨��
 */

/*
 * ORDER��Ϣ
 */
typedef struct {
    /*
     * ��TEϵͳ�������Ϣ
     * Ϊ���Ͻṹ�����ֱ��, δֱ������ ORDER_TE �ṹ
     * <b>
     * �� ORDER_TE �ṹ�����ı�, ��Ҫͬ���޸ĸ�����
     * </b>
     */
    unsigned long long		orderNo;                                    /* ������ */
    unsigned long long		orderNoOld;									/* �ɶ����� */
    unsigned long long		orderNoOld1;								/* �ɶ����� */
    int						orderProcessNo;								/* �������к� */
    int						orderProcessNoOld;							/* �ɶ������к� */
    int						orderProcessNoOld1;							/* �ɶ������к� */
    int			            orderTime;                                  /* ��������ʱ�� */
    char					isinCod[STOCK_ISIN_CODE_LENGTH];			/* ��Ʊ����֤ȯ��ʶ��*/
    char    				pbuId[PBU_FIRM_LENGTH];           		/* pbu�� */
    char    				orderBSType;                            	/* ������־ - BUY �� SELL */
    char    				orderType;                       			/* �������� */
    long long  				orderPrice;                             	/* �۸� */
    long long  				orderQty;                               	/* ���� */
    char					trdResTypCod[3];							/* ������������ */
    char					trnTypId;									/* �������� */
    char					orderStatus; 								/* ����״̬ */
    long long				remPeakQty;									/* ʣ���ʾ���� */
    long long				peakSizeQty;								/* ����ʾ���� */
    char					ordrResCod;									/* ������������ */
    /* -------------------------           */

    long long	balance;                                    /* ���۳��ѳɽ����ѳ������ʣ��δ��������������ӦΪ0�� */
    char    	status;                                     /* ״̬ - OPEN, MATCHED; OPEN �ĲŲ����� */

    /*
     * �ṹָ��
     */
    int			stockNo;                            		/* ��Ʊ��¼�ţ�stock����� */
            	
    int     	nextStock;                                  /* ͬ��Ʊ��һ��ί�� */
            	
    int     	priceLevel;                                 /* ��ί�м�λ�ڼ�λ�б��е�λ�� */
            	
    int     	prevPrice;                                  /* ͬ��λ��һ��ί�� */
    int     	nextPrice;                                  /* ͬ��λ��һ��ί�� */
    
    /* -------------------------           */           	

} ORDER_CS;


/*
 * ��������
 */
ORDER_CS*   GetOrderList();                             /* ����Order�������ʼλ�� */
ORDER_CS*   GetOrder(int index);                        /* ����ָ��λ�õ�ORDER_CS */
int         GetOrderNumber();                           /* ����Order���� */
//int         AddOrder(ORDER_TE*);                        /* ��Order����������Order��Ŀ */
int         PushOrder(ORDER_CS*);                        /* ���յ��Ķ������������ڴ� */
BOOL 		DetachOrder(ORDER_CS*);						/* �Ӷ�������ɾ��order��Ŀ����order�����������룩 */
BOOL		AttachOrder(ORDER_CS*);						/* ��order��Ŀ��ӵ��������� */
void		UpdateStockDel(STOCK_CS*, ORDER_CS*);		/* ����֤ȯ��Ϣ��ȥ����Ӧ������ͳ�� */
void		UpdateStockAdd(STOCK_CS*, ORDER_CS*);		/* ����֤ȯ��Ϣ�����Ӷ�Ӧ������ͳ�� */

BOOL		IsIceOrder(ORDER_CS*);						/* �ж϶����Ƿ�Ϊ��ɽ���� */

void		StockStatAdd(ORDER_CS*);					/* �Ѷ�����Ϣ��ӵ�����ʱ��ͳ����Ϣ */
void		StockStatDel(ORDER_CS*, long long);			/* �ӽ���ʱ��ͳ��ɾ��Order��Ϣ */

int         GetWorkedOrderNumber();                     /* ���ص�ǰ�Ѵ������ί���� */
void        SetWorkedOrderNumber(int);                  /* ���õ�ǰ�Ѵ������ί���� */
void        IncreaseWorkedOrderNumber();                /* ������ǰ�Ѵ������ί���� */

BOOL        IsBuyOrder(ORDER_CS*);                      /* ����ָ����ί���Ƿ�������ί�� */
BOOL        IsSellOrder(ORDER_CS*);                     /* ����ָ����ί���Ƿ�������ί�� */

BOOL        IsTradingPhase(ORDER_CS*);					/* ����Order�Ƿ�μӲ�Ʒ����ʱ�� */

BOOL        IsOrderOutmoded(ORDER_CS*);                 /* ����ί���Ƿ��ѹ�ʱ������ȫ���ɽ��򳷵���*/
BOOL        IsOrderProcessComplete();                   /* ����ί���Ƿ���ȫ��������� */

void        PrintOrderInfoToFile(int fp);               /* ��ӡί����Ϣ��ָ�����ļ��� */
void		PrintOrderCS(ORDER_CS*);

#endif  /* _MODEL_ORDER_H */
