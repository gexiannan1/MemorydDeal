/******************************************************************
** �ļ���:      stock_index.h
** �� ��:       0.1
** ������:      Li Bo
** �� ��:       2006.8.30
** �� ��:       ��Ʊָ�� ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/stock_index.h,v 1.1 2012/03/29 01:19:54 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:54 $
 */

#ifndef _STOCK_INDEX_H
#define _STOCK_INDEX_H

#include    "model_length_define.h"
#include	"stock.h"
#include    "../util/symbol.h"


/*
 * �ṹ�嶨��
 */

/*
 * ����ָ��������ṹ
 */
typedef	struct{
	DETAILED_INDEX	detailedIndex;										/* ����ָ������ */
	char			indexCode[STOCK_ID_LENGTH];							/* ��Ʊָ��������� */
} STOCK_INDEX_CS;


/*
 * ��������
 */
STOCK_INDEX_CS*		GetStockIndexHead();								/* ��ù�Ʊָ���洢��ʼλ�� */
STOCK_INDEX_CS*		GetStockIndex(int);									/* ����ָ��λ�õ�STOCK_INDEX_CS */
STOCK_INDEX_CS*		GetStockIndexByIsinCod(char*);						/* ���ݷ�������ù�Ʊָ�� */
int                 GetStockIndexNumber();               				/* ����StockIndex���� */
int					AddStockIndexInfo(STOCK_INFO*);						/* ���ָ����Ϣ�������ڴ� */
BOOL				UpdateDetailedIndex(DETAILED_INDEX*);				/* �����ڴ��и���ָ����Ϣ */

void				PrintStockIndex(STOCK_INDEX_CS*);					/* ���ָ����Ϣ */
void				PrintStockIndexToFile(int);							/* ���ָ����Ϣ���ļ� */

#endif /* _STOCK_INDEX_H */
