/******************************************************************
** 文件名:      order.h
** 版 本:       0.1
** 创建人:      zhaodj
** 日 期:       2005.9.6
** 描 述:       ORDER信息定义的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/order.h,v 1.1 2012/03/29 01:19:53 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:53 $
 */

#ifndef _MODEL_ORDER_H
#define _MODEL_ORDER_H

#include	"stock.h"
#include    "../util/symbol.h"


/*
 * 结构体定义
 */

/*
 * ORDER信息
 */
typedef struct {
    /*
     * 从TE系统导入的信息
     * 为保障结构定义的直观, 未直接引入 ORDER_TE 结构
     * <b>
     * 若 ORDER_TE 结构发生改变, 需要同步修改该区段
     * </b>
     */
    unsigned long long		orderNo;                                    /* 订单号 */
    unsigned long long		orderNoOld;									/* 旧订单号 */
    unsigned long long		orderNoOld1;								/* 旧订单号 */
    int						orderProcessNo;								/* 订单序列号 */
    int						orderProcessNoOld;							/* 旧订单序列号 */
    int						orderProcessNoOld1;							/* 旧订单序列号 */
    int			            orderTime;                                  /* 订单接收时间 */
    char					isinCod[STOCK_ISIN_CODE_LENGTH];			/* 股票国际证券标识码*/
    char    				pbuId[PBU_FIRM_LENGTH];           		/* pbu号 */
    char    				orderBSType;                            	/* 买卖标志 - BUY 或 SELL */
    char    				orderType;                       			/* 订单类型 */
    long long  				orderPrice;                             	/* 价格 */
    long long  				orderQty;                               	/* 数量 */
    char					trdResTypCod[3];							/* 交易限制类型 */
    char					trnTypId;									/* 事务类型 */
    char					orderStatus; 								/* 订单状态 */
    long long				remPeakQty;									/* 剩余揭示数量 */
    long long				peakSizeQty;								/* 最大揭示数量 */
    char					ordrResCod;									/* 订单限制类型 */
    /* -------------------------           */

    long long	balance;                                    /* 余额（扣除已成交和已撤单后的剩余未处理数量，最终应为0） */
    char    	status;                                     /* 状态 - OPEN, MATCHED; OPEN 的才参与撮合 */

    /*
     * 结构指针
     */
    int			stockNo;                            		/* 股票记录号，stock表入口 */
            	
    int     	nextStock;                                  /* 同股票下一笔委托 */
            	
    int     	priceLevel;                                 /* 该委托价位在价位列表中的位置 */
            	
    int     	prevPrice;                                  /* 同价位上一笔委托 */
    int     	nextPrice;                                  /* 同价位下一笔委托 */
    
    /* -------------------------           */           	

} ORDER_CS;


/*
 * 函数声明
 */
ORDER_CS*   GetOrderList();                             /* 返回Order数组的起始位置 */
ORDER_CS*   GetOrder(int index);                        /* 返回指定位置的ORDER_CS */
int         GetOrderNumber();                           /* 返回Order数量 */
//int         AddOrder(ORDER_TE*);                        /* 向Order数组中增加Order条目 */
int         PushOrder(ORDER_CS*);                        /* 将收到的订单丢到共享内存 */
BOOL 		DetachOrder(ORDER_CS*);						/* 从订单簿中删除order条目（把order从链表中脱离） */
BOOL		AttachOrder(ORDER_CS*);						/* 把order条目添加到订单簿中 */
void		UpdateStockDel(STOCK_CS*, ORDER_CS*);		/* 更新证券信息，去掉对应订单的统计 */
void		UpdateStockAdd(STOCK_CS*, ORDER_CS*);		/* 更新证券信息，增加对应订单的统计 */

BOOL		IsIceOrder(ORDER_CS*);						/* 判断订单是否为冰山订单 */

void		StockStatAdd(ORDER_CS*);					/* 把订单信息添加到交易时段统计信息 */
void		StockStatDel(ORDER_CS*, long long);			/* 从交易时段统计删除Order信息 */

int         GetWorkedOrderNumber();                     /* 返回当前已处理过的委托数 */
void        SetWorkedOrderNumber(int);                  /* 设置当前已处理过的委托数 */
void        IncreaseWorkedOrderNumber();                /* 递增当前已处理过的委托数 */

BOOL        IsBuyOrder(ORDER_CS*);                      /* 返回指定的委托是否是买入委托 */
BOOL        IsSellOrder(ORDER_CS*);                     /* 返回指定的委托是否是卖出委托 */

BOOL        IsTradingPhase(ORDER_CS*);					/* 返回Order是否参加产品交易时段 */

BOOL        IsOrderOutmoded(ORDER_CS*);                 /* 返回委托是否已过时（如已全部成交或撤单）*/
BOOL        IsOrderProcessComplete();                   /* 返回委托是否已全部处理完毕 */

void        PrintOrderInfoToFile(int fp);               /* 打印委托信息到指定的文件中 */
void		PrintOrderCS(ORDER_CS*);

#endif  /* _MODEL_ORDER_H */
