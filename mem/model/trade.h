/******************************************************************
** �ļ���:      trade.h
** �� ��:       0.1
** ������:      zhaodj
** �� ��:       2005.9.6
** �� ��:       TRADE��Ϣ�����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
** 2006.03.23   Wei Huanyin 1��IsValidTrade��������ֵ��char��Ϊint
							2��VALID_TRADE ֵ��Ϊ 0 INVALID_TRADEֵ��Ϊ1
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/trade.h,v 1.1 2012/03/29 01:19:55 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:55 $
 */

#ifndef _MODEL_TRADE_H
#define _MODEL_TRADE_H


#include    "model_te.h"
#include    "../util/symbol.h"


/*
 *
 */
#define VALID_TRADE         0         /* ��ʾ���ݷ������� */
#define INVALID_TRADE       1         /* ��ʾ���ݲ��������� */

#define TRADE_TYPE_NORMAL_TRD               'X'

/*
 * �ṹ�嶨��
 */

/*
 * TRADE��Ϣ
 */
typedef struct {
    /*
     * ��TEϵͳ�������Ϣ
     * Ϊ���Ͻṹ�����ֱ��, δֱ������ TRADE_TE �ṹ
     * <b>
     * �� TRADE_TE �ṹ�����ı�, ��Ҫͬ���޸ĸ�����
     * </b>
     */
    unsigned long long			tradeNo;                                    /* ���׺� */
    int				            tradeTime;                                  /* ���׷���ʱ�䣬hhmmss */
    int							tradeProcessNo;								/* �ɽ������ */
    char						isinCod[STOCK_ISIN_CODE_LENGTH];			/* ��Ʊ����֤ȯ��ʶ��*/
    unsigned long long			orderNo;                                    /* ���׶�Ӧ�Ķ����� */
    char						orderType;									/* ���׶�Ӧ�Ķ��������� */
    char    					pbuId[PBU_FIRM_LENGTH];           			/* pbu�� */
    int							orderProcessNo;								/* �ɽ���Ӧ�Ķ�������� */
    long long					price;                                      /* �۸� */
    long long					qty;                                        /* ���� */
    long long					mktVal;										/* �ɽ���� */
    char						type;                                       /* �������� */
    char						trnTypId;									/* �������� */
    char						tradeBSType;								/* ������������ */
    char						tradeBSFlag;								/* �����̱�� */
    long long					remPeakQty;									/* ʣ����߽�ʾ���� */
    long long					peakSizeQty;								/* ����ʾ���� */
    unsigned long				ordrEntTim;									/* ��ɽ����������¶ʱ�� */
    /* -------------------------           */

    int     index;                                     /* �ɽ���ţ���0��ʼ�� */
	
	int		stockNo;									/* ��Ʊ��¼�ţ�stock����� */
    /*
     * �ṹָ��
     */
    int     nextStock;                                  /* ͬ��Ʊ��һ�ʳɽ� */
    /* -------------------------           */

} TRADE_CS;


/*
 * ��������
 */
TRADE_CS*   GetTradeList();                             /* ����Trade�������ʼλ�� */
TRADE_CS*   GetTrade(int index);                        /* ����ָ��λ�õ�TRADE_CS */
int         GetTradeNumber();                           /* ����Trade���� */
TRADE_CS*   GetLastTrade();                             /* �������һ�ʳɽ� */
int         AddTrade(TRADE_TE*);                        /* ��Trade����������Trade��Ŀ */

BOOL		IsBuyTrade(TRADE_CS*);						/* �����Ƿ��������ͳɽ� */
BOOL		IsSellTrade(TRADE_CS*);						/* �����Ƿ��������ͳɽ� */
BOOL		IsMarketPriceTrade(TRADE_CS*);				/* �����Ƿ����м�ί�гɽ� */

int         GetWorkedTradeNumber();                     /* ���ص�ǰ�Ѵ�����ĳɽ����� */
void        SetWorkedTradeNumber(int);                  /* ���õ�ǰ�Ѵ�����ĳɽ����� */
void        IncreaseWorkedTradeNumber();                /* ������ǰ�Ѵ�����ĳɽ����� */

int         GetSentTradeNumber();                       /* ���ص�ǰ�ѷ��͵ĳɽ����� */
void        SetSentTradeNumber(int);                    /* ���õ�ǰ�ѷ��͵ĳɽ����� */
void        IncreaseSentTradeNumber();                  /* ������ǰ�ѷ��͵ĳɽ����� */

BOOL        IsPendingTrade(TRADE_CS*);                  /* ����ָ���ĳɽ��Ƿ���Ҫ�ӹ� */
int        	IsValidTrade(TRADE_CS*);                    /* �ж���ʳɽ��Ƿ��Ƿ��Ϸ������������� */

void        PrintTradeInfoToFile(int fp);               /* ��ӡ�ɽ���Ϣ��ָ�����ļ��� */


#endif  /* _MODEL_TRADE_H */
