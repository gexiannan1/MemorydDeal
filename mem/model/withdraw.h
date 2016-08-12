/******************************************************************
** �ļ���:      withdraw.h
** �� ��:       0.1
** ������:      zhaodj
** �� ��:       2005.9.6
** �� ��:       WITHDRAW��Ϣ�����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/withdraw.h,v 1.1 2012/03/29 01:19:55 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:55 $
 */

#ifndef _MODEL_WITHDRAW_H
#define _MODEL_WITHDRAW_H


#include    "model_te.h"
#include    "../util/symbol.h"


/*
 * �ṹ������
 */

/*
 * WITHDRAW��Ϣ
 */
typedef struct {
    /*
     * ��TEϵͳ�������Ϣ
     * Ϊ���Ͻṹ�����ֱ��, δֱ������ WITHDRAW_TE �ṹ
     * <b>
     * �� WITHDRAW_TE �ṹ�����ı�, ��Ҫͬ���޸ĸ�����
     * </b>
     */
    unsigned long long			orderNo;									/* ������ */
    unsigned long long			orderNoOld;									/* �ɶ����� */
    unsigned long long			orderNoOld1;								/* �ɶ�����(����ר��) */
    int							orderProcessNo;								/* ��������� */
    int							orderProcessNoOld;							/* �ɶ�������� */
    int							orderProcessNoOld1;							/*  */
    int				            orderTime;									/* ��������ʱ�� */
    char						isinCod[STOCK_ISIN_CODE_LENGTH];			/* ��Ʊ����֤ȯ��ʶ��*/
    char						pbuId[PBU_FIRM_LENGTH];				/* pbu�� */
    char						ordrBuyCod;									/* ������־ - BUY �� SELL */
    char						ordrTypCod;									/* �������� */
    long long					price;										/* �۸� */
    long long					qty;										/* ���� */
    char						trdResTypCod[3];							/* ������������ */
    long long					remPeakQty;									/* ʣ����߽�ʾ���� */
    char						orderStatus;								/* ����״̬ */
    long long					peakSizeQty;								/* ����ʾ���� */
    char						trnTypId;									/* �������� */
    int							withdrawProcessNo;							/* ��������� */
    
    
    /* -------------------------           */

    /*
     * �ṹָ��
     */
    int     nextStock;                                  /* ͬ��Ʊ��һ�ʳɽ� */
    
    int		stockNo;									/* ��Ʊ��¼�ţ�stock����� */
    /* -------------------------           */

} WITHDRAW_CS;


/*
 * ��������
 */
WITHDRAW_CS*    GetWithdrawList();                      /* ����Withdraw�������ʼλ�� */
WITHDRAW_CS*    GetWithdraw(int index);                 /* ����ָ��λ�õ�WITHDRAW_CS */
int             GetWithdrawNumber();                    /* ����Withdraw���� */
int             GetLastWithdrawTime();                  /* �������һ�ʳ���ʱ�� */
int             AddWithdraw(ORDER_CHANGE_TE*);          /* ��Withdraw����������Withdraw��Ŀ */

int             GetWorkedWithdrawNumber();              /* ���ص�ǰ�Ѵ�����ĳ����� */
void            SetWorkedWithdrawNumber(int);           /* ���õ�ǰ�Ѵ�����ĳ����� */
void            IncreaseWorkedWithdrawNumber();         /* ������ǰ�Ѵ�����ĳ����� */

BOOL			CancelOrder(WITHDRAW_CS*);				/* ���ݼӹ�-ȡ������ */
BOOL			ModifyOrder(WITHDRAW_CS*);				/* ���ݼӹ�-�޸Ķ��� */
BOOL			DeleteOrder(WITHDRAW_CS*);				/* ���ݼӹ�-ɾ������ */
void			UpdateStockSort(WITHDRAW_CS*);			/* ����Stock�������� */
BOOL			IsBuyWithdraw(WITHDRAW_CS*);			/* ����ָ����ί���Ƿ�������ί�� */
BOOL			IsSellWithdraw(WITHDRAW_CS*);			/* ����ָ����ί���Ƿ�������ί�� */

BOOL			PrintWithdrawToFile();
void            PrintWithdrawInfoToFile(int);			/* ��ӡ������Ϣ��ָ�����ļ��� */
BOOL			PrintStockToFile();
#endif  /* _MODEL_WITHDRAW_H */
