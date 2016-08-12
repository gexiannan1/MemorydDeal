/******************************************************************
** 文件名:      stock_pbu.h
** 版 本:       0.1
** 创建人:      Li Bo
**              
** 日 期:       2006.12.11
** 描 述:       Stock中的PBU信息定义的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
 * 常量定义
 */
#define				PBU_ORDER_STAT_ADD			0
#define				PBU_ORDER_STAT_DEL			1

#define				PBU_ORDER_SORT_BUY			0					/* pbu排名标记 买 */
#define				PBU_ORDER_SORT_SELL			1					/* pbu排名标记 卖*/


/*
 * 结构体定义
 */

typedef struct {
	char			pbuFirm[PBU_FIRM_LENGTH];						/* pbu号码 */
} PBU_FIRM;                                                 		
                                                            		
//pbu信息                                                   		
typedef struct {                                            		
	int				pbuDataAdr;										/* 指定的pbu对应的数据位置 */
	int				pbuBuyOrderSortNo;								/* 指定的pbu的买委托排名(从0开始，初始化时为-1) */
	int				pbuSellOrderSortNo;								/* 指定的pbu的卖委托排名(从0开始，初始化时为-1) */
} PBU_HEAD;                                             			
                                                            		
                                                            		
                                                            		
//pbu数据                                               			
typedef struct {                                        			
	long long		orderBuyAmount;									/* PBU对应证券的买委托当前交易时段数量 */
	long long		orderBuyMoney;									/* PBU对应证券的买委托当前交易时段金额 */
	long long		orderOABuyAmount;								/* PBU对应证券的买委托OA交易时段数量 */
	long long		orderOABuyMoney;								/* PBU对应证券的买委托OA交易时段金额 */
	long long		orderAUBuyAmount;								/* PBU对应证券的买委托AU交易时段数量 */
	long long		orderAUBuyMoney;								/* PBU对应证券的买委托AU交易时段金额 */
	long long		orderSUBuyAmount;								/* PBU对应证券的买委托SU交易时段数量 */
	long long		orderSUBuyMoney;								/* PBU对应证券的买委托SU交易时段金额 */
	long long		orderFPBuyAmount;								/* PBU对应证券的买委托FP交易时段数量 */
	long long		orderFPBuyMoney;								/* PBU对应证券的买委托FP交易时段金额 */
	                                                    			
	long long		orderSellAmount;								/* PBU对应证券的卖委托当前交易时段数量 */
	long long		orderSellMoney;									/* PBU对应证券的卖委托当前交易时段金额 */
	long long		orderOASellAmount;								/* PBU对应证券的卖委托OA交易时段数量 */
	long long		orderOASellMoney;								/* PBU对应证券的卖委托OA交易时段金额 */
	long long		orderAUSellAmount;								/* PBU对应证券的卖委托AU交易时段数量 */
	long long		orderAUSellMoney;								/* PBU对应证券的卖委托AU交易时段金额 */
	long long		orderSUSellAmount;								/* PBU对应证券的卖委托SU交易时段数量 */
	long long		orderSUSellMoney;								/* PBU对应证券的卖委托SU交易时段金额 */
	long long		orderFPSellAmount;								/* PBU对应证券的卖委托FP交易时段数量 */
	long long		orderFPSellMoney;								/* PBU对应证券的卖委托FP交易时段金额 */
	                                                        		
	long long		tradeBuyAmount;									/* PBU对应证券的买成交数量 */
	long long		tradeBuyMoney;									/* PBU对应证券的买成交金额 */
	long long		tradeSellAmount;								/* PBU对应证券的卖成交数量 */
	long long		tradeSellMoney;									/* PBU对应证券的卖成交金额 */
} PBU_DATA;                                                 		
                                                            		
                                                            		
/*                                                          		
 * 函数声明                                                 		
 */                                                         		
void			InitPBU();											/* 初始化PBU内存区 */
                                                            		
PBU_FIRM*		GetPBUFirm();										/* 返回PBU号的起始位置 */
PBU_FIRM*		GetPBUFirmByPbuNo(int);								/* 根据pbu的位置获得pbu号信息 */
                                                            		
int				GetPBUNoByFirm(char* firm);							/* 根据pbu代码获得此pbu在pbu号码表中的位置 */
int				GetPBUNumber();										/* 获得当前已经存储的pbu号码数量 */
int				AddFirm(char*);										/* 添加pbu席位号到共享内存 */
PBU_DATA*		AddNewPBUData(PBU_HEAD*);							/* 添加新的pbu数据 */

PBU_HEAD*		GetPBUTableByStockNo(int, int);						/* 根据stockNo和pbu在号码表的位置返回对应的pbu信息地址 */
int*			GetPBUBuySortByStockNo(int, int);					/* 根据stockNo和排名返回对应的PBU买委托排名地址 */
int*			GetPBUSellSortByStockNo(int, int);					/* 根据stockNo和排名返回对应的PBU卖委托排名地址 */

void			PBUStatusChange(STOCK_CS*);							/* 交易时段变换处理 */
void			ResortPBUSortList(STOCK_CS*);						/* PBU重新排名处理 */
                                                            		
PBU_DATA*		GetPBUDataByIndex(int index);						/* 根据地址号获取对应的PBU数据地址 */
int				GetPBUDataNumber();									/* 获取当前已经存储的PBU数据数量 */

void			UpdatePBUTradeStat(PBU_DATA*, TRADE_CS*);			/* 维护pbu成交统计 */

void			PBUOrderStatAdd(STOCK_CS*,PBU_DATA*, ORDER_CS*);				/* pbu交易时段委托统计（增加） */
void			PBUOrderStatDel(STOCK_CS*,PBU_DATA*, ORDER_CS*, long long);		/* pbu交易时段委托统计（减少） */

void			UpdatePBUOrderSortList(STOCK_CS*, int, ORDER_CS*);	/* 维护pbu委托排名 */
void			UpdatePBUTradeSortList(STOCK_CS*, int, TRADE_CS*);	/* 维护pbu成交排名 */

void			PrintPBUList();										/* 输出pbu订单和成交排名到文件中 */

#endif  /* _STOCK_PBU_H */
