/******************************************************************
** 文件名:      price.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.9.6
** 描 述:       价位结构定义的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
 * 结构体定义
 */

/*
 * BID_CS（委买）信息
 */
typedef struct {
    long long  		price;							/* 价格 */
    int     		orderNumber;					/* 当前交易时段委托笔数 */
    long long       orderAmount;					/* 当前交易时段委托数量 */

	int				tradePhaseOrderNum[4];			/* 交易时段委托笔数统计信息 0为OA，1为AU，2为SU，3为FP */
    long long		tradePhaseOrderAmount[4];		/* 交易时段委托数量统计信息*/
    /*
     * 结构指针
     */
    int     firstOrder;                             /* 第一笔委托 */
    int     lastOrder;                              /* 最后一笔委托 */

    int     prev;                                   /* 上一价位 */
    int     next;                                   /* 下一价位 */
    /* -------------------------           */

} BID_CS;


/*
 * OFFER_CS（委卖）信息
 */
typedef BID_CS  OFFER_CS;


/*
 * 函数声明
 */
BID_CS*     GetBidList();                                   /* 返回委买价位列表的起始位置 */
BID_CS*     GetStockBidList(int stockNo);                   /* 返回指定股票的委买价位列表 */
BID_CS*     GetBid(int index);                              /* 返回指定位置的BID_CS */
int         GetBidNumber();                                 /* 返回Bid总数（包含已丢弃的空闲位置） */

OFFER_CS*   GetOfferList();                                 /* 返回委卖价位列表的起始位置 */
OFFER_CS*   GetStockOfferList(int stockNo);                 /* 返回指定股票的委卖价位列表 */
OFFER_CS*   GetOffer(int index);                            /* 返回指定位置的OFFER_CS */
int         GetOfferNumber();                               /* 返回Offer总数（包含已丢弃的空闲位置） */

void        UpdateBidByOrder(ORDER_CS *order);              /* 委托加工处理时委买价位处理 */
void        UpdateOfferByOrder(ORDER_CS *order);            /* 委托加工处理时委卖价位处理 */

void 		BidStatAdd(BID_CS*, ORDER_CS*);					/* 向买信息中增加交易时段统计信息 */
void 		OfferStatAdd(OFFER_CS*, ORDER_CS*);				/* 向卖信息中增加交易时段统计信息 */
void 		BidStatDel(BID_CS*, ORDER_CS*, long long);		/* 向买信息中减少交易时段统计信息 */
void 		OfferStatDel(OFFER_CS*, ORDER_CS*, long long);	/* 向卖信息中减少交易时段统计信息 */

void        UpdateBidByWithdraw(ORDER_CS*, long long);		/* 成交、撤单加工处理时的委买价位处理 */
void        UpdateOfferByWithdraw(ORDER_CS*, long long);	/* 成交、撤单加工处理时的委卖价位处理 */

BOOL		IsHaveOpponent(ORDER_CS*);						/* 判断订单是否倒挂 *///返回true 可以撮合 否则不能

void        PrintBidInfoToFile(int fp);                     /* 打印委买价位信息到指定的文件中 */
void        PrintOfferInfoToFile(int fp);                   /* 打印委卖价位信息到指定的文件中 */
BOOL		PrintStockSnapshotToFile();						/* 打印证券快照信息到文件 */

#endif  /* _MODEL_PRICE_H */
