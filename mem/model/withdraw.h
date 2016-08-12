/******************************************************************
** 文件名:      withdraw.h
** 版 本:       0.1
** 创建人:      zhaodj
** 日 期:       2005.9.6
** 描 述:       WITHDRAW信息定义的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
 * 结构体声明
 */

/*
 * WITHDRAW信息
 */
typedef struct {
    /*
     * 从TE系统导入的信息
     * 为保障结构定义的直观, 未直接引入 WITHDRAW_TE 结构
     * <b>
     * 若 WITHDRAW_TE 结构发生改变, 需要同步修改该区段
     * </b>
     */
    unsigned long long			orderNo;									/* 订单号 */
    unsigned long long			orderNoOld;									/* 旧订单号 */
    unsigned long long			orderNoOld1;								/* 旧订单号(报价专用) */
    int							orderProcessNo;								/* 订单处理号 */
    int							orderProcessNoOld;							/* 旧订单处理号 */
    int							orderProcessNoOld1;							/*  */
    int				            orderTime;									/* 订单接收时间 */
    char						isinCod[STOCK_ISIN_CODE_LENGTH];			/* 股票国际证券标识码*/
    char						pbuId[PBU_FIRM_LENGTH];				/* pbu号 */
    char						ordrBuyCod;									/* 买卖标志 - BUY 或 SELL */
    char						ordrTypCod;									/* 订单类型 */
    long long					price;										/* 价格 */
    long long					qty;										/* 数量 */
    char						trdResTypCod[3];							/* 交易限制类型 */
    long long					remPeakQty;									/* 剩余最高揭示数量 */
    char						orderStatus;								/* 订单状态 */
    long long					peakSizeQty;								/* 最大揭示数量 */
    char						trnTypId;									/* 事务类型 */
    int							withdrawProcessNo;							/* 撤单处理号 */
    
    
    /* -------------------------           */

    /*
     * 结构指针
     */
    int     nextStock;                                  /* 同股票下一笔成交 */
    
    int		stockNo;									/* 股票记录号，stock表入口 */
    /* -------------------------           */

} WITHDRAW_CS;


/*
 * 函数声明
 */
WITHDRAW_CS*    GetWithdrawList();                      /* 返回Withdraw数组的起始位置 */
WITHDRAW_CS*    GetWithdraw(int index);                 /* 返回指定位置的WITHDRAW_CS */
int             GetWithdrawNumber();                    /* 返回Withdraw数量 */
int             GetLastWithdrawTime();                  /* 返回最后一笔撤单时间 */
int             AddWithdraw(ORDER_CHANGE_TE*);          /* 向Withdraw数组中增加Withdraw条目 */

int             GetWorkedWithdrawNumber();              /* 返回当前已处理过的撤单数 */
void            SetWorkedWithdrawNumber(int);           /* 设置当前已处理过的撤单数 */
void            IncreaseWorkedWithdrawNumber();         /* 递增当前已处理过的撤单数 */

BOOL			CancelOrder(WITHDRAW_CS*);				/* 数据加工-取消订单 */
BOOL			ModifyOrder(WITHDRAW_CS*);				/* 数据加工-修改订单 */
BOOL			DeleteOrder(WITHDRAW_CS*);				/* 数据加工-删除订单 */
void			UpdateStockSort(WITHDRAW_CS*);			/* 更新Stock撤单队列 */
BOOL			IsBuyWithdraw(WITHDRAW_CS*);			/* 返回指定的委托是否是买入委托 */
BOOL			IsSellWithdraw(WITHDRAW_CS*);			/* 返回指定的委托是否是卖出委托 */

BOOL			PrintWithdrawToFile();
void            PrintWithdrawInfoToFile(int);			/* 打印撤单信息到指定的文件中 */
BOOL			PrintStockToFile();
#endif  /* _MODEL_WITHDRAW_H */
