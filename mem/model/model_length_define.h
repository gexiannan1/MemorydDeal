/******************************************************************
** �ļ���:      model_length_define.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.10.29
** �� ��:       Model��������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/model_length_define.h,v 1.1 2012/03/29 01:19:52 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:52 $
 */

#ifndef _MODEL_LENGTH_DEFINE_H
#define _MODEL_LENGTH_DEFINE_H


/*
 * CS���ݳ��ȶ���
 * (Ϊ�������ݿ�϶�����г��ȶ���Ϊ4�ı���)
 */
#define STOCK_ID_LENGTH                     8               /* ��Ʊ���볤�� */
#define STOCK_ISIN_CODE_LENGTH				13				/* ��Ʊ����֤ȯ��ʶ�� */
#define STOCK_SHORTNAME_LENGTH              12              /* ��Ʊ���Ƴ��� */
#define PBU_FIRM_LENGTH                   	8               /* pbu�ų��� */
#define CATEGORY_NAME_LENGTH                32              /* Ʒ�����Ƴ��� */
#define CATEGORY_SUB_TYPE_NUMBER            256             /* Ʒ��������������� */
#define CATEGORY_SUB_TYPE_LENGTH            4               /* Ʒ�������ͳ��� */
#define INDUSTRY_ID_LENGTH                  4               /* ��ҵ���볤�� */
#define INDUSTRY_NAME_LENGTH                64              /* ��ҵ���Ƴ��� */
#define	CURRENCY_TYPE_LENGTH				4				/* �������� */


/*
 * �������г��ȶ���
 */
#define SHARE_ORDER_SORT_LENGTH             10              /* ��Ʊί���������� */
#define WITHDRAW_SORT_LENGTH                10              /* ���ʳ����������� */
#define SHARE_WITHDRAW_SORT_LENGTH          10              /* ��Ʊ�ۼƳ����������� */
#define INDUSTRY_STOCK_TRADE_SORT_LENGTH    5               /* ��ҵ���ɳɽ������������ */
#define	PBU_TRADE_SORT_LENGTH				5				/* pbu �ɽ��������� */

#define		SET_DATA_MAX_LEN				130
#define		COMMON_DATA_MAX_LEN				210

/*
 * ����
 */
#define TEN_THOUSAND						10000
#define ONE_HUNDRAD							100

#endif  /* _MODEL_LENGTH_DEFINE_H */
