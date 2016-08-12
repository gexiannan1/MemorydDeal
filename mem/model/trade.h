/******************************************************************
** 文件名:      trade.h
** 版 本:       0.1
** 创建人:      zhaodj
** 日 期:       2005.9.6
** 描 述:       TRADE信息定义的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
** 2006.03.23   Wei Huanyin 1、IsValidTrade函数返回值由char改为int
							2、VALID_TRADE 值改为 0 INVALID_TRADE值改为1
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
#define VALID_TRADE         0         /* 表示数据符合条件 */
#define INVALID_TRADE       1         /* 表示数据不符合条件 */

#define TRADE_TYPE_NORMAL_TRD               'X'

/*
 * 结构体定义
 */

/*
 * TRADE信息
 */
typedef struct {
    /*
     * 从TE系统导入的信息
     * 为保障结构定义的直观, 未直接引入 TRADE_TE 结构
     * <b>
     * 若 TRADE_TE 结构发生改变, 需要同步修改该区段
     * </b>
     */
    unsigned long long			tradeNo;                                    /* 交易号 */
    int				            tradeTime;                                  /* 交易发生时间，hhmmss */
    int							tradeProcessNo;								/* 成交处理号 */
    char						isinCod[STOCK_ISIN_CODE_LENGTH];			/* 股票国际证券标识码*/
    unsigned long long			orderNo;                                    /* 交易对应的订单号 */
    char						orderType;									/* 交易对应的订单的类型 */
    char    					pbuId[PBU_FIRM_LENGTH];           			/* pbu号 */
    int							orderProcessNo;								/* 成交对应的订单处理号 */
    long long					price;                                      /* 价格 */
    long long					qty;                                        /* 数量 */
    long long					mktVal;										/* 成交金额 */
    char						type;                                       /* 交易类型 */
    char						trnTypId;									/* 事务类型 */
    char						tradeBSType;								/* 交易买卖类型 */
    char						tradeBSFlag;								/* 买卖盘标记 */
    long long					remPeakQty;									/* 剩余最高揭示数量 */
    long long					peakSizeQty;								/* 最大揭示数量 */
    unsigned long				ordrEntTim;									/* 冰山订单重新披露时间 */
    /* -------------------------           */

    int     index;                                     /* 成交序号（从0开始） */
	
	int		stockNo;									/* 股票记录号，stock表入口 */
    /*
     * 结构指针
     */
    int     nextStock;                                  /* 同股票下一笔成交 */
    /* -------------------------           */

} TRADE_CS;


/*
 * 函数声明
 */
TRADE_CS*   GetTradeList();                             /* 返回Trade数组的起始位置 */
TRADE_CS*   GetTrade(int index);                        /* 返回指定位置的TRADE_CS */
int         GetTradeNumber();                           /* 返回Trade数量 */
TRADE_CS*   GetLastTrade();                             /* 返回最后一笔成交 */
int         AddTrade(TRADE_TE*);                        /* 向Trade数组中增加Trade条目 */

BOOL		IsBuyTrade(TRADE_CS*);						/* 返回是否是买类型成交 */
BOOL		IsSellTrade(TRADE_CS*);						/* 返回是否是卖类型成交 */
BOOL		IsMarketPriceTrade(TRADE_CS*);				/* 返回是否是市价委托成交 */

int         GetWorkedTradeNumber();                     /* 返回当前已处理过的成交数量 */
void        SetWorkedTradeNumber(int);                  /* 设置当前已处理过的成交数量 */
void        IncreaseWorkedTradeNumber();                /* 递增当前已处理过的成交数量 */

int         GetSentTradeNumber();                       /* 返回当前已发送的成交数量 */
void        SetSentTradeNumber(int);                    /* 设置当前已发送的成交数量 */
void        IncreaseSentTradeNumber();                  /* 递增当前已发送的成交数量 */

BOOL        IsPendingTrade(TRADE_CS*);                  /* 返回指定的成交是否需要加工 */
int        	IsValidTrade(TRADE_CS*);                    /* 判断逐笔成交是否是符合发布条件的数据 */

void        PrintTradeInfoToFile(int fp);               /* 打印成交信息到指定的文件中 */


#endif  /* _MODEL_TRADE_H */
