/******************************************************************
** 文件名:      sort.h
** 版 本:       0.1
** 创建人:      Song Fang
**              zhaodj
** 日 期:       2005.9.6
** 描 述:       委托排名等股票盘中排名信息的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
 * 结构体定义
 */

/*
 * 单笔委托买入撤单排名（单笔委托数量最大的10笔委买撤单）
 */
typedef struct {
    int     withdrawNo[WITHDRAW_SORT_LENGTH];
} WITHDRAW_BUY_SORT_LIST;

/*
 * 单笔委托卖出撤单排名（单笔委托数量最大的10笔委卖撤单）
 */
typedef WITHDRAW_BUY_SORT_LIST          WITHDRAW_SELL_SORT_LIST;
/* -------------------------           */


/*
 * 函数声明
 */
WITHDRAW_BUY_SORT_LIST*     GetAShareWithdrawBuySortList();				/* 返回单笔委托买入撤单排名 */
WITHDRAW_SELL_SORT_LIST*    GetAShareWithdrawSellSortList();			/* 返回单笔委托卖出撤单排名 */
WITHDRAW_BUY_SORT_LIST*     GetBShareWithdrawBuySortList();				/* 返回单笔委托买入撤单排名 */
WITHDRAW_SELL_SORT_LIST*    GetBShareWithdrawSellSortList();			/* 返回单笔委托卖出撤单排名 */

STOCK_CS**  GetShareOrderBuyRanking(int shareType);						/* 排序后的股票委托买入排名列表 */
STOCK_CS**  GetShareOrderSellRanking(int shareType);					/* 排序后的股票委托卖出排名列表 */
STOCK_CS**  GetShareWithdrawBuyRanking(int shareType);					/* 排序后的股票累计委托买入撤单排名 */
STOCK_CS**  GetShareWithdrawSellRanking(int shareType);					/* 排序后的股票累计委托买出撤单排名 */

void        PrintShareOrderBuySortListToFile(int fp,int shareType);		/* 打印股票委托买入排名信息到指定的文件中 */
void        PrintShareOrderSellSortListToFile(int fp,int shareType);	/* 打印股票委托卖出排名信息到指定的文件中 */
void        PrintWithdrawBuySortListToFile(int fp,int shareType);		/* 打印单笔撤单排名信息到指定的文件中 */
void        PrintWithdrawSellSortListToFile(int fp,int shareType);		/* 打印单笔撤单排名信息到指定的文件中 */
void        PrintShareWithdrawBuySortListToFile(int fp,int shareType);	/* 打印股票累计撤单排名信息到指定的文件中 */
void        PrintShareWithdrawSellSortListToFile(int fp,int shareType);	/* 打印股票累计撤单排名信息到指定的文件中 */


#endif  /* _MODEL_SORT_H */
