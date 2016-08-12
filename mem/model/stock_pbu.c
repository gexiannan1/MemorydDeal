/******************************************************************
** 文件名:      stock_pbu.c
** 版 本:       0.1
** 创建人:      Li Bo
** 日 期:       2006.12.12
** 描 述:       Stock中的PBU信息处理
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/stock_pbu.c,v 1.1 2012/03/29 01:19:54 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:54 $
 */

#include	<stdlib.h> 
#include	<stdio.h>
#include    <string.h>
#include    <fcntl.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include	<unistd.h>

#include	"model.h"
#include	"../memory.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"
#include    "../util/log/log.h"
#include    "stock.h"


/*
 * 模块名称
 */
static const char   _moduleName[] = "stock_pbu";

/*
 * 内部函数声明
 */
static PBU_HEAD*	GetPBUTable();										/* 返回PBU索引列表的起始位置 */
static int*		   	GetPBUBuySort();									/* 返回PBU买委托排名的起始位置 */
static int*			GetPBUSellSort();									/* 返回PBU卖委托排名的起始位置 */
static PBU_DATA*	GetPBUData();										/* 返回PBU数据的起始位置 */

//static void			UpdatePBUBuyOrderSortList(STOCK_CS*, int, int, int);		/* pbu买委托重新排名 */
//static void			UpdatePBUSellOrderSortList(STOCK_CS*, int, int, int);		/* pbu卖委托重新排名 */

//static void			UpdatePBUBuyTradeSortList(STOCK_CS*, int);			/* pbu买成交重新排名 */
//static void			UpdatePBUSellTradeSortList(STOCK_CS*, int);			/* pbu卖成交重新排名 */

static int 			CompFirm(PBU_FIRM *p1, PBU_FIRM *p2);				/* 二分法需要的比较函数 */



/*****************************************************************
** 函数名:      InitPBU
** 输 入:       无
** 输 出:       无
** 功能描述:    初始化pbu内存区，包括pbu索引列表，pbu排名区（初始数值不为0的区域）
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
InitPBU(){
	#ifdef USE_PBU
		PBU_HEAD*	pbuHead = NULL;
		int*		pbuBuySort = NULL;
		int*		pbuSellSort = NULL;
		int			i = 0;
		int			j = 0;
		
		MEM_CONFIG_SHARE *memConfig = GetShareMemConfigStats();
		
		pbuHead = GetPBUTable();
		pbuBuySort = GetPBUBuySort();
		pbuSellSort = GetPBUSellSort();
		for(i = 0; i < memConfig->stockMax; i++){
			for(j = 0; j < memConfig->pbuNumberMax; j++, pbuHead++, pbuBuySort++, pbuSellSort++){
				pbuHead->pbuDataAdr = -1;
				pbuHead->pbuBuyOrderSortNo = -1;
				pbuHead->pbuSellOrderSortNo = -1;
				
				*pbuBuySort = -1;
				*pbuSellSort = -1;
			}
		}
	#endif
}


/*****************************************************************
** 函数名:      GetPBUFirm
** 输 入:       无
** 输 出:       PBU_FIRM*
** 功能描述:    返回PBU号的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
PBU_FIRM*	
GetPBUFirm(){
	#ifdef USE_PBU
		return (PBU_FIRM*) GetShareMemory() + GetShareMemAllocationStats()->pbuFirmOffset;
	#endif
	
	return NULL;
}



/*****************************************************************
** 函数名:      GetPBUFirmByPbuNo
** 输 入:       int
** 输 出:       PBU_FIRM*
** 功能描述:    根据pbu的位置获得pbu号信息
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
PBU_FIRM* 
GetPBUFirmByPbuNo(int pbuNo){
	#ifdef USE_PBU
		if(pbuNo < 0 || pbuNo >= GetShareMemConfigStats()->pbuNumberMax){
			return NULL;
		}
		return GetPBUFirm() + pbuNo;
	#endif
	
	return NULL;
}



/*****************************************************************
** 函数名:      GetPBUNoByFirm
** 输 入:       无
** 输 出:       int
** 功能描述:    根据pbu代码获得此pbu在索引列表中的相对于首地址的位置
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int 
GetPBUNoByFirm(char* firm){
    PBU_FIRM*		target = NULL;
    PBU_FIRM		firmComp;
    
    memcpy(firmComp.pbuFirm, firm, PBU_FIRM_LENGTH);
    
    target = (PBU_FIRM*)bsearch(&firmComp, GetPBUFirm(),
		GetPBUNumber(), sizeof(PBU_FIRM), (int(*)(const void *,const void *))CompFirm);
		
	if(target == NULL){
		return -1;
	}

    return target - GetPBUFirm();
}


/*****************************************************************
** 函数名:      GetPBUNumber
** 输 入:       无
** 输 出:       int
** 功能描述:    获得当前已经存储的pbu号码数量
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int	
GetPBUNumber(){
	#ifdef USE_PBU
		return GetShareMemCurrentStats()->nextPBUFirm;
	#endif
	
	return -1;
}


/*****************************************************************
** 函数名:      GetPBUDataNumber
** 输 入:       无
** 输 出:       int
** 功能描述:    获取当前已经存储的PBU数据数量
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int 
GetPBUDataNumber(){
	#ifdef USE_PBU
		return GetShareMemCurrentStats()->nextPBUData;
	#endif
	
	return -1;
}


/*****************************************************************
** 函数名:      GetPBUTableByStockNo
** 输 入:       stockNo firmIndex
** 输 出:       PBU_HEAD
** 功能描述:    根据stockNo和pbu在号码表的位置返回对应的pbu信息地址
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
PBU_HEAD*	
GetPBUTableByStockNo(int stockNo, int firmIndex){
	#ifdef USE_PBU
		if(stockNo < 0 || firmIndex < 0 || stockNo >= GetShareMemConfigStats()->stockMax || firmIndex >= GetShareMemConfigStats()->pbuNumberMax){
			return NULL;
		}
		return GetPBUTable() + GetShareMemConfigStats()->pbuNumberMax * stockNo + firmIndex;
	#endif
	return NULL;
}


/*****************************************************************
** 函数名:      GetPBUBuySortByStockNo
** 输 入:       stockNo sortIndex
** 输 出:       int
** 功能描述:    根据stockNo和排名返回对应的PBU买委托排名地址
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int*	
GetPBUBuySortByStockNo(int stockNo, int sortIndex){
	#ifdef USE_PBU
		if(stockNo < 0 || sortIndex < 0 || stockNo >= GetShareMemConfigStats()->stockMax || sortIndex >= GetShareMemConfigStats()->pbuNumberMax){
			return NULL;
		}
		return GetPBUBuySort() + GetShareMemConfigStats()->pbuNumberMax * stockNo + sortIndex;
	#endif
	return NULL;
}



/*****************************************************************
** 函数名:      GetPBUSellSortByStockNo
** 输 入:       stockNo sortIndex
** 输 出:       int
** 功能描述:    根据stockNo和排名返回对应的PBU卖委托排名地址
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int* 
GetPBUSellSortByStockNo(int stockNo, int sortIndex){ 
	#ifdef USE_PBU
		if(stockNo < 0 || sortIndex < 0 || stockNo >= GetShareMemConfigStats()->stockMax || sortIndex >= GetShareMemConfigStats()->pbuNumberMax){
			return NULL;
		}
		return GetPBUSellSort() + GetShareMemConfigStats()->pbuNumberMax * stockNo + sortIndex;
	#endif
	return NULL;
}


/*****************************************************************
** 函数名:      GetPBUDataByIndex
** 输 入:       index
** 输 出:       PBU_DATA
** 功能描述:    根据地址号获取对应的PBU数据地址
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
PBU_DATA* 
GetPBUDataByIndex(int index){
	#ifdef USE_PBU
		if(index < 0 || index >= GetShareMemConfigStats()->pbuDataNumberMax){
			return NULL;
		}
		return GetPBUData() + index;
	#endif
	return NULL;
}


/*****************************************************************
** 函数名:      AddFirm
** 输 入:       pbuFirm
** 输 出:       插入的位置
** 功能描述:    添加pbu席位号到共享内存(同时排序)
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int	
AddFirm(char* pbuFirm){
	#ifdef USE_PBU
		PBU_FIRM*	firm = NULL;
		int         index   = GetShareMemCurrentStats()->nextPBUFirm;
		int			insertIndex = 0;
		int			i		= 0;
		
		//检查内存是否越界
	    if (index >= GetShareMemConfigStats()->pbuNumberMax) {
	        FATAL("stock_pbu.AddFirm()", "pbu数量超出预定义的最大pbu数量！ Current Number: %d"
	                , index);
	        return -1;
	    }
		
		firm = GetPBUFirm();
		
		for(i = 0; i < index; i++){
			if(strcmp(firm->pbuFirm, pbuFirm) > 0){
				break;
			}
			firm++;
		}
		
		memmove(firm + 1, firm, sizeof(PBU_FIRM) * (index - (firm - GetPBUFirm())));
		
		memcpy(firm->pbuFirm, pbuFirm, PBU_FIRM_LENGTH);
		
		GetShareMemCurrentStats()->nextPBUFirm++;
		
		return firm - GetPBUFirm();
	#endif
	return -1;
}


/*****************************************************************
** 函数名:      PBUOrderStatAdd
** 输 入:       stockCS, pbuData, order
** 输 出:       无
** 功能描述:    pbu交易时段委托统计（增加）
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.16
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
PBUOrderStatAdd(STOCK_CS* stockCS, PBU_DATA* pbuData, ORDER_CS* order){
	if(pbuData == NULL || order == NULL){
		ERROR(_moduleName, "PBUOrderStatAdd输入参数不能为NULL");
		return;
	}
	
	if(IsBuyOrder(order)){
		if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
			pbuData->orderOABuyAmount += order->balance;
			pbuData->orderOABuyMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
			pbuData->orderAUBuyAmount += order->balance;
			pbuData->orderAUBuyMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
			pbuData->orderSUBuyAmount += order->balance;
			pbuData->orderSUBuyMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
			pbuData->orderFPBuyAmount += order->balance;
			pbuData->orderFPBuyMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else{
			//其他类型不需要统计
		}
	}
	else if(IsSellOrder(order)){
		if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
			pbuData->orderOASellAmount += order->balance;
			pbuData->orderOASellMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
			pbuData->orderAUSellAmount += order->balance;
			pbuData->orderAUSellMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
			pbuData->orderSUSellAmount += order->balance;
			pbuData->orderSUSellMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
			pbuData->orderFPSellAmount += order->balance;
			pbuData->orderFPSellMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else{
			//其他类型不需要统计
		}
	}
	
	//维护pbu当前交易时段统计信息
	if(IsTradingPhase(order)){
		if(IsBuyOrder(order)){
			if(IsIceOrder(order)){
				//冰山订单统计揭示数量
				TRACE(_moduleName,"开始pbu当前交易时段统计（冰山订单），pbu[%s]买入总数量[%lld]",order->pbuId, pbuData->orderBuyAmount);
				pbuData->orderBuyAmount += order->remPeakQty;
				pbuData->orderBuyMoney += (stockCS->lotSize * order->remPeakQty / TEN_THOUSAND * order->orderPrice);
				TRACE(_moduleName,"pbu当前交易时段统计结束，冰山订单揭示数量[%lld]，pbu[%s]买入总数量[%lld]",order->remPeakQty, order->pbuId, pbuData->orderBuyAmount);
			}
			else{
				pbuData->orderBuyAmount += order->balance;
				pbuData->orderBuyMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
			}
		}
		else if(IsSellOrder(order)){
			if(IsIceOrder(order)){
				//冰山订单统计揭示数量
				TRACE(_moduleName,"开始pbu当前交易时段统计（冰山订单），pbu[%s]卖出总数量[%lld]",order->pbuId, pbuData->orderSellAmount);
				pbuData->orderSellAmount += order->remPeakQty;	
				pbuData->orderSellMoney += (stockCS->lotSize * order->remPeakQty / TEN_THOUSAND * order->orderPrice);
				TRACE(_moduleName,"pbu当前交易时段统计结束，冰山订单揭示数量[%lld]，pbu[%s]卖出总数量[%lld]",order->remPeakQty, order->pbuId, pbuData->orderSellAmount);
			}
			else{
				pbuData->orderSellAmount += order->balance;	
				pbuData->orderSellMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
			}
		}
	}
}


/*****************************************************************
** 函数名:      PBUOrderStatDel
** 输 入:       pbuData, order, amount
** 输 出:       无
** 功能描述:    pbu交易时段委托统计（减少）
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.16
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
PBUOrderStatDel(STOCK_CS* stockCS, PBU_DATA* pbuData, ORDER_CS* order, long long amount){
	if(pbuData == NULL || order == NULL){
		ERROR(_moduleName, "PBUOrderStatDel输入参数不能为NULL");
		return;
	}
	
	if(amount < 0){
		ERROR(_moduleName, "PBUOrderStatDel输入参数amount不能小于0");
		return;
	}

	if(IsBuyOrder(order)){
		if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
			pbuData->orderOABuyAmount -= amount;
			pbuData->orderOABuyMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
			pbuData->orderAUBuyAmount -= order->balance;
			pbuData->orderAUBuyMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
			pbuData->orderSUBuyAmount -= order->balance;
			pbuData->orderSUBuyMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
			pbuData->orderFPBuyAmount -= order->balance;
			pbuData->orderFPBuyMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else{
			//其他类型不需要统计
		}
	}
	else if(IsSellOrder(order)){
		if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
			pbuData->orderOASellAmount -= amount;
			pbuData->orderOASellMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
			pbuData->orderAUSellAmount -= order->balance;
			pbuData->orderAUSellMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
			pbuData->orderSUSellAmount -= order->balance;
			pbuData->orderSUSellMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
			pbuData->orderFPSellAmount -= order->balance;
			pbuData->orderFPSellMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else{
			//其他类型不需要统计
		}
	}
	
	//维护pbu当前交易时段统计信息
	if(IsTradingPhase(order)){
		if(IsBuyOrder(order)){
			pbuData->orderBuyAmount -= amount;
			pbuData->orderBuyMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(IsSellOrder(order)){
			pbuData->orderSellAmount -= amount;	
			pbuData->orderSellMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
	}
}


/*****************************************************************
** 函数名:      UpdatePBUTradeStat
** 输 入:       pbuData, trade
** 输 出:       无
** 功能描述:    维护pbu成交统计
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.16
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
UpdatePBUTradeStat(PBU_DATA* pbuData, TRADE_CS* trade){
	if(pbuData == NULL || trade == NULL){
		ERROR(_moduleName, "UpdatePBUTradeStat输入参数不能为NULL");
		return;
	}
	
	if(IsBuyTrade(trade)){
		pbuData->tradeBuyAmount += trade->qty;
		pbuData->tradeBuyMoney += trade->mktVal;
	}
	else if(IsSellTrade(trade)){
		pbuData->tradeSellAmount += trade->qty;
		pbuData->tradeSellMoney += trade->mktVal;
	}
}


/*****************************************************************
** 函数名:      AddNewPBUData
** 输 入:       pbuHead
** 输 出:       PBU_DATA*
** 功能描述:    添加新的pbu数据
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.16
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
PBU_DATA*	
AddNewPBUData(PBU_HEAD* pbuHead){
	#ifdef USE_PBU
		int index = GetShareMemCurrentStats()->nextPBUData;
		
		if(index >= GetShareMemConfigStats()->pbuDataNumberMax){
			ERROR(_moduleName, "添加pub数据失败，pbu数据数量超出预定义的最大数量！ Current Number: %d", index);
			return NULL;
		}
		
		pbuHead->pbuDataAdr = index;
		GetShareMemCurrentStats()->nextPBUData++;
		return GetPBUDataByIndex(index);
	#endif
	return NULL;
}


/*****************************************************************
** 函数名:      UpdatePBUOrderSortList
** 输 入:       stock, pbuNo, order
** 输 出:       无
** 功能描述:    维护pbu委托排名
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.17
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void	
UpdatePBUOrderSortList(STOCK_CS* stock, int pbuNo, ORDER_CS* order){
	PBU_HEAD*	pbuHead = NULL;
	PBU_HEAD*	pbuHeadCmp = NULL;
	PBU_DATA*	pbuData = NULL;
	PBU_DATA*	pbuDataCmp = NULL;
	int*		pbuSortList = NULL;
	int			sortNo = -1;
	int			begin = -1;
	int			end = -1;
	
	if(stock == NULL){
		ERROR(_moduleName, "UpdatePBUOrderSortList的输入参数不能为NULL");
		return;
	}
	
	pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
	if(pbuHead == NULL){
		ERROR(_moduleName, "pbu委托排名失败，未找到对应的pbu索引信息");
		return;
	}
	
	if(IsBuyOrder(order)){
		if(pbuHead->pbuBuyOrderSortNo == -1){
			begin = 0;
			end = stock->lastBuyOrderSortOffset;
			//需要新插入数据，所以要更新stock信息中的pbu排名最后存放位置
			stock->lastBuyOrderSortOffset++;
		}
		else {
			pbuSortList = GetPBUBuySortByStockNo(stock->stockNo, pbuHead->pbuBuyOrderSortNo);
			pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
			if(pbuData == NULL){
				ERROR(_moduleName, "pbu委托排名失败，未找到对应的pbu数据信息");
				return;
			}
			
			//当前排名是第一名
			if(pbuHead->pbuBuyOrderSortNo == 0){
				//与第二名比较
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList + 1));
				if(pbuHeadCmp == NULL){
					//当前排名列表里只有一个，不用再排
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbu委托排名失败，未找到对应的pbu数据信息");
					return;
				}
				if(pbuData->orderBuyAmount < pbuDataCmp->orderBuyAmount){
					begin = 1;
					end = stock->lastBuyOrderSortOffset;
				}
				else{
					//已经是第一了，不需要再排
					return;	
				}
			}
			//当前排名是最后一名
			else if(pbuHead->pbuBuyOrderSortNo == stock->lastBuyOrderSortOffset){
				//与倒数第二名比较
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList - 1));
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu信息");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu数据信息");
					return;
				}
				
				if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
					begin = 0;
					end = stock->lastBuyOrderSortOffset - 1;
				}
				else{
					//仍然是倒数第一，不需要再排
					return;	
				}
			}
			//当前排名不是第一也不是最后一名
			else{
				//与前边一名比较
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList - 1));
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu信息");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu数据信息");
					return;
				}
				if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
					begin = 0;
					end = pbuHead->pbuBuyOrderSortNo - 1;
				}
				else if(pbuData->orderBuyAmount == pbuDataCmp->orderBuyAmount){
					//与前面一名相等，那么不需要重新排名
					return;
				}
				else{
					//与后面一名比较
					pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList + 1));
					if(pbuHeadCmp == NULL){
						ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu信息");
						return;
					}
					pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
					if(pbuDataCmp == NULL){
						ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu数据信息");
						return;
					}
					if(pbuData->orderBuyAmount < pbuDataCmp->orderBuyAmount){
						begin = pbuHead->pbuBuyOrderSortNo + 1;
						end = stock->lastBuyOrderSortOffset;
					}
					else{
						//比前面一名小，并且比后面一名大(或者相等)，所以不用再重排
						return;
					}
				}
			}
		}
		//重新排名
		//UpdatePBUBuyOrderSortList(stock, pbuNo, begin, end);
	}
	else if(IsSellOrder(order)){
		if(pbuHead->pbuSellOrderSortNo == -1){
			begin = 0;
			end = stock->lastSellOrderSortOffset;
			//需要新插入数据，所以要更新stock信息中的pbu排名最后存放位置
			stock->lastSellOrderSortOffset++;
		}
		else {
			pbuSortList = GetPBUSellSortByStockNo(stock->stockNo, pbuHead->pbuSellOrderSortNo);
			pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
			if(pbuData == NULL){
				ERROR(_moduleName, "pbu委托排名失败，未找到对应的pbu数据信息");
				return;
			}	
			
			//当前排名是第一名
			if(pbuHead->pbuSellOrderSortNo == 0){
				//与第二名比较
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList + 1));
				if(pbuHeadCmp == NULL){
					//当前排名列表里只有一个，不用再排
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbu委托排名失败，未找到对应的pbu数据信息");
					return;
				}
				if(pbuData->orderSellAmount < pbuDataCmp->orderSellAmount){
					begin = 1;
					end = stock->lastSellOrderSortOffset;
				}
				else{
					//已经是第一了，不需要再排
					return;	
				}
			}
			//当前排名是最后一名
			else if(pbuHead->pbuSellOrderSortNo == stock->lastSellOrderSortOffset){
				//与倒数第二名比较
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList - 1));
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu信息");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu数据信息");
					return;
				}
				
				if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
					begin = 0;
					end = stock->lastSellOrderSortOffset - 1;
				}
				else{
					//仍然是倒数第一，不需要再排
					return;	
				}
			}
			//当前排名不是第一也不是最后一名
			else{
				//与前边一名比较
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList - 1));
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu信息");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu数据信息");
					return;
				}
				if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
					begin = 0;
					end = pbuHead->pbuSellOrderSortNo - 1;
				}
				else if(pbuData->orderSellAmount == pbuDataCmp->orderSellAmount){
					//与前面一名相等，那么不需要重新排名
					return;
				}
				else{
					//与后面一名比较
					pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList + 1));
					if(pbuHeadCmp == NULL){
						ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu信息");
						return;
					}
					pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
					if(pbuDataCmp == NULL){
						ERROR(_moduleName, "pbu委托排名失败，未找到需要比较的pbu数据信息");
						return;
					}
					if(pbuData->orderSellAmount < pbuDataCmp->orderSellAmount){
						begin = pbuHead->pbuSellOrderSortNo + 1;
						end = stock->lastSellOrderSortOffset;
					}
					else{
						//比前面一名小，并且比后面一名大(或者相等)，所以不用再重排
						return;
					}
				}
			}
		}
		//重新排名
		//UpdatePBUSellOrderSortList(stock, pbuNo, begin, end);
	}
	else{
		ERROR(_moduleName, "pbu委托排名失败，订单类型错误");
		return;
	}
}


/*****************************************************************
** 函数名:      UpdatePBUTradeSortList
** 输 入:       stock, pbuNo, trade
** 输 出:       无
** 功能描述:    维护pbu成交排名
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.18
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
UpdatePBUTradeSortList(STOCK_CS* stock, int pbuNo, TRADE_CS* trade){
	if(IsBuyTrade(trade)){
	//	UpdatePBUBuyTradeSortList(stock, pbuNo);
	}
	else if(IsSellTrade(trade)){
		//UpdatePBUSellTradeSortList(stock, pbuNo);
	}
}


/*****************************************************************
** 函数名:      UpdatePBUBuyTradeSortList
** 输 入:       stock, pbuNo
** 输 出:       无
** 功能描述:    维护pbu买成交排名
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.19
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
/*static void 
UpdatePBUBuyTradeSortList(STOCK_CS* stock, int pbuNo){
	PBU_HEAD*	pbuHead = NULL;
	PBU_HEAD*	pbuHeadCmp = NULL;
	PBU_DATA*	pbuData = NULL;
	PBU_DATA*	pbuDataCmp = NULL;
	int			begin = -1;
	int			end = -1;
	int			i = 0;
	
	if(stock == NULL || pbuNo < 0){
		ERROR(_moduleName, "UpdatePBUBuyTradeSortList输入参数错误");
		return;
	}
	
	pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
	if(pbuHead == NULL){
		ERROR(_moduleName, "UpdatePBUBuyTradeSortList错误，未找到需要排序的pbu索引信息");
		return;
	}
	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
	if(pbuData == NULL){
		ERROR(_moduleName, "UpdatePBUBuyTradeSortList错误，pbu数据信息错误");
		return;
	}
	
	
	for(i = PBU_TRADE_SORT_LENGTH - 1; i >= 0; i--){
		if(stock->pbuTradeBuySort[i] == -1){
			continue;
		}
		if(stock->pbuTradeBuySort[i] == pbuNo){
			end = i;
			continue;
		}
		
		pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, stock->pbuTradeBuySort[i]);
		if(pbuHeadCmp == NULL){
			ERROR(_moduleName, "UpdatePBUBuyTradeSortList错误，未找到需要比较的pbu索引信息");
			return;
		}
		pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
		if(pbuDataCmp == NULL){
			ERROR(_moduleName, "UpdatePBUBuyTradeSortList错误，未找到需要比较的pbu数据");
			return;
		}
		
		if(pbuData->tradeBuyAmount <= pbuDataCmp->tradeBuyAmount){
			begin = i + 1;
			break;
		}
	}
	
	if(begin == PBU_TRADE_SORT_LENGTH){
		//要插入的成交数量小于最后一名的成交数量，因此不需要重新排序
		return;
	}
	if(begin == -1){
		//排名队列是空的，或者要插入的成交数量应该排在第一名
		begin = 0;
	}
	if(end == -1){
		//要插入的pbu成交没有在原来的排名队列中
		end = PBU_TRADE_SORT_LENGTH - 1;
	}
	
	if(begin == end){
		//排名没有变化，要重新排名的pbu仍然排在原位，或者正好排在最后一位
		stock->pbuTradeBuySort[begin] = pbuNo;
		return;
	}
	
	memmove(stock->pbuTradeBuySort + begin + 1, stock->pbuTradeBuySort + begin, sizeof(int) * (end - begin));
	stock->pbuTradeBuySort[begin] = pbuNo;
}*/


/*****************************************************************
** 函数名:      UpdatePBUSellTradeSortList
** 输 入:       stock, pbuNo
** 输 出:       无
** 功能描述:    维护pbu卖成交排名
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.19
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
/*static void 
UpdatePBUSellTradeSortList(STOCK_CS* stock, int pbuNo){
	PBU_HEAD*	pbuHead = NULL;
	PBU_HEAD*	pbuHeadCmp = NULL;
	PBU_DATA*	pbuData = NULL;
	PBU_DATA*	pbuDataCmp = NULL;
	int			begin = -1;
	int			end = -1;
	int			i = 0;
	
	if(stock == NULL || pbuNo < 0){
		ERROR(_moduleName, "UpdatePBUSellTradeSortList输入参数错误");
		return;
	}
	
	pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
	if(pbuHead == NULL){
		ERROR(_moduleName, "UpdatePBUSellTradeSortList错误，未找到需要排序的pbu索引信息");
		return;
	}
	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
	if(pbuData == NULL){
		ERROR(_moduleName, "UpdatePBUSellTradeSortList错误，pbu数据信息错误");
		return;
	}
	
	
	for(i = PBU_TRADE_SORT_LENGTH - 1; i >= 0; i--){
		if(stock->pbuTradeSellSort[i] == -1){
			continue;
		}
		if(stock->pbuTradeSellSort[i] == pbuNo){
			end = i;
			continue;
		}
		
		pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, stock->pbuTradeSellSort[i]);
		if(pbuHeadCmp == NULL){
			ERROR(_moduleName, "UpdatePBUSellTradeSortList错误，未找到需要比较的pbu索引信息");
			return;
		}
		pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
		if(pbuDataCmp == NULL){
			ERROR(_moduleName, "UpdatePBUSellTradeSortList错误，未找到需要比较的pbu数据");
			return;
		}
		
		if(pbuData->tradeSellAmount <= pbuDataCmp->tradeSellAmount){
			begin = i + 1;
			break;
		}
	}
	
	if(begin == PBU_TRADE_SORT_LENGTH){
		//要插入的成交数量小于最后一名的成交数量，因此不需要重新排序
		return;
	}
	if(begin == -1){
		//排名队列是空的，或者要插入的成交数量应该排在第一名
		begin = 0;
	}
	if(end == -1){
		//要插入的pbu成交没有在原来的排名队列中
		end = PBU_TRADE_SORT_LENGTH - 1;
	}
	
	if(begin == end){
		//排名没有变化，要重新排名的pbu仍然排在原位，或者正好排在最后一位
		stock->pbuTradeSellSort[begin] = pbuNo;
		return;
	}
	
	memmove(stock->pbuTradeSellSort + begin + 1, stock->pbuTradeSellSort + begin, sizeof(int) * (end - begin));
	stock->pbuTradeSellSort[begin] = pbuNo;
}*/



/*****************************************************************
** 函数名:      UpdatePBUBuyOrderSortList
** 输 入:       pbuNo:需要重新排名的pbu
				sortBegin查找起始偏移，sortEnd查找结束位置偏移
** 输 出:       无
** 功能描述:    pbu买委托重新排名
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.18
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
/*static void 
UpdatePBUBuyOrderSortList(STOCK_CS* stock, int pbuNo, int sortBegin, int sortEnd){
	PBU_DATA*	pbuData = NULL;
	PBU_DATA*	pbuDataCmp = NULL;
	PBU_HEAD*	pbuHead = NULL;
	PBU_HEAD*	pbuHeadCmp = NULL;
	PBU_HEAD*	pbuHeadChange = NULL;
	int*		sortChange = NULL;
	int*		sortList = NULL;
	int*		sortInsert = NULL;
	int			begin = sortBegin;
	int			end = sortEnd;
	int			comp = -1;
	int			i = 0;
	
	pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
	
	sortList = GetPBUBuySortByStockNo(stock->stockNo, begin);
	if(sortEnd == -1){
		*sortList = pbuNo;
		pbuHead->pbuBuyOrderSortNo = begin;
		return;
	}
	pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
	pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
	
	//要插入的pbu委托数量大于当前排在首位的pbu委托数量
	if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
		memmove(sortList + 1, sortList, sizeof(int) * (end - begin + 1));
		*sortList = pbuNo;
		for(i = begin; i <= end + 1; i++){
			sortChange = GetPBUBuySortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuBuyOrderSortNo = i;
		}
		return;
	}
	
	sortList = GetPBUBuySortByStockNo(stock->stockNo, end);
	pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
	pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
	//要插入的pbu委托数量小于当前排在末尾的pbu委托数量
	if(pbuData->orderBuyAmount < pbuDataCmp->orderBuyAmount){
		//如果搜索区间从0开始，说明是新插入的数据，直接加到排名列表尾部
		if(begin == 0){
			*(sortList + 1) = pbuNo;
			pbuHead->pbuBuyOrderSortNo = end + 1;
			return;
		}
		memmove(sortList - (end - begin + 1), sortList - (end - begin), sizeof(int) * (end - begin + 1));
		*sortList = pbuNo;
		for(i = begin - 1; i <= end; i++){
			sortChange = GetPBUBuySortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuBuyOrderSortNo = i;
		}
		return;
	}
	
	//使用二分法查找需要插入的位置
	while(end - begin > 1){
		comp = (end + begin) / 2;
		
		sortList = GetPBUBuySortByStockNo(stock->stockNo, comp);
		pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
		pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
		
		if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
			end = comp;
		}
		else if(pbuData->orderBuyAmount <= pbuDataCmp->orderBuyAmount){
			begin = comp;
		}
	}
	//需要插入的位置在begin或者end，需要根据插入方向决定需要移动的数据
	//如果搜索区间从0开始，那么插入位置为end，end后面的数据后移
	if(sortBegin == 0){
		sortInsert = GetPBUBuySortByStockNo(stock->stockNo, end);
		memmove(sortInsert + 1, sortInsert, sizeof(int) * (sortEnd - end + 1));
		*sortInsert = pbuNo;
		for(i = end; i <= sortEnd; i++){
			sortChange = GetPBUBuySortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuBuyOrderSortNo = i;
		}
	}
	//如果搜索区间不是从0开始，那么插入的位置为begin，begin前面的数据前移
	else{
		sortInsert = GetPBUBuySortByStockNo(stock->stockNo, begin);
		memmove(sortInsert - (begin - sortBegin + 1), sortInsert - (begin - sortBegin), sizeof(int) * (begin - sortBegin + 1));
		*sortInsert = pbuNo;
		for(i = sortBegin - 1; i <= begin; i++){
			sortChange = GetPBUBuySortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuBuyOrderSortNo = i;
		}
	}
}*/


/*****************************************************************
** 函数名:      UpdatePBUSellOrderSortList
** 输 入:       pbuNo:需要重新排名的pbu
				sortBegin查找起始偏移，sortEnd查找结束位置偏移
** 输 出:       无
** 功能描述:    pbu买委托重新排名
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.18
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
/*static void 
UpdatePBUSellOrderSortList(STOCK_CS* stock, int pbuNo, int sortBegin, int sortEnd){
	PBU_DATA*	pbuData = NULL;
	PBU_DATA*	pbuDataCmp = NULL;
	PBU_HEAD*	pbuHead = NULL;
	PBU_HEAD*	pbuHeadCmp = NULL;
	PBU_HEAD*	pbuHeadChange = NULL;
	int*		sortChange = NULL;
	int*		sortList = NULL;
	int*		sortInsert = NULL;
	int			begin = sortBegin;
	int			end = sortEnd;
	int			comp = -1;
	int			i = 0;
	
	pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
	
	sortList = GetPBUSellSortByStockNo(stock->stockNo, begin);
	if(sortEnd == -1){
		*sortList = pbuNo;
		pbuHead->pbuSellOrderSortNo = begin;
		return;
	}
	pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
	pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
	
	//要插入的pbu委托数量大于当前排在首位的pbu委托数量
	if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
		memmove(sortList + 1, sortList, sizeof(int) * (end - begin + 1));
		*sortList = pbuNo;
		for(i = begin; i <= end + 1; i++){
			sortChange = GetPBUSellSortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuSellOrderSortNo = i;
		}
		return;
	}
	
	sortList = GetPBUSellSortByStockNo(stock->stockNo, end);
	pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
	pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
	//要插入的pbu委托数量小于当前排在末尾的pbu委托数量
	if(pbuData->orderSellAmount < pbuDataCmp->orderSellAmount){
		//如果搜索区间从0开始，说明是新插入的数据，直接加到排名列表尾部
		if(begin == 0){
			*(sortList + 1) = pbuNo;
			pbuHead->pbuSellOrderSortNo = end + 1;
			return;
		}
		memmove(sortList - (end - begin + 1), sortList - (end - begin), sizeof(int) * (end - begin + 1));
		*sortList = pbuNo;
		for(i = begin - 1; i <= end; i++){
			sortChange = GetPBUSellSortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuSellOrderSortNo = i;
		}
		return;
	}
	
	//使用二分法查找需要插入的位置
	while(end - begin > 1){
		comp = (end + begin) / 2;
		
		sortList = GetPBUSellSortByStockNo(stock->stockNo, comp);
		pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
		pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
		
		if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
			end = comp;
		}
		else if(pbuData->orderSellAmount <= pbuDataCmp->orderSellAmount){
			begin = comp;
		}
	}
	//需要插入的位置在begin或者end，需要根据插入方向决定需要移动的数据
	//如果搜索区间从0开始，那么插入位置为end，end后面的数据后移
	if(sortBegin == 0){
		sortInsert = GetPBUSellSortByStockNo(stock->stockNo, end);
		memmove(sortInsert + 1, sortInsert, sizeof(int) * (sortEnd - end + 1));
		*sortInsert = pbuNo;
		for(i = end; i <= sortEnd; i++){
			sortChange = GetPBUSellSortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuSellOrderSortNo = i;
		}
	}
	//如果搜索区间不是从0开始，那么插入的位置为begin，begin前面的数据前移
	else{
		sortInsert = GetPBUSellSortByStockNo(stock->stockNo, begin);
		memmove(sortInsert - (begin - sortBegin + 1), sortInsert - (begin - sortBegin), sizeof(int) * (begin - sortBegin + 1));
		*sortInsert = pbuNo;
		for(i = sortBegin - 1; i <= begin; i++){
			sortChange = GetPBUSellSortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuSellOrderSortNo = i;
		}
	}
}*/


/*****************************************************************
** 函数名:      GetPBUData
** 输 入:       无
** 输 出:       PBU_DATA*
** 功能描述:    返回PBU数据的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static PBU_DATA*	
GetPBUData(){
	#ifdef USE_PBU
		return (PBU_DATA*) GetShareMemory() + GetShareMemAllocationStats()->pbuDataOffset;
	#endif
	return NULL;
}


/*****************************************************************
** 函数名:      GetPBUTable
** 输 入:       无
** 输 出:       PBU_HEAD*
** 功能描述:    返回PBU索引列表的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static PBU_HEAD*	
GetPBUTable(){
	#ifdef USE_PBU
		return (PBU_HEAD*) GetShareMemory() + GetShareMemAllocationStats()->pbuTableOffset;
	#endif
	return NULL;
}


/*****************************************************************
** 函数名:      GetPBUBuySort
** 输 入:       无
** 输 出:       int*
** 功能描述:    返回PBU买委托排名的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static int*   
GetPBUBuySort(){
	#ifdef USE_PBU
		return (int*) GetShareMemory() + GetShareMemAllocationStats()->pbuBuySortOffset;
	#endif
	return NULL;
}


/*****************************************************************
** 函数名:      GetPBUSellSort
** 输 入:       无
** 输 出:       int*
** 功能描述:    返回PBU卖委托排名的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static int*   
GetPBUSellSort(){
	#ifdef USE_PBU
		return (int*) GetShareMemory() + GetShareMemAllocationStats()->pbuSellSortOffset;
	#endif
	return NULL;
}


/*****************************************************************
** 函数名:      CompFirm
** 输 入:       char**
** 输 出:       int
** 功能描述:    二分法需要的比较函数
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static int 
CompFirm(PBU_FIRM *p1, PBU_FIRM *p2){ 
	return strcmp(p1->pbuFirm, p2->pbuFirm);
}


/*****************************************************************
** 函数名:      PBUStatusChange
** 输 入:       stock
** 输 出:       无
** 功能描述:    交易时段变换处理
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2007.3.6
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void	
PBUStatusChange(STOCK_CS* stock){
	PBU_HEAD*		pbuHead = NULL;
	PBU_DATA*		pbuData = NULL;
	int				i = 0;
	
	if(stock == NULL){
		ERROR(_moduleName, "PBUStatusChange错误，输入参数不能为NULL");
		return;
	}
	
	for(i = 0; i < GetShareMemCurrentStats()->nextPBUFirm; i++){
		pbuHead = GetPBUTableByStockNo(stock->stockNo, i);
		if(pbuHead == NULL){
			ERROR(_moduleName, "PBUStatusChange错误，搜索pbu索引信息失败");
			return;
		}
		if(pbuHead->pbuDataAdr == -1){
			continue;
		}
		pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
		
		//更改前交易时段是开盘集合竟价吗
		if(strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_OCALL, strlen(STOCK_TRADING_RESTRICTION_OCALL))==0){
			//去掉OA订单的统计	
			pbuData->orderBuyAmount -= pbuData->orderOABuyAmount;
			pbuData->orderBuyMoney -= pbuData->orderOABuyMoney;
			pbuData->orderSellAmount -= pbuData->orderOASellAmount;
			pbuData->orderSellMoney -= pbuData->orderOASellMoney;
			//去掉AU订单的统计
			pbuData->orderBuyAmount -= pbuData->orderAUBuyAmount;
			pbuData->orderBuyMoney -= pbuData->orderAUBuyMoney;
			pbuData->orderSellAmount -= pbuData->orderAUSellAmount;
			pbuData->orderSellMoney -= pbuData->orderAUSellMoney;
		}
		//更改前交易时段是盘中集合竟价吗
		else if(strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_ICALL, strlen(STOCK_TRADING_RESTRICTION_ICALL))==0){
			//去掉AU订单的统计
			pbuData->orderBuyAmount -= pbuData->orderAUBuyAmount;
			pbuData->orderBuyMoney -= pbuData->orderAUBuyMoney;
			pbuData->orderSellAmount -= pbuData->orderAUSellAmount;
			pbuData->orderSellMoney -= pbuData->orderAUSellMoney;
		}
		//更改前交易时段是订单簿平衡时段吗
		else if(strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_IOBB, strlen(STOCK_TRADING_RESTRICTION_IOBB))==0 || strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IPOBB,strlen(STOCK_TRADING_RESTRICTION_IPOBB))==0 || strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OOBB,strlen(STOCK_TRADING_RESTRICTION_OOBB))==0 || strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OPOBB,strlen(STOCK_TRADING_RESTRICTION_OPOBB))==0){
			//去掉SU订单的统计	
			pbuData->orderBuyAmount -= pbuData->orderSUBuyAmount;
			pbuData->orderBuyMoney -= pbuData->orderSUBuyMoney;
			pbuData->orderSellAmount -= pbuData->orderSUSellAmount;
			pbuData->orderSellMoney -= pbuData->orderSUSellMoney;
		}
		//更改前交易时段是固定价格交易时段吗
		else if(strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_FCALL, strlen(STOCK_TRADING_RESTRICTION_FCALL))==0){
			//去掉FP订单的统计
			pbuData->orderBuyAmount -= pbuData->orderFPBuyAmount;
			pbuData->orderBuyMoney -= pbuData->orderFPBuyMoney;
			pbuData->orderSellAmount -= pbuData->orderFPSellAmount;
			pbuData->orderSellMoney -= pbuData->orderFPSellMoney;
		}
		
		//新交易时段是开盘集合竟价吗
		if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL,strlen(STOCK_TRADING_RESTRICTION_OCALL))==0){
			//增加OA订单的统计
			pbuData->orderBuyAmount += pbuData->orderOABuyAmount;
			pbuData->orderBuyMoney += pbuData->orderOABuyMoney;
			pbuData->orderSellAmount += pbuData->orderOASellAmount;
			pbuData->orderSellMoney += pbuData->orderOASellMoney;
			//增加AU订单的统计
			pbuData->orderBuyAmount += pbuData->orderAUBuyAmount;
			pbuData->orderBuyMoney += pbuData->orderAUBuyMoney;
			pbuData->orderSellAmount += pbuData->orderAUSellAmount;
			pbuData->orderSellMoney += pbuData->orderAUSellMoney;
		}
		//新交易时段是盘中集合竟价吗
		else if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_ICALL,strlen(STOCK_TRADING_RESTRICTION_ICALL))==0){
			//增加AU订单的统计
			pbuData->orderBuyAmount += pbuData->orderAUBuyAmount;
			pbuData->orderBuyMoney += pbuData->orderAUBuyMoney;
			pbuData->orderSellAmount += pbuData->orderAUSellAmount;
			pbuData->orderSellMoney += pbuData->orderAUSellMoney;
		}
		//新交易时段是订单簿平衡时段吗
		else if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IOBB,strlen(STOCK_TRADING_RESTRICTION_IOBB))==0 || \
			strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IPOBB,strlen(STOCK_TRADING_RESTRICTION_IPOBB))==0 || \
			strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OOBB,strlen(STOCK_TRADING_RESTRICTION_OOBB))==0 || \
			strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OPOBB,strlen(STOCK_TRADING_RESTRICTION_OPOBB))==0){
			//增加SU订单的统计
			pbuData->orderBuyAmount += pbuData->orderSUBuyAmount;
			pbuData->orderBuyMoney += pbuData->orderSUBuyMoney;
			pbuData->orderSellAmount += pbuData->orderSUSellAmount;
			pbuData->orderSellMoney += pbuData->orderSUSellMoney;
		}
		//新交易时段是订固定价格竞价时段吗
		else if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_FCALL,strlen(STOCK_TRADING_RESTRICTION_FCALL))==0){
			//增加FP订单的统计
			pbuData->orderBuyAmount += pbuData->orderFPBuyAmount;
			pbuData->orderBuyMoney += pbuData->orderFPBuyMoney;
			pbuData->orderSellAmount += pbuData->orderFPSellAmount;
			pbuData->orderSellMoney += pbuData->orderFPSellMoney;
		}
		
	}
	return;
}


/*****************************************************************
** 函数名:      ResortPBUSortList
** 输 入:       stock
** 输 出:       无
** 功能描述:    PBU重新排名处理
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2007.3.6
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
ResortPBUSortList(STOCK_CS* stock){
	PBU_HEAD*		pbuHead = NULL;
	PBU_HEAD*		pbuHeadCmp = NULL;
	PBU_HEAD*		pbuHeadChange = NULL;
	PBU_DATA*		pbuData = NULL;
	PBU_DATA*		pbuDataCmp = NULL;
	int*			buySortList = NULL;
	int*			sellSortList = NULL;
	int*			sortInsert = NULL;
	int*			sortChange = NULL;
	int				begin = -1;
	int				end = -1;
	int				comp = -1;
	int				i = 0;
	
	if(stock == NULL){
		ERROR(_moduleName, "PBUStatusChange错误，输入参数不能为NULL");
		return;
	}
	
	//排名列表清空
	stock->lastBuyOrderSortOffset = -1;
	stock->lastSellOrderSortOffset = -1;
	//重新排名
	for(i = 0; i < GetShareMemCurrentStats()->nextPBUFirm; i++){
		pbuHead = GetPBUTableByStockNo(stock->stockNo, i);
		if(pbuHead == NULL){
			ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu索引信息失败");
			return;
		}
		if(pbuHead->pbuDataAdr == -1){
			continue;
		}
		pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
		
		//买排名处理
		//排名列表空
		if(stock->lastBuyOrderSortOffset == -1){
			buySortList = GetPBUBuySortByStockNo(stock->stockNo, 0);
			*buySortList = i;
			stock->lastBuyOrderSortOffset++;
		}
		else{
			//与第一名比较
			buySortList = GetPBUBuySortByStockNo(stock->stockNo, 0);
			pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *buySortList);
			if(pbuHeadCmp == NULL){
				ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu索引信息失败");
				return;
			}
			pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
			if(pbuDataCmp == NULL){
				ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu数据失败");
				return;
			}
			
			if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
				memmove(buySortList + 1, buySortList, sizeof(int)*(stock->lastBuyOrderSortOffset + 1));
				*buySortList = i;
				stock->lastBuyOrderSortOffset++;
			}
			else{
				//与最后一名比较
				buySortList = GetPBUBuySortByStockNo(stock->stockNo, stock->lastBuyOrderSortOffset);
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *buySortList);
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu索引信息失败");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu数据失败");
					return;
				}
				if(pbuData->orderBuyAmount < pbuDataCmp->orderBuyAmount){
					*(buySortList + 1) = i;
					stock->lastBuyOrderSortOffset++;
				}
				else{
					//二分法搜索要插入的位置(end是将要插入的位置，end后面的数据后移)
					begin = 0;
					end = stock->lastBuyOrderSortOffset;
					while(end - begin > 1){
						comp = (end + begin) / 2;
					
						buySortList = GetPBUBuySortByStockNo(stock->stockNo, comp);
						pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *buySortList);
						if(pbuHeadCmp == NULL){
							ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu索引信息失败");
							return;
						}
						pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
						
						if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
							end = comp;
						}
						else if(pbuData->orderBuyAmount <= pbuDataCmp->orderBuyAmount){
							begin = comp;
						}
					}
					
					sortInsert = GetPBUBuySortByStockNo(stock->stockNo, end);
					memmove(sortInsert + 1, sortInsert, sizeof(int) * (stock->lastBuyOrderSortOffset - end + 1));
					*sortInsert = i;
					stock->lastBuyOrderSortOffset++;
				}
			}
		}
		
		//卖排名处理
		//排名列表空
		if(stock->lastSellOrderSortOffset == -1){
			sellSortList = GetPBUSellSortByStockNo(stock->stockNo, 0);
			*sellSortList = i;
			stock->lastSellOrderSortOffset++;
		}
		else{
			//与第一名比较
			sellSortList = GetPBUSellSortByStockNo(stock->stockNo, 0);
			pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sellSortList);
			if(pbuHeadCmp == NULL){
				ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu索引信息失败");
				return;
			}
			pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
			if(pbuDataCmp == NULL){
				ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu数据失败");
				return;
			}
			
			if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
				memmove(sellSortList + 1, sellSortList, sizeof(int)*(stock->lastSellOrderSortOffset + 1));
				*sellSortList = i;
				stock->lastSellOrderSortOffset++;
			}
			else{
				//与最后一名比较
				sellSortList = GetPBUSellSortByStockNo(stock->stockNo, stock->lastSellOrderSortOffset);
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sellSortList);
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu索引信息失败");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu数据失败");
					return;
				}
				if(pbuData->orderSellAmount < pbuDataCmp->orderSellAmount){
					*(sellSortList + 1) = i;
					stock->lastSellOrderSortOffset++;
				}
				else{
					//二分法搜索要插入的位置(end是将要插入的位置，end后面的数据后移)
					begin = 0;
					end = stock->lastSellOrderSortOffset;
					while(end - begin > 1){
						comp = (end + begin) / 2;
					
						sellSortList = GetPBUSellSortByStockNo(stock->stockNo, comp);
						pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sellSortList);
						if(pbuHeadCmp == NULL){
							ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu索引信息失败");
							return;
						}
						pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
						
						if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
							end = comp;
						}
						else if(pbuData->orderSellAmount <= pbuDataCmp->orderSellAmount){
							begin = comp;
						}
					}
					
					sortInsert = GetPBUSellSortByStockNo(stock->stockNo, end);
					memmove(sortInsert + 1, sortInsert, sizeof(int) * (stock->lastSellOrderSortOffset - end + 1));
					*sortInsert = i;
					stock->lastSellOrderSortOffset++;
				}
			}
		}
	}
	//更新pbuHead中的排名信息
	for(i = 0; i <= stock->lastBuyOrderSortOffset; i++){
		sortChange = GetPBUBuySortByStockNo(stock->stockNo, i);
		pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
		if(pbuHeadChange == NULL){
			ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu索引信息失败");
			return;
		}
		pbuHeadChange->pbuBuyOrderSortNo = i;
	}
	for(i = 0; i <= stock->lastSellOrderSortOffset; i++){
		sortChange = GetPBUSellSortByStockNo(stock->stockNo, i);
		pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
		if(pbuHeadChange == NULL){
			ERROR(_moduleName, "ResortPBUSortList 错误，搜索pbu索引信息失败");
			return;
		}
		pbuHeadChange->pbuSellOrderSortNo = i;
	}
}



/*****************************************************************
** 函数名:      PrintPBUList
** 输 入:       stock
** 输 出:       无
** 功能描述:    输出pbu订单和成交排名到文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2007.3.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
PrintPBUList(){
	char        buf[2048];
	int     	fp = 0;
    char    	filePath[256];
    STOCK_CS*	stock = NULL;
    PBU_FIRM*	pbuFirm = NULL;
    PBU_HEAD*	pbuHead = NULL;
    PBU_DATA*	pbuData = NULL;
    int*		pbuBuyList = NULL;
    int*		pbuSellList = NULL;
    int			i = 0;
    int			j = 0;

	if (!IsHomeValid()) {
        return;
    }
    
    sprintf(filePath, "%s/PbuOrderList%s.txt", GetHome(), GetTime());
    
	if ((fp = open(filePath, O_WRONLY | O_CREAT | O_APPEND,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
        printf("ERROR: 创建文件[%s]失败! \n", filePath);
        return;
    }
    
    for (i = 0; i < GetStockNumber(); i++) {
    	stock = GetStock(i);
    	if(stock == NULL){
    		continue;
    	}
        memset(buf, 0, 2048);
        
        for(j = 0; j < 5; j++){
        	pbuBuyList = GetPBUBuySortByStockNo(stock->stockNo, j);
        	pbuHead = GetPBUTableByStockNo(stock->stockNo, *pbuBuyList);
        	if(pbuHead == NULL){
        		continue;
        	}
        	pbuFirm = GetPBUFirmByPbuNo(*pbuBuyList);
        	if(pbuFirm == NULL){
        		continue;
        	}
        	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
        	if(pbuData == NULL){
        		continue;
        	}
        	
        	sprintf(buf, "--buyOrderList=[%8d]--isinCod=[%12s]--pbuFirm=[%8s]--pbuBuyOrderAmount=[%16lld]--\n",
        		 j,
        		 stock->isinCod,
        		 pbuFirm->pbuFirm,
        		 pbuData->orderBuyAmount);
        	write(fp, buf, strlen(buf));
        }
        
        memset(buf, 0, 2048);
        for(j = 0; j < 5; j++){
        	pbuSellList = GetPBUSellSortByStockNo(stock->stockNo, j);
        	pbuHead = GetPBUTableByStockNo(stock->stockNo, *pbuSellList);
        	if(pbuHead == NULL){
        		continue;
        	}
        	pbuFirm = GetPBUFirmByPbuNo(*pbuSellList);
        	if(pbuFirm == NULL){
        		continue;
        	}
        	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
        	if(pbuData == NULL){
        		continue;
        	}
        	
        	sprintf(buf, "--sellOrderList=[%8d]--isinCod=[%12s]--pbuFirm=[%8s]--pbuBuyOrderAmount=[%16lld]--\n",
        		 j,
        		 stock->isinCod,
        		 pbuFirm->pbuFirm,
        		 pbuData->orderSellAmount);
        	write(fp, buf, strlen(buf));
        }
        
        memset(buf, 0, 2048);
        for(j = 0; j < PBU_TRADE_SORT_LENGTH; j++){
        	pbuHead = GetPBUTableByStockNo(stock->stockNo, stock->pbuTradeBuySort[j]);
        	if(pbuHead == NULL){
        		continue;
        	}
        	pbuFirm = GetPBUFirmByPbuNo(stock->pbuTradeBuySort[j]);
        	if(pbuFirm == NULL){
        		continue;
        	}
        	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
        	if(pbuData == NULL){
        		continue;
        	}
        	sprintf(buf, "--buyTradeList=[%8d]--isinCod=[%12s]--pbuFirm=[%8s]--tradeBuyAmount=[%16lld]--\n",
        		 j,
        		 stock->isinCod,
        		 pbuFirm->pbuFirm,
        		 pbuData->tradeBuyAmount);
        	write(fp, buf, strlen(buf));
        }
        
        memset(buf, 0, 2048);
        for(j = 0; j < PBU_TRADE_SORT_LENGTH; j++){
        	pbuHead = GetPBUTableByStockNo(stock->stockNo, stock->pbuTradeSellSort[j]);
        	if(pbuHead == NULL){
        		continue;
        	}
        	pbuFirm = GetPBUFirmByPbuNo(stock->pbuTradeSellSort[j]);
        	if(pbuFirm == NULL){
        		continue;
        	}
        	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
        	if(pbuData == NULL){
        		continue;
        	}
        	sprintf(buf, "--sellTradeList=[%8d]--isinCod=[%12s]--pbuFirm=[%8s]--tradeSellAmount=[%16lld]--\n",
        		 j,
        		 stock->isinCod,
        		 pbuFirm->pbuFirm,
        		 pbuData->tradeSellAmount);
        	write(fp, buf, strlen(buf));
        }
    }
    close(fp);
}
