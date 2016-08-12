/******************************************************************
** �ļ���:      stock_pbu.h
** �� ��:       0.1
** ������:      Li Bo
**              
** �� ��:       2006.12.11
** �� ��:       Stock�е�PBU��Ϣ�����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/stock_pbu.h,v 1.1 2012/03/29 01:19:54 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:54 $
 */


#ifndef _STOCK_PBU_H
#define _STOCK_PBU_H

#include    "model_length_define.h"
#include    "order.h"
#include	"trade.h"
#include	"stock.h"
#include    "../util/symbol.h"

/*
 * ��������
 */
#define				PBU_ORDER_STAT_ADD			0
#define				PBU_ORDER_STAT_DEL			1

#define				PBU_ORDER_SORT_BUY			0					/* pbu������� �� */
#define				PBU_ORDER_SORT_SELL			1					/* pbu������� ��*/


/*
 * �ṹ�嶨��
 */

typedef struct {
	char			pbuFirm[PBU_FIRM_LENGTH];						/* pbu���� */
} PBU_FIRM;                                                 		
                                                            		
//pbu��Ϣ                                                   		
typedef struct {                                            		
	int				pbuDataAdr;										/* ָ����pbu��Ӧ������λ�� */
	int				pbuBuyOrderSortNo;								/* ָ����pbu����ί������(��0��ʼ����ʼ��ʱΪ-1) */
	int				pbuSellOrderSortNo;								/* ָ����pbu����ί������(��0��ʼ����ʼ��ʱΪ-1) */
} PBU_HEAD;                                             			
                                                            		
                                                            		
                                                            		
//pbu����                                               			
typedef struct {                                        			
	long long		orderBuyAmount;									/* PBU��Ӧ֤ȯ����ί�е�ǰ����ʱ������ */
	long long		orderBuyMoney;									/* PBU��Ӧ֤ȯ����ί�е�ǰ����ʱ�ν�� */
	long long		orderOABuyAmount;								/* PBU��Ӧ֤ȯ����ί��OA����ʱ������ */
	long long		orderOABuyMoney;								/* PBU��Ӧ֤ȯ����ί��OA����ʱ�ν�� */
	long long		orderAUBuyAmount;								/* PBU��Ӧ֤ȯ����ί��AU����ʱ������ */
	long long		orderAUBuyMoney;								/* PBU��Ӧ֤ȯ����ί��AU����ʱ�ν�� */
	long long		orderSUBuyAmount;								/* PBU��Ӧ֤ȯ����ί��SU����ʱ������ */
	long long		orderSUBuyMoney;								/* PBU��Ӧ֤ȯ����ί��SU����ʱ�ν�� */
	long long		orderFPBuyAmount;								/* PBU��Ӧ֤ȯ����ί��FP����ʱ������ */
	long long		orderFPBuyMoney;								/* PBU��Ӧ֤ȯ����ί��FP����ʱ�ν�� */
	                                                    			
	long long		orderSellAmount;								/* PBU��Ӧ֤ȯ����ί�е�ǰ����ʱ������ */
	long long		orderSellMoney;									/* PBU��Ӧ֤ȯ����ί�е�ǰ����ʱ�ν�� */
	long long		orderOASellAmount;								/* PBU��Ӧ֤ȯ����ί��OA����ʱ������ */
	long long		orderOASellMoney;								/* PBU��Ӧ֤ȯ����ί��OA����ʱ�ν�� */
	long long		orderAUSellAmount;								/* PBU��Ӧ֤ȯ����ί��AU����ʱ������ */
	long long		orderAUSellMoney;								/* PBU��Ӧ֤ȯ����ί��AU����ʱ�ν�� */
	long long		orderSUSellAmount;								/* PBU��Ӧ֤ȯ����ί��SU����ʱ������ */
	long long		orderSUSellMoney;								/* PBU��Ӧ֤ȯ����ί��SU����ʱ�ν�� */
	long long		orderFPSellAmount;								/* PBU��Ӧ֤ȯ����ί��FP����ʱ������ */
	long long		orderFPSellMoney;								/* PBU��Ӧ֤ȯ����ί��FP����ʱ�ν�� */
	                                                        		
	long long		tradeBuyAmount;									/* PBU��Ӧ֤ȯ����ɽ����� */
	long long		tradeBuyMoney;									/* PBU��Ӧ֤ȯ����ɽ���� */
	long long		tradeSellAmount;								/* PBU��Ӧ֤ȯ�����ɽ����� */
	long long		tradeSellMoney;									/* PBU��Ӧ֤ȯ�����ɽ���� */
} PBU_DATA;                                                 		
                                                            		
                                                            		
/*                                                          		
 * ��������                                                 		
 */                                                         		
void			InitPBU();											/* ��ʼ��PBU�ڴ��� */
                                                            		
PBU_FIRM*		GetPBUFirm();										/* ����PBU�ŵ���ʼλ�� */
PBU_FIRM*		GetPBUFirmByPbuNo(int);								/* ����pbu��λ�û��pbu����Ϣ */
                                                            		
int				GetPBUNoByFirm(char* firm);							/* ����pbu�����ô�pbu��pbu������е�λ�� */
int				GetPBUNumber();										/* ��õ�ǰ�Ѿ��洢��pbu�������� */
int				AddFirm(char*);										/* ���pbuϯλ�ŵ������ڴ� */
PBU_DATA*		AddNewPBUData(PBU_HEAD*);							/* ����µ�pbu���� */

PBU_HEAD*		GetPBUTableByStockNo(int, int);						/* ����stockNo��pbu�ں�����λ�÷��ض�Ӧ��pbu��Ϣ��ַ */
int*			GetPBUBuySortByStockNo(int, int);					/* ����stockNo���������ض�Ӧ��PBU��ί��������ַ */
int*			GetPBUSellSortByStockNo(int, int);					/* ����stockNo���������ض�Ӧ��PBU��ί��������ַ */

void			PBUStatusChange(STOCK_CS*);							/* ����ʱ�α任���� */
void			ResortPBUSortList(STOCK_CS*);						/* PBU������������ */
                                                            		
PBU_DATA*		GetPBUDataByIndex(int index);						/* ���ݵ�ַ�Ż�ȡ��Ӧ��PBU���ݵ�ַ */
int				GetPBUDataNumber();									/* ��ȡ��ǰ�Ѿ��洢��PBU�������� */

void			UpdatePBUTradeStat(PBU_DATA*, TRADE_CS*);			/* ά��pbu�ɽ�ͳ�� */

void			PBUOrderStatAdd(STOCK_CS*,PBU_DATA*, ORDER_CS*);				/* pbu����ʱ��ί��ͳ�ƣ����ӣ� */
void			PBUOrderStatDel(STOCK_CS*,PBU_DATA*, ORDER_CS*, long long);		/* pbu����ʱ��ί��ͳ�ƣ����٣� */

void			UpdatePBUOrderSortList(STOCK_CS*, int, ORDER_CS*);	/* ά��pbuί������ */
void			UpdatePBUTradeSortList(STOCK_CS*, int, TRADE_CS*);	/* ά��pbu�ɽ����� */

void			PrintPBUList();										/* ���pbu�����ͳɽ��������ļ��� */

#endif  /* _STOCK_PBU_H */
