/******************************************************************
** 文件名:      model_length_define.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.10.29
** 描 述:       Model常量定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
 * CS数据长度定义
 * (为避免数据空隙，所有长度定义为4的倍数)
 */
#define STOCK_ID_LENGTH                     8               /* 股票代码长度 */
#define STOCK_ISIN_CODE_LENGTH				13				/* 股票国际证券标识码 */
#define STOCK_SHORTNAME_LENGTH              12              /* 股票名称长度 */
#define PBU_FIRM_LENGTH                   	8               /* pbu号长度 */
#define CATEGORY_NAME_LENGTH                32              /* 品种名称长度 */
#define CATEGORY_SUB_TYPE_NUMBER            256             /* 品种子类型最大数量 */
#define CATEGORY_SUB_TYPE_LENGTH            4               /* 品种子类型长度 */
#define INDUSTRY_ID_LENGTH                  4               /* 行业代码长度 */
#define INDUSTRY_NAME_LENGTH                64              /* 行业名称长度 */
#define	CURRENCY_TYPE_LENGTH				4				/* 货币类型 */


/*
 * 排名队列长度定义
 */
#define SHARE_ORDER_SORT_LENGTH             10              /* 股票委托排名长度 */
#define WITHDRAW_SORT_LENGTH                10              /* 单笔撤单排名长度 */
#define SHARE_WITHDRAW_SORT_LENGTH          10              /* 股票累计撤单排名长度 */
#define INDUSTRY_STOCK_TRADE_SORT_LENGTH    5               /* 行业个股成交金额排名长度 */
#define	PBU_TRADE_SORT_LENGTH				5				/* pbu 成交排名长度 */

#define		SET_DATA_MAX_LEN				130
#define		COMMON_DATA_MAX_LEN				210

/*
 * 精度
 */
#define TEN_THOUSAND						10000
#define ONE_HUNDRAD							100

#endif  /* _MODEL_LENGTH_DEFINE_H */
