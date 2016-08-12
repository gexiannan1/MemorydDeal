/******************************************************************
** 文件名:      withdraw.c
** 版 本:       0.1
** 创建人:      wugb
** 日 期:       2006.9.11
** 描 述:       Withdraw信息定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/withdraw.c,v 1.1 2012/03/29 01:19:55 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:55 $
 */


#include	<stdlib.h> 
#include	<stdio.h>
#include    <string.h>
#include    <fcntl.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include	<unistd.h>

#include    "withdraw.h"
#include    "model.h"
#include    "order.h"
#include    "stock.h"
#include	"price.h"
#include    "../memory.h"
#include    "../env/env.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"
#include    "../util/time/common_time.h"
#include    "../util/log/log.h"

/*
 * 模块名称
 */
static const char   _moduleName[] = "撤单处理";

static void UpdateWithdrawSortList(int *sortList, WITHDRAW_CS *withdraw, ORDER_CS *order);
/*****************************************************************
** 函数名:      GetWithdrawList
** 输 入:       无
** 输 出:       WITHDRAW_CS[]
** 功能描述:    返回Withdraw数组的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
WITHDRAW_CS*
GetWithdrawList() {
    return (WITHDRAW_CS*) GetShareMemory() + GetShareMemAllocationStats()->withdrawOffset;
}


/*****************************************************************
** 函数名:      GetWithdraw
** 输 入:       无
** 输 出:       WITHDRAW_CS*
** 功能描述:    返回指定位置的WITHDRAW_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
WITHDRAW_CS*
GetWithdraw(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->withdrawMax) {
        return NULL;
    }
    return GetWithdrawList() + index;
}


/*****************************************************************
** 函数名:      GetWithdrawNumber
** 输 入:       无
** 输 出:       Withdraw总数
** 功能描述:    返回Withdraw数量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetWithdrawNumber() {
    return GetShareMemCurrentStats()->nextWithdraw;
}


/*****************************************************************
** 函数名:      GetLastWithdrawTime
** 输 入:       无
** 输 出:       撤单时间 (hhmmss)
** 功能描述:    返回最后一笔撤单时间
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetLastWithdrawTime() {
    int index = GetShareMemCurrentStats()->nextWithdraw;
    if (index > 0) {
        return GetWithdraw(index - 1)->orderTime;
    }
    return 0;
}


/*****************************************************************
** 函数名:      AddWithdraw
** 输 入:       无
** 输 出:       新增的Withdraw的位置
** 功能描述:    向Withdraw数组中增加Withdraw条目
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
AddWithdraw(ORDER_CHANGE_TE *teWithdraw) {
    WITHDRAW_CS*	target = NULL;
    ORDER_CS*		order = NULL;
    int         	index   = GetShareMemCurrentStats()->nextWithdraw;
	
    if (index >= GetShareMemConfigStats()->withdrawMax) {
        FATAL("Withdraw.AddWithdraw()", "撤单数量超出预定义的最大撤单数量！ Current Number: %d"
                , index);
        return -1;
    }

    target = GetWithdraw(index);
    
    target->orderNo				= teWithdraw->orderNo;
    target->orderNoOld			= teWithdraw->orderNoOld;
    target->orderNoOld1			= teWithdraw->orderNoOld1;
    target->orderProcessNo	  	= teWithdraw->orderProcessNo;
    target->orderProcessNoOld	= teWithdraw->orderProcessNoOld;
    target->orderProcessNoOld1	= teWithdraw->orderProcessNoOld1;
    target->orderTime			= teWithdraw->orderTime;
	target->ordrBuyCod			= teWithdraw->orderBSType;
	target->ordrTypCod			= teWithdraw->orderType;
	target->price				= teWithdraw->orderPrice / ONE_HUNDRAD;
	target->qty					= teWithdraw->orderQty;
	target->remPeakQty			= teWithdraw->remPeakQty;
	target->orderStatus			= teWithdraw->orderStatus;
	target->peakSizeQty			= teWithdraw->peakSizeQty;
	target->trnTypId			= teWithdraw->trnTypId;
	memcpy(target->isinCod, teWithdraw->isinCod, STOCK_ISIN_CODE_LENGTH);
	memcpy(target->pbuId, teWithdraw->pbuId, PBU_FIRM_LENGTH);

    target->nextStock = NULL_INDEX;
    if(teWithdraw->orderProcessNo<0)
	{
		ERROR(_moduleName, "撤单处理失败! teWithdraw->orderProcessNo[%d]",teWithdraw->orderProcessNo);
	}
    order = GetOrder(target->orderProcessNo);
    if(order == NULL){
    	GetShareMemCurrentStats()->nextWithdraw++;
    	return -1;
    }
    target->stockNo = order->stockNo;
	target->withdrawProcessNo = index;
	
    GetShareMemCurrentStats()->nextWithdraw++;
    
    return index;
}


/*****************************************************************
** 函数名:      GetWorkedWithdrawNumber
** 输 入:       无
** 输 出:       已处理过的撤单总数
** 功能描述:    返回当前已处理过的撤单数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetWorkedWithdrawNumber() {
    return GetMemTradeStats()->workedWithdrawNumber;
}


/*****************************************************************
** 函数名:      SetWorkedWithdrawNumber
** 输 入:       当前已处理过的撤单数
** 输 出:       无
** 功能描述:    设置当前已处理过的撤单数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetWorkedWithdrawNumber(int workedNumber) {
    GetMemTradeStats()->workedWithdrawNumber = workedNumber;
}


/*****************************************************************
** 函数名:      IncreaseWorkedWithdrawNumber
** 输 入:       无
** 输 出:       无
** 功能描述:    递增当前已处理过的撤单数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
IncreaseWorkedWithdrawNumber() {
    GetMemTradeStats()->workedWithdrawNumber++;
}


/*****************************************************************
** 函数名:      CancelOrder
** 输 入:       WITHDRAW_CS *
** 输 出:       BOOL
** 功能描述:    数据加工-取消订单
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL 
CancelOrder(WITHDRAW_CS* csWithdraw){
	#ifdef USE_PBU
		int			pbuNo = -1;
		PBU_HEAD*	pbuHead = NULL;
		PBU_DATA*	pbuData = NULL;
	#endif
	STOCK_CS	*stock		= NULL;
	ORDER_CS    *order		= NULL;
	
	stock = GetStock(csWithdraw->stockNo);
	if(stock == NULL){
		ERROR(_moduleName, "获取股票信息失败");
		return FALSE;
	}
	
	//判断是否为报价订单
	if(csWithdraw->ordrTypCod == 'Q')
	{
		order = GetOrder(csWithdraw->orderProcessNoOld);
	}else{
		order = GetOrder(csWithdraw->orderProcessNo);
	}
		
	//从订单簿中删除订单
	if(order == NULL){
		ERROR(_moduleName, "获取订单信息失败");
		return FALSE;
	}
	
	if(!DetachOrder(order)){
    	ERROR(_moduleName, "把订单从订单簿中删除失败! 位置order->orderNo[%d]",order->orderNo);
    	return FALSE;
    }
    StockStatDel(order, order->balance);
    
    #ifdef USE_PBU
		pbuNo = GetPBUNoByFirm(order->pbuId);
		if(pbuNo < 0){
			ERROR(_moduleName, "更新pbu委托信息失败，未找到对应的pbu号码");
			return;
		}
		
		pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
		if(pbuHead == NULL){
			ERROR(_moduleName, "更新pbu委托信息失败，未找到对应的pbu索引号");
			return;
		}
		
		pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
		if(pbuData == NULL){
			ERROR(_moduleName, "更新pbu委托信息失败，未找到对应的pbu数据");
			return;
		}
		
		//pbu统计
		PBUOrderStatDel(stock, pbuData, order, order->balance);
		//维护pbu委托排名
		UpdatePBUOrderSortList(stock, pbuNo, order);
	#endif
    
    //为了防止发送订单的剩余数量为0，取消订单后，原有订单的剩余数量不再设置为0，改为设置订单状态为撤单状态
    //order->balance = 0;
    order->status = ORDER_STATUS_WITHDRAWN;
    
    TRACE(_moduleName, "[STOCK] stockNo=[%-4d]--orderBuyNumber=[%-4d]--orderBuyAmount=[%-16lld]--orderSellNumber=[%-4d]--orderSellAmount=[%-16lld]",
			stock->stockNo,
			stock->orderBuyNumber,
			stock->orderBuyAmount,
			stock->orderSellNumber,
			stock->orderSellAmount
         );
    
    return TRUE;
}

/*****************************************************************
** 函数名:      ModifyOrder
** 输 入:       WITHDRAW_CS *, ORDER_CS *
** 输 出:       BOOL
** 功能描述:    数据加工-修改订单
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL 
ModifyOrder(WITHDRAW_CS * csWithdraw){
	#ifdef USE_PBU
		int			pbuNo = -1;
		PBU_HEAD*	pbuHead = NULL;
		PBU_DATA*	pbuData = NULL;
	#endif
	STOCK_CS	*stock                  = NULL;
	ORDER_CS    *order                  = NULL;
	BID_CS		*bid					= NULL;
	OFFER_CS	*offer					= NULL;
	
	//根据订单处理号查找对应的订单
	order = GetOrder(csWithdraw->orderProcessNo);
	stock = GetStock(csWithdraw->stockNo);
	if(stock == NULL){
		ERROR(_moduleName, "获取股票信息失败");
		return FALSE;
	}
	if(order == NULL){
		ERROR(_moduleName, "获取订单信息失败");
		return FALSE;
	}
	
	#ifdef USE_PBU
		pbuNo = GetPBUNoByFirm(order->pbuId);
		if(pbuNo < 0){
			ERROR(_moduleName, "更新pbu委托信息失败，未找到对应的pbu号码");
			return;
		}
		
		pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
		if(pbuHead == NULL){
			ERROR(_moduleName, "更新pbu委托信息失败，未找到对应的pbu索引号");
			return;
		}
		
		pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
		if(pbuData == NULL){
			ERROR(_moduleName, "更新pbu委托信息失败，未找到对应的pbu数据");
			return;
		}
	#endif
	
	//判断是否为冻结,Y为冻结，N为正常
	if(csWithdraw->orderStatus == 'N'){
		if(order->balance != csWithdraw->qty){
			//修改订单数量，在证券信息里扣减委托数量为（原委托数量-修改数量）
			if(!IsIceOrder(order)){
				if(IsBuyWithdraw(csWithdraw)){
					if((bid = GetBid(order->priceLevel)) == NULL){
						ERROR(_moduleName,"获取买价位队列失败");
						return FALSE;
					}
					//从统计信息中减去未修改前的订单信息
					if(IsTradingPhase(order)){
						UpdateStockDel(stock, order);
						bid->orderAmount -= order->balance;
					}
					StockStatDel(order, order->balance);
					BidStatDel(bid, order, order->balance);
					
					#ifdef USE_PBU
						//pbu统计
						PBUOrderStatDel(stock, pbuData, order, order->balance);
					#endif
					
					//更新撤单累计数量
					stock->withdrawBuyAmount += (order->balance - csWithdraw->qty);
					stock->withdrawBuyMoney += (stock->lotSize * (order->balance - csWithdraw->qty) / TEN_THOUSAND * order->orderPrice);
					//修改订单数量
					order->orderQty += (csWithdraw->qty - order->balance);
					order->balance = csWithdraw->qty;
					//增加修改后的订单到统计信息中
					if(IsTradingPhase(order)){
						UpdateStockAdd(stock, order);
						bid->orderAmount += order->balance;
					}
					StockStatAdd(order);
					BidStatAdd(bid, order);
					
					#ifdef USE_PBU
						//pbu统计
						PBUOrderStatAdd(stock, pbuData, order);
						//维护pbu委托排名
						UpdatePBUOrderSortList(stock, pbuNo, order);
					#endif
				}
				else if(IsSellWithdraw(csWithdraw)){
					if((offer = GetOffer(order->priceLevel)) == NULL){
						ERROR(_moduleName,"获取卖价位队列失败");
						return FALSE;
					}
					//从统计信息中减去未修改前的订单信息
					if(IsTradingPhase(order)){
						UpdateStockDel(stock, order);
						offer->orderAmount -= order->balance;
					}
					StockStatDel(order, order->balance);
					OfferStatDel(offer, order, order->balance);
					
					#ifdef USE_PBU
						//pbu统计
						PBUOrderStatDel(stock, pbuData, order, order->balance);
					#endif
					
					//更新撤单累计数量
					stock->withdrawSellAmount += (order->orderQty - csWithdraw->qty);
					stock->withdrawSellMoney += (stock->lotSize * (order->balance - csWithdraw->qty) / TEN_THOUSAND * order->orderPrice);
					//修改订单数量
					order->orderQty += (csWithdraw->qty - order->balance);
					order->balance = csWithdraw->qty;
					//增加修改后的订单到统计信息中
					if(IsTradingPhase(order)){
						UpdateStockAdd(stock, order);
						offer->orderAmount += order->balance;
					}
					StockStatAdd(order);
					OfferStatAdd(offer, order);
					
					#ifdef USE_PBU
						//pbu统计
						PBUOrderStatAdd(stock, pbuData, order);
						//维护pbu委托排名
						UpdatePBUOrderSortList(stock, pbuNo, order);
					#endif
				}
			}
			else{
				//如果是冰山订单，不需要修改任何统计信息，因为冰山订单的揭示数量不能修改
				TRACE(_moduleName, "处理撤单，冰山订单，");
	    		order->orderQty += (csWithdraw->qty - order->balance);
				order->balance = csWithdraw->qty;
			}
		}

		if(strcmp(csWithdraw->trdResTypCod, order->trdResTypCod) != 0){
			//判断原订单参与产品交易时段吗
			if(IsTradingPhase(order)){
				//删除Order交易限制类型统计信息
				StockStatDel(order, order->balance);
			  	
			  	#ifdef USE_PBU
					//pbu统计
					PBUOrderStatDel(stock, pbuData, order, order->balance);
				#endif
			  	
			  	//修改订单交易时段限制
				strcpy(order->trdResTypCod, csWithdraw->trdResTypCod);
				
				//新增Order交易限制类型统计信息
				StockStatAdd(order);
				
				#ifdef USE_PBU
					//pbu统计
					PBUOrderStatAdd(stock, pbuData, order);
					//维护pbu委托排名
					UpdatePBUOrderSortList(stock, pbuNo, order);
				#endif
				
				//修改后订单还可以参加当前交易时段吗
				if(!IsTradingPhase(order)) {
					//更新证券统计信息，减少订单笔数和订单数量
					UpdateStockDel(stock, order);
					//更新价位队列的交易时段统计信息
					if(IsBuyOrder(order)){
						if((bid = GetBid(order->priceLevel)) == NULL){
							ERROR(_moduleName,"获取买价位队列失败");
							return FALSE;
						}
						BidStatDel(bid, order, order->balance);
					}
					else if(IsSellOrder(order)){
						if((offer = GetOffer(order->priceLevel)) == NULL){
							ERROR(_moduleName,"获取卖价位队列失败");
							return FALSE;
						}
						OfferStatDel(offer, order, order->balance);
					}
				}
				
			}
			else{
				//删除Order交易限制类型统计信息
				StockStatDel(order, order->balance);
			  	
			  	#ifdef USE_PBU
					//pbu统计
					PBUOrderStatDel(stock, pbuData, order, order->balance);
				#endif
			  	
			  	//修改订单交易时段限制
				strcpy(order->trdResTypCod, csWithdraw->trdResTypCod);
				
				//新增Order交易限制类型统计信息
				StockStatAdd(order);
			    
			    #ifdef USE_PBU
					//pbu统计
					PBUOrderStatAdd(stock, pbuData, order);
					//维护pbu委托排名
					UpdatePBUOrderSortList(stock, pbuNo, order);
				#endif
			    
			    //修改后订单还可以参加当前交易时段吗
				if(IsTradingPhase(order)) {
					//更新证券统计信息，增加订单笔数和订单数量
					UpdateStockAdd(stock, order);
					//更新价位队列的交易时段统计信息
					if(IsBuyOrder(order)){
						if((bid = GetBid(order->priceLevel)) == NULL){
							ERROR(_moduleName,"获取买价位队列失败");
							return FALSE;
						}
						BidStatAdd(bid, order);
					}
					else if(IsSellOrder(order)){
						if((offer = GetOffer(order->priceLevel)) == NULL){
							ERROR(_moduleName,"获取卖价位队列失败");
							return FALSE;
						}
						OfferStatAdd(offer, order);
					}
				}
			}
		}
		
		//判断是否为冰山订单，最高揭示数量和原订单不同修改订单的最高揭示数量
		if(IsIceOrder(order)){
			if(csWithdraw->peakSizeQty != order->peakSizeQty){
				TRACE(_moduleName, "处理撤单，修改冰山订单最高揭示数量，原最高揭示数量[%lld]，新最高揭示数量[%lld]",order->peakSizeQty,csWithdraw->peakSizeQty);
				order->peakSizeQty = csWithdraw->peakSizeQty;
			}
		}
	}
	else if(csWithdraw->orderStatus=='Y'){
		//从订单簿中删除订单
		TRACE(_moduleName, "冻结订单处理开始orderNo=[%d]",order->orderNo);
		if(!DetachOrder(order)){
    		ERROR(_moduleName, "把订单从订单簿中删除失败! 位置order->orderNo[%d]",order->orderNo);
    		return FALSE;
    	}
    	StockStatDel(order, order->balance);
    	
    	#ifdef USE_PBU
			//pbu统计
			PBUOrderStatDel(stock, pbuData, order, order->balance);
			//维护pbu委托排名
			UpdatePBUOrderSortList(stock, pbuNo, order);
		#endif
    	
    	TRACE(_moduleName, "冻结订单处理结束orderNo=[%d]",order->orderNo);
	}
	
	PrintOrderCS(order);
	///////////////////////
	PrintStock(stock);
	///////////////////////
	return TRUE;
}
/*****************************************************************
** 函数名:      DeleteOrder
** 输 入:       WITHDRAW_CS *
** 输 出:       BOOL
** 功能描述:    数据加工-删除订单
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL 
DeleteOrder(WITHDRAW_CS * csWithdraw){
	#ifdef USE_PBU
		int			pbuNo = -1;
		PBU_HEAD*	pbuHead = NULL;
		PBU_DATA*	pbuData = NULL;
	#endif
	STOCK_CS	*stock                  = NULL;
	ORDER_CS    *order                  = NULL;
	
	//根据订单处理号查找对应的订单
	order = GetOrder(csWithdraw->orderProcessNo);
	stock = GetStock(csWithdraw->stockNo);
    if(stock == NULL){
		ERROR(_moduleName, "获取股票信息失败");
		return FALSE;
	}
	if(order == NULL){
		ERROR(_moduleName, "获取订单信息失败");
		return FALSE;
	}
    
    //更新证券信息中的撤单队列
    UpdateStockSort(csWithdraw);
    
    //更新撤单排名
    if(stock->categoryNo == CATEGORY_ID_ASHARE)
    {
    	if(IsBuyWithdraw(csWithdraw)){
    		UpdateWithdrawSortList(GetAShareWithdrawBuySortList()->withdrawNo,csWithdraw,order);
    	}
	 	else if(IsSellWithdraw(csWithdraw)){
	 		UpdateWithdrawSortList(GetAShareWithdrawSellSortList()->withdrawNo,csWithdraw,order);
	 	}
    }
    
    if(stock->categoryNo == CATEGORY_ID_BSHARE)
    {
    	if(IsBuyWithdraw(csWithdraw)){
    		UpdateWithdrawSortList(GetBShareWithdrawBuySortList()->withdrawNo,csWithdraw,order);
    	}
	 	else if(IsSellWithdraw(csWithdraw)){
	 		UpdateWithdrawSortList(GetBShareWithdrawSellSortList()->withdrawNo,csWithdraw,order);
	 	}
    }

	//在证券信息里扣减委托数量为（原委托数量-修改数量），增加撤单笔数、撤单数量
	if(IsBuyWithdraw(csWithdraw)){
		stock->withdrawBuyNumber++;
		if(IsIceOrder(order)){
			TRACE(_moduleName, "开始处理冰山订单撤单，原买入撤单数量[%lld]",stock->withdrawBuyAmount);
			stock->withdrawBuyAmount += csWithdraw->remPeakQty;
			stock->withdrawBuyMoney += (stock->lotSize * csWithdraw->remPeakQty / TEN_THOUSAND * order->orderPrice);
			TRACE(_moduleName, "处理冰山订单撤单结束，增加撤单统计数量，现买入撤单数量[%lld]，冰山订单揭示数量[%lld]",stock->withdrawBuyAmount,csWithdraw->remPeakQty);
		}
		else{
			stock->withdrawBuyAmount += csWithdraw->qty;
			stock->withdrawBuyMoney += (stock->lotSize * csWithdraw->qty / TEN_THOUSAND * order->orderPrice);
		}
	}
	else if(IsSellWithdraw(csWithdraw)){
		stock->withdrawSellNumber++;
		if(IsIceOrder(order)){
			TRACE(_moduleName, "开始处理冰山订单撤单，原卖出撤单数量[%lld]",stock->withdrawSellAmount);
			stock->withdrawSellAmount += csWithdraw->remPeakQty;
			stock->withdrawSellMoney += (stock->lotSize * csWithdraw->remPeakQty / TEN_THOUSAND * order->orderPrice);
			TRACE(_moduleName, "处理冰山订单撤单结束，增加撤单统计数量，现卖出撤单数量[%lld]，冰山订单揭示数量[%lld]",stock->withdrawSellAmount,csWithdraw->remPeakQty);
		}
		else{
			stock->withdrawSellAmount += csWithdraw->qty;
			stock->withdrawSellMoney += (stock->lotSize * csWithdraw->qty / TEN_THOUSAND * order->orderPrice);
		}
	}
	
	//从订单簿中删除订单
	if(!DetachOrder(order)){
    	ERROR(_moduleName, "把订单从订单簿中删除失败! 位置order->orderNo[%d]",order->orderNo);
    	return FALSE;
    }
    //从交易时段统计中减去撤单数量
    StockStatDel(order, order->balance);
	
	#ifdef USE_PBU
		pbuNo = GetPBUNoByFirm(order->pbuId);
		if(pbuNo < 0){
			ERROR(_moduleName, "更新pbu委托信息失败，未找到对应的pbu号码");
			return;
		}
		
		pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
		if(pbuHead == NULL){
			ERROR(_moduleName, "更新pbu委托信息失败，未找到对应的pbu索引号");
			return;
		}
		
		pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
		if(pbuData == NULL){
			ERROR(_moduleName, "更新pbu委托信息失败，未找到对应的pbu数据");
			return;
		}
		
		//pbu统计
		PBUOrderStatDel(stock, pbuData, order, order->balance);
		//维护pbu委托排名
		UpdatePBUOrderSortList(stock, pbuNo, order);
	#endif
	
	//为了防止发送订单的剩余数量为0，从订单队列中删除订单后，原有订单的剩余数量不再设置为0，改为设置订单状态为撤单状态
    //order->balance = 0;
    order->status = ORDER_STATUS_WITHDRAWN;
	
	///////////////////////
	PrintStock(stock);
	///////////////////////
	return TRUE;
}

/*****************************************************************
** 函数名:      UpdateStockSort
** 输 入:       WITHDRAW_CS *, ORDER_CS *, STOCK_CS	*
** 输 出:       无
** 功能描述:    更新Stock撤单队列
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void UpdateStockSort(WITHDRAW_CS * withdraw){
    int         lastWithdraw    = -1;
    STOCK_CS	*stock                  = NULL;
    
    stock = GetStock(withdraw->stockNo);
    if(stock == NULL){
		ERROR(_moduleName, "获取股票信息失败");
		return;
	}
    //更新stock、withdraw中的撤单指针
    if (stock->firstWithdraw < 0) {
        stock->firstWithdraw = stock->lastWithdraw = withdraw->withdrawProcessNo;
    } else {
        lastWithdraw = stock->lastWithdraw;
        stock->lastWithdraw = withdraw->withdrawProcessNo;
        GetWithdraw(lastWithdraw)->nextStock = withdraw->withdrawProcessNo;
    }
}

/*****************************************************************
** 函数名:      IsBuyWithdraw
** 输 入:       WITHDRAW_CS*
** 输 出:       BOOL
** 功能描述:    返回指定的委托是否是买入委托
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL 
IsBuyWithdraw(WITHDRAW_CS * csWithdraw) {
    return csWithdraw->ordrBuyCod == ORDER_TYPE_BUY;
}


/*****************************************************************
** 函数名:      IsSellWithdraw
** 输 入:       WITHDRAW_CS*
** 输 出:       BOOL
** 功能描述:    返回指定的委托是否是卖出委托
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL 
IsSellWithdraw(WITHDRAW_CS * csWithdraw) {
    return csWithdraw->ordrBuyCod == ORDER_TYPE_SELL;
}

/*****************************************************************
** 函数名:      UpdateWithdrawSortList
** 输 入:
** 输 出:       无
** 功能描述:    更新单笔委托买入/卖出撤单排名
**              （单笔委托数量最大的10笔委买/委卖撤单）
** 全局变量:    无
** 调用模块:    DeleteOrder
** 作 者:       Baoyi Li
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
UpdateWithdrawSortList(int *sortList, WITHDRAW_CS *withdraw, ORDER_CS *order) {
    int         i               = 0;
    int         begin           = -1;
    int         end             = -1;
    WITHDRAW_CS *tmpWithdraw    = NULL;
    ORDER_CS    *tmpOrder       = NULL;
    
    //如果订单已执行数量大于0，那么这笔订单的撤单不能参加单笔撤单排名
    if(order->orderQty - order->balance > 0){
    	return;
    }
	
    for (i = 0; i < WITHDRAW_SORT_LENGTH; i++) {
        if (sortList[i] < 0) {
            if (begin < 0) {
                sortList[i] = withdraw->withdrawProcessNo;
                return;
            } else {
                end = i;
                break;
            }
        }

        tmpWithdraw = GetWithdraw(sortList[i]);
        tmpOrder 	= GetOrder(tmpWithdraw->orderProcessNo);
		
        if (begin < 0 && order->orderQty > tmpOrder->orderQty) {
            begin = i;
        }

        if (withdraw->orderNo == tmpWithdraw->orderNo) {
            end = i;
            break;
        }
    }

    if (begin < 0) {
        return;
    }

    if (end < 0) {
        end = WITHDRAW_SORT_LENGTH - 1;
    }

    for (i = end; i > begin; i--) {
        sortList[i] = sortList[i - 1];
    }

    sortList[begin] = withdraw->withdrawProcessNo;
}


/*****************************************************************
** 函数名:      PrintWithdrawToFile
** 输 入:       
** 输 出:       无
** 功能描述:    打印撤单信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:		Li Bo
** 日 期:		2006.11.1
** 版本:        0.1
****************************************************************/
BOOL
PrintWithdrawToFile(){
    char			buf[2048];
	int				fp = 0;
    char			filePath[256];
    char			stockType = 0;
    WITHDRAW_CS* 	withdraw = NULL;
    STOCK_CS*		stock = NULL;
    int				i = 0;

	if (!IsHomeValid()) {
        return FALSE;
    }
    
    sprintf(filePath, "%s/WithdrawSnapshot%s.txt", GetHome(), GetTime());
    
	if ((fp = open(filePath, O_WRONLY | O_CREAT | O_APPEND,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
        printf("ERROR: 创建文件[%s]失败! \n", filePath);
        return FALSE;
    }
    
    for (i = 0; i < GetWithdrawNumber(); i++) {
    	withdraw = GetWithdraw(i);
    	memset(buf, 0, 2048);
        stock = GetStock(withdraw->stockNo);
        if(stock == NULL){
        	continue;
        }
        if(IsAShare(stock)){
        	stockType = 'A';
        }
        else if(IsBShare(stock)){
        	stockType = 'B';
        }
        else{
        	continue;
        }
        sprintf(buf,"%12s|%15lld|%c|%15d|%c|%15lld|\n",
        	withdraw->isinCod,
        	withdraw->orderNo,
        	stockType,
        	withdraw->orderTime,
        	withdraw->ordrBuyCod,
        	withdraw->qty
        	);
        write(fp, buf, strlen(buf));
    }
    close(fp);
    return TRUE;
}



/*****************************************************************
** 函数名:      PrintWithdrawToFile
** 输 入:       文件描述符
** 输 出:       无
** 功能描述:    打印撤单信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.11.30
** 修 改:		
** 日 期:		
** 版本:        0.1
****************************************************************/
void
PrintWithdrawInfoToFile(int fp){
	char			buf[2048];
    WITHDRAW_CS* 	withdraw = NULL;
    int         	number  = 0;
    int         	i       = 0;

    number = GetWithdrawNumber();

    sprintf(buf, "#****撤单信息****\n#撤单总数: %d\n\n", number);
    write(fp, buf, strlen(buf));

    for (i = 0; i < number; i++) {
        withdraw = GetWithdraw(i);
		
		memset(buf, 0, 2048);

        sprintf(buf, "orderNo=[%16llu]--orderNoOld=[%16llu]--orderNoOld1=[%16llu]--orderProcessNo=[%9d]--orderProcessNoOld=[%9d]--orderProcessNoOld1=[%9d]--orderTime=[%9d]--isinCod=[%12s]--pbuId=[%8s]--ordrBuyCod=[%c]--ordrTypCod=[%c]--price=[%16lld]--qty=[%16lld]--trdResTypCod=[%3s]--remPeakQty=[%16lld]--orderStatus=[%c]--peakSizeQty=[%16lld]--trnTypId=[%c]--withdrawProcessNo=[%9d]--\n",
        	withdraw->orderNo,
        	withdraw->orderNoOld,
        	withdraw->orderNoOld1,
        	withdraw->orderProcessNo,
        	withdraw->orderProcessNoOld,
        	withdraw->orderProcessNoOld1,
        	withdraw->orderTime,
        	withdraw->isinCod,
        	withdraw->pbuId,
        	withdraw->ordrBuyCod,
        	withdraw->ordrTypCod,
        	withdraw->price,
        	withdraw->qty,
        	withdraw->trdResTypCod,
        	withdraw->remPeakQty,
        	withdraw->orderStatus,
        	withdraw->peakSizeQty,
        	withdraw->trnTypId,
        	withdraw->withdrawProcessNo);

        write(fp, buf, strlen(buf));
    }
}
