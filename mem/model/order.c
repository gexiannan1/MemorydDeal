/******************************************************************
** 文件名:      order.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.9.11
** 描 述:       Order信息定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/order.c,v 1.1 2012/03/29 01:19:53 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:53 $
 */


#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>

#include    "order.h"
#include    "model.h"
#include    "../memory.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"
#include    "../util/log/log.h"

/*
 * 模块名称
 */
static const char   _moduleName[] = "order_cs";


/*
 * 内部函数声明
 */

/*****************************************************************
** 函数名:      GetOrderList
** 输 入:       无
** 输 出:       ORDER_CS[]
** 功能描述:    返回Order数组的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
ORDER_CS*
GetOrderList() {
    return (ORDER_CS*) GetShareMemory() + GetShareMemAllocationStats()->orderOffset;
}


/*****************************************************************
** 函数名:      GetOrder
** 输 入:       无
** 输 出:       ORDER_CS*
** 功能描述:    返回指定位置的ORDER_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1.1
****************************************************************/
ORDER_CS*
GetOrder(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->orderMax) {
        return NULL;
    }
    return GetOrderList() + index;
}


/*****************************************************************
** 函数名:      GetOrderNumber
** 输 入:       无
** 输 出:       Order总数
** 功能描述:    返回Order数量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetOrderNumber() {
    return GetShareMemCurrentStats()->nextOrder;
}


/*****************************************************************
** 函数名:      PushOrder
** 输 入:       无
** 输 出:       新增的Order的位置
** 功能描述:    向Order数组中增加Order条目
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
PushOrder(ORDER_CS *order) {
    ORDER_CS    *target = NULL;
    STOCK_CS	*stock  = NULL;
    int         index   = GetShareMemCurrentStats()->nextOrder;

    //检查内存是否越界
    if (index >= GetShareMemConfigStats()->orderMax) {  
      ///printf("error index=%lld ordermax=%lld\n", index, GetShareMemConfigStats()->orderMax);
        ERROR("Order.PushOrder()", "添加订单orderNo=[%lld]时失败，订单数量超出预定义的最大订单数量！ Current Number: %d"
                , order->orderNo, index);
        return -1;
    }
	//printf("index=%lld ordermax=%lld\n", index, GetShareMemConfigStats()->orderMax);
	//如果orderProcessNo不在order队列尾，返回错误。
    if(order->orderProcessNo < 0){
    	ERROR("Order.PushOrder()", "添加订单orderNo=[%lld]时失败，订单处理号不能小于0", order->orderNo);
    	return -1;
    }
    target = GetOrder(index);

    target->orderNo     = order->orderNo;
    target->orderNoOld			= order->orderNoOld;
    target->orderNoOld1			= order->orderNoOld1;
    target->orderProcessNo		= order->orderProcessNo;
    target->orderProcessNoOld	= order->orderProcessNoOld;
    target->orderProcessNoOld1	= order->orderProcessNoOld1;
    target->orderTime			= order->orderTime;
    target->orderBSType			= order->orderBSType;
    target->orderType			= order->orderType;
    target->orderPrice			= order->orderPrice / ONE_HUNDRAD;
 //   target->orderQty			= order->orderQty + order->ordExeQty;
    target->trnTypId			= order->trnTypId;
    target->orderStatus			= order->orderStatus;
    target->remPeakQty			= order->remPeakQty;
    target->peakSizeQty			= order->peakSizeQty;
    target->ordrResCod			= order->ordrResCod;
    memcpy(target->isinCod, order->isinCod, STOCK_ISIN_CODE_LENGTH);
    memcpy(target->pbuId, order->pbuId, PBU_FIRM_LENGTH);
    memcpy(target->trdResTypCod, order->trdResTypCod, 3);

    //初始化Order
    target->balance = order->orderQty;
    target->status = ORDER_STATUS_OPEN;
    
    stock = GetStockByIsinCod(order->isinCod);

    if(stock == NULL){
      target->stockNo = NULL_INDEX;
      GetShareMemCurrentStats()->nextOrder++;
      ERROR(_moduleName, "未找到订单orderNo=[%lld]对应的证券isinCod=[%12s]", order->orderNo,order->isinCod);
      return -1;
    }

    target->stockNo = stock->stockNo;
    memcpy(target->isinCod, stock->isinCod, STOCK_ISIN_CODE_LENGTH);
    
    target->nextStock = target->priceLevel = \
        target->prevPrice = target->nextPrice = NULL_INDEX;

    GetShareMemCurrentStats()->nextOrder++;
    return index;
}


/*****************************************************************
** 函数名:      AttachOrder
** 输 入:       需要添加的order
** 输 出:       TRUE FALSE
** 功能描述:    把order条目添加到订单簿中
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL 
AttachOrder(ORDER_CS* pOrderCS){
	STOCK_CS*	pStockCS	= NULL;
	
	if(pOrderCS == NULL){
		ERROR(_moduleName, "AttachOrder输入参数错误，不能为NULL");
		return FALSE;
	}
	
	//如果订单剩余数量为0，不需要加入到价格队列
	if(pOrderCS->balance == 0){
		return TRUE;
	}
	
	//添加到价格队列
	if(IsBuyOrder(pOrderCS)){
		UpdateBidByOrder(pOrderCS);
	}
	else if(IsSellOrder(pOrderCS)){
		UpdateOfferByOrder(pOrderCS);
	}

	//更新统计信息
	if(IsTradingPhase(pOrderCS)){
		pStockCS = GetStock(pOrderCS->stockNo);
		if(pStockCS == NULL){
			ERROR(_moduleName, "AttachOrder 获取证券信息失败");
			return FALSE;
		}
		UpdateStockAdd(pStockCS, pOrderCS);
	}
	
	return TRUE;
}


/*****************************************************************
** 函数名:      DetachOrder
** 输 入:       需要脱离的order
** 输 出:       TRUE FALSE
** 功能描述:    从订单簿中删除order条目（把order从链表中脱离）
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL 
DetachOrder(ORDER_CS* pOrderCS){
	STOCK_CS*	pStockCS	= NULL;
	
	if(pOrderCS == NULL){
		ERROR(_moduleName, "DetachOrder 输入参数错误，不能为NULL");
		return FALSE;
	}
	
	//处理价格队列
	if(IsBuyOrder(pOrderCS)){
		if(IsIceOrder(pOrderCS)){
			UpdateBidByWithdraw(pOrderCS, pOrderCS->remPeakQty);
		}
		else{
			UpdateBidByWithdraw(pOrderCS, pOrderCS->balance);
		}
	}
	else if(IsSellOrder(pOrderCS)){
		if(IsIceOrder(pOrderCS)){
			//UpdateOfferByWithdraw(pOrderCS, pOrderCS->remPeakQty);
		}
		else{
		//	UpdateOfferByWithdraw(pOrderCS, pOrderCS->balance);
		}
	}
	else{
	}
	
	//更新统计信息
	if(IsTradingPhase(pOrderCS)){
		pStockCS = GetStock(pOrderCS->stockNo);
		if(pStockCS == NULL){
			ERROR(_moduleName, "DetachOrder 获取股票信息失败");
			return FALSE;
		}
		UpdateStockDel(pStockCS, pOrderCS);
	}
	
	return TRUE;
}


/*****************************************************************
** 函数名:      StockStatAdd
** 输 入:       ORDER_CS*
** 输 出:       无
** 功能描述:    把订单信息添加到交易时段统计信息
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.27
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
StockStatAdd(ORDER_CS* pOrderCS){
	STOCK_CS* stockCS = NULL;
	int		  bsType = 0; 
	int		  trdType = 0;
	int		  i = 0;
	
	if(pOrderCS == NULL){
		ERROR(_moduleName, "StockStatAdd 输入参数不能为NULL");
		return ;
	}
	
	stockCS = GetStock(pOrderCS->stockNo);
	if(stockCS == NULL){
		ERROR(_moduleName, "StockStatAdd 获取股票信息失败");
		return;
	}
	
	//确定买卖标记，买为0 卖为1
	if(IsBuyOrder(pOrderCS)){
		bsType = 0;
	}
	else if(IsSellOrder(pOrderCS)){
		bsType = 1;
	}
	
	//确定交易时段标记
	if(strcmp(pOrderCS->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
		trdType = 0;
	}
	else if(strcmp(pOrderCS->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
		trdType = 2;
	}
	else if(strcmp(pOrderCS->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
		trdType = 4;
	}
	else if(strcmp(pOrderCS->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
		trdType = 6;
	}
	else{
		//其他类型不需要统计
		return;
	}
	
	//需要改变的统计信息下标i的获得
	i = bsType + trdType;
	
	//把订单信息添加到交易时段统计信息
	stockCS->tradePhaseStat[i].number++;
	stockCS->tradePhaseStat[i].amount += pOrderCS->balance;
	stockCS->tradePhaseStat[i].money += (stockCS->lotSize * pOrderCS->balance / TEN_THOUSAND * pOrderCS->orderPrice);
	
}


/*****************************************************************
** 函数名:      StockStatDel
** 输 入:       ORDER_CS* amount为需要删除的数量
** 输 出:       无
** 功能描述:    从交易时段统计删除Order信息
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.27
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
StockStatDel(ORDER_CS* pOrderCS, long long amount){
	STOCK_CS* stockCS = NULL;
	int		  bsType = 0; 
	int		  trdType = 0;
	int		  i = 0;
	
	if(pOrderCS == NULL){
		ERROR(_moduleName, "StockStatDel 输入参数不能为NULL");
		return ;
	}
	
	stockCS = GetStock(pOrderCS->stockNo);
	if(stockCS == NULL){
		ERROR(_moduleName, "StockStatDel 获取股票信息失败");
		return;
	}
	
	//确定买卖标记，买为0 卖为1
	if(IsBuyOrder(pOrderCS)){
		bsType = 0;
	}
	else if(IsSellOrder(pOrderCS)){
		bsType = 1;
	}
	
	//确定交易时段标记
	if(strcmp(pOrderCS->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
		trdType = 0;
	}
	else if(strcmp(pOrderCS->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
		trdType = 2;
	}
	else if(strcmp(pOrderCS->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
		trdType = 4;
	}
	else if(strcmp(pOrderCS->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
		trdType = 6;
	}
	else{
		//其他类型不需要统计
		return;
	}
	
	//需要改变的统计信息下标i的获得
	i = bsType + trdType;
	
	//从交易时段统计信息中删除订单信息
	stockCS->tradePhaseStat[i].amount -= amount;
	stockCS->tradePhaseStat[i].money -= (stockCS->lotSize * amount / TEN_THOUSAND * pOrderCS->orderPrice);
	if(pOrderCS->balance - amount == 0){
		stockCS->tradePhaseStat[i].number--;
	}
}


/*****************************************************************
** 函数名:      UpdateStockAdd
** 输 入:       STOCK_CS* pStockCS, ORDER_CS* pOrderCS
** 输 出:       无
** 功能描述:    更新证券信息，增加对应订单的统计
** 全局变量:    无
** 调用模块:    无
** 作 者:       wugb
** 日 期:       2006.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
UpdateStockAdd(STOCK_CS* pStockCS, ORDER_CS* pOrderCS){
	if(IsBuyOrder(pOrderCS)){
		pStockCS->orderBuyNumber++;
		if(IsIceOrder(pOrderCS)){
			TRACE(_moduleName,"开始更新证券信息(增加)，买入总数量[%lld]",pStockCS->orderBuyAmount);
			pStockCS->orderBuyAmount += pOrderCS->remPeakQty;
			pStockCS->orderBuyMoney += (pStockCS->lotSize * pOrderCS->remPeakQty / TEN_THOUSAND * pOrderCS->orderPrice);
			TRACE(_moduleName,"更新证券信息(增加)结束，冰山订单揭示数量[%lld]，买入总数量[%lld]",pOrderCS->remPeakQty,pStockCS->orderBuyAmount);
		}
		else{
			pStockCS->orderBuyAmount += pOrderCS->balance;
			pStockCS->orderBuyMoney += (pStockCS->lotSize * pOrderCS->balance / TEN_THOUSAND * pOrderCS->orderPrice);
		}
		
	}
	else if(IsSellOrder(pOrderCS)){
		pStockCS->orderSellNumber++;
		if(IsIceOrder(pOrderCS)){
			TRACE(_moduleName,"开始更新证券信息(增加)，卖出总数量[%lld]",pStockCS->orderBuyAmount);
			pStockCS->orderSellAmount += pOrderCS->remPeakQty;
			pStockCS->orderSellMoney += (pStockCS->lotSize * pOrderCS->remPeakQty / TEN_THOUSAND * pOrderCS->orderPrice);
			TRACE(_moduleName,"更新证券信息(增加)结束，冰山订单揭示数量[%lld]，卖出总数量[%lld]",pOrderCS->remPeakQty,pStockCS->orderBuyAmount);
		}
		else{
			pStockCS->orderSellAmount += pOrderCS->balance;
			pStockCS->orderSellMoney += (pStockCS->lotSize * pOrderCS->balance / TEN_THOUSAND * pOrderCS->orderPrice);
		}
	}
	else{
	}
}

/*****************************************************************
** 函数名:      UpdateStockDel
** 输 入:       STOCK_CS* pStockCS, ORDER_CS* pOrderCS
** 输 出:       无
** 功能描述:    更新证券信息，去掉对应订单的统计
** 全局变量:    无
** 调用模块:    无
** 作 者:       wugb
** 日 期:       2006.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
UpdateStockDel(STOCK_CS* pStockCS, ORDER_CS* pOrderCS){
	if(IsBuyOrder(pOrderCS)){
		pStockCS->orderBuyNumber--;
		if(IsIceOrder(pOrderCS)){
			TRACE(_moduleName,"开始更新证券信息(减少)，买入总数量[%lld]",pStockCS->orderBuyAmount);
			pStockCS->orderBuyAmount -= pOrderCS->remPeakQty;
			pStockCS->orderBuyMoney -= (pStockCS->lotSize * pOrderCS->remPeakQty / TEN_THOUSAND * pOrderCS->orderPrice);
			TRACE(_moduleName,"更新证券信息(减少)结束，冰山订单揭示数量[%lld]，买入总数量[%lld]",pOrderCS->remPeakQty,pStockCS->orderBuyAmount);
		}
		else{
			pStockCS->orderBuyAmount -= pOrderCS->balance;
			pStockCS->orderBuyMoney -= (pStockCS->lotSize * pOrderCS->balance / TEN_THOUSAND * pOrderCS->orderPrice);
		}
	}
	else if(IsSellOrder(pOrderCS)){
		pStockCS->orderSellNumber--;
		if(IsIceOrder(pOrderCS)){
			TRACE(_moduleName,"开始更新证券信息(减少)，卖出总数量[%lld]",pStockCS->orderBuyAmount);
			pStockCS->orderSellAmount -= pOrderCS->remPeakQty;
			pStockCS->orderSellMoney -= (pStockCS->lotSize * pOrderCS->remPeakQty / TEN_THOUSAND * pOrderCS->orderPrice);
			TRACE(_moduleName,"更新证券信息(减少)结束，冰山订单揭示数量[%lld]，卖出总数量[%lld]",pOrderCS->remPeakQty,pStockCS->orderBuyAmount);
		}
		else{
			pStockCS->orderSellAmount -= pOrderCS->balance;
			pStockCS->orderSellMoney -= (pStockCS->lotSize * pOrderCS->balance / TEN_THOUSAND * pOrderCS->orderPrice);
		}
	}
	else{
	}
}


/*****************************************************************
** 函数名:      IsTradingPhase
** 输 入:       ORDER_CS *order
** 输 出:       BOOL
** 功能描述:    返回Order是否参加产品交易时段
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.9.9
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsTradingPhase(ORDER_CS *order) {
	STOCK_CS	*stock                  = NULL;
	
	stock = GetStock(order->stockNo);
	//只参加开盘集合竞价
	if(strcmp(order->trdResTypCod,ORDER_TRADING_RESTRICTION_OA)==0){
		//OCALL - 开市集合竞价
		if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL,strlen(STOCK_TRADING_RESTRICTION_OCALL))==0){
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	//集合竞价期间有效
	if(strcmp(order->trdResTypCod,ORDER_TRADING_RESTRICTION_AU)==0){
		//OCALL - 开市集合竞价 ,ICALL - 盘中集合竞价
		if((strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_ICALL,strlen(STOCK_TRADING_RESTRICTION_ICALL))==0) || (strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL,strlen(STOCK_TRADING_RESTRICTION_OCALL))==0)){
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	//在订单薄平衡时段输入
	if(strcmp(order->trdResTypCod,ORDER_TRADING_RESTRICTION_SU)==0){
		//IOBB - 盘中订单簿平衡，IPOBB - 盘中订单簿平衡前期，OOBB  - 开市订单簿平衡，OPOBB - 开市订单簿平衡前期	
		if((strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IOBB,strlen(STOCK_TRADING_RESTRICTION_IOBB))==0) || (strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IPOBB,strlen(STOCK_TRADING_RESTRICTION_IPOBB))==0) || (strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OOBB,strlen(STOCK_TRADING_RESTRICTION_OOBB))==0) || (strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OPOBB,strlen(STOCK_TRADING_RESTRICTION_OPOBB))==0)){
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	
	//在固定价格时段有效
	if(strcmp(order->trdResTypCod,ORDER_TRADING_RESTRICTION_FP)==0){
		//FCALL - 固定价格集合竞价
		if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_FCALL,strlen(STOCK_TRADING_RESTRICTION_FCALL))==0){
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	
	//无限制
	if(strcmp(order->trdResTypCod,ORDER_TRADING_RESTRICTION_SPACE)==0){ 
		return TRUE;
	}
	return TRUE;
} 



/*****************************************************************
** 函数名:      IsIceOrder
** 输 入:       orderCS
** 输 出:       TRUE FALSE
** 功能描述:    判断订单是否为冰山订单
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.23
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL	
IsIceOrder(ORDER_CS* orderCS){
	return orderCS->orderType == 'I';
}



/*****************************************************************
** 函数名:      GetWorkedOrderNumber
** 输 入:       无
** 输 出:       已处理过的委托总数
** 功能描述:    返回当前已处理过的委托数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetWorkedOrderNumber() {
    return GetMemTradeStats()->workedOrderNumber;
}


/*****************************************************************
** 函数名:      SetWorkedOrderNumber
** 输 入:       当前已处理过的委托数
** 输 出:       无
** 功能描述:    设置当前已处理过的委托数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetWorkedOrderNumber(int workedNumber) {
    GetMemTradeStats()->workedOrderNumber = workedNumber;
}


/*****************************************************************
** 函数名:      IncreaseWorkedOrderNumber
** 输 入:       无
** 输 出:       无
** 功能描述:    递增当前已处理过的委托数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
IncreaseWorkedOrderNumber() {
    GetMemTradeStats()->workedOrderNumber++;
}

/*****************************************************************
** 函数名:      IsBuyOrder
** 输 入:       ORDER_CS*
** 输 出:       BOOL
** 功能描述:    返回指定的委托是否是买入委托
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.19
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsBuyOrder(ORDER_CS *order) {
    return order->orderBSType == 1;
}


/*****************************************************************
** 函数名:      IsSellOrder
** 输 入:       ORDER_CS*
** 输 出:       BOOL
** 功能描述:    返回指定的委托是否是卖出委托
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.19
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsSellOrder(ORDER_CS *order) {
    return order->orderBSType ==  2;
}


/*****************************************************************
** 函数名:      IsOrderOutmoded
** 输 入:       ORDER_CS*
** 输 出:       BOOL
** 功能描述:    返回委托是否已过时（如已全部成交或撤单）
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2006.3.2
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsOrderOutmoded(ORDER_CS *order) {
    return order->balance <= 0;
}


/*****************************************************************
** 函数名:      IsOrderProcessComplete
** 输 入:       无
** 输 出:       BOOL
** 功能描述:    返回委托是否已全部处理完毕
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2006.3.2
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsOrderProcessComplete() {
    return GetWorkedOrderNumber() >= GetOrderNumber();
}


/*****************************************************************
** 函数名:      PrintOrderCS
** 输 入:       orderCS
** 输 出:       无
** 功能描述:    输出order到本地文件
** 全局变量:    无
** 调用模块:    
** 作 者:       Li Bo
** 日 期:       2006.10.12
** 修 改:		
** 日 期:		
** 版本:        0.1
****************************************************************/
void	
PrintOrderCS(ORDER_CS* orderCS){
	if (orderCS == NULL){
		TRACE(_moduleName,"orderTe is NULL!!!");
		return;
	}
	
	TRACE(_moduleName,"[ORDER_CS] orderNo=[%-16lld]--orderNoOld=[%-16lld]--orderNoOld1=[%-16lld]--orderProcessNo=[%-9d]--orderProcessNoOld=[%-9d]--orderProcessNoOld1=[%-9d]--orderTime=[%-8d]--isinCod=[%-12s]--pbuId=[%-5s]--orderBSType=[%c]--orderType=[%c]--orderPrice=[%-14lld]--orderQty=[%-13lld]--trdResTypCod=[%-2s]--trnTypId=[%c]--orderStatus=[%c]--remPeakQty=[%-13lld]--peakSizeQty=[%-13lld]--ordrResCod=[%c]--balance=[%-13lld]",
		 orderCS->orderNo,
		 orderCS->orderNoOld,
		 orderCS->orderNoOld1,
		 orderCS->orderProcessNo,
		 orderCS->orderProcessNoOld,
		 orderCS->orderProcessNoOld1,
		 orderCS->orderTime,
		 orderCS->isinCod,
		 orderCS->pbuId,
		 orderCS->orderBSType,
		 orderCS->orderType,
		 orderCS->orderPrice,
		 orderCS->orderQty,
		 orderCS->trdResTypCod,
		 orderCS->trnTypId,
		 orderCS->orderStatus,
		 orderCS->remPeakQty,
		 orderCS->peakSizeQty,
		 orderCS->ordrResCod,
		 orderCS->balance
         );
}


/*****************************************************************
** 函数名:      PrintOrderInfoToFile
** 输 入:       文件描述符
** 输 出:       无
** 功能描述:    打印委托信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
PrintOrderInfoToFile(int fp){
    ORDER_CS    *orderCS  = NULL;
    char        buf[1024];
    int         number  = 0;
    int         i       = 0;

    number = GetOrderNumber();

    sprintf(buf, "#****委托信息****\n#委托总数: %d\n\n", number);
    write(fp, buf, strlen(buf));

    for (i = 0; i < number; i++) {
        orderCS = GetOrder(i);

        sprintf(buf, "[ORDER_CS] orderNo=[%-16lld]--orderNoOld=[%-16lld]--orderNoOld1=[%-16lld]--orderProcessNo=[%-9d]--orderProcessNoOld=[%-9d]--orderProcessNoOld1=[%-9d]--orderTime=[%-8d]--isinCod=[%-12s]--pbuId=[%-5s]--orderBSType=[%c]--orderType=[%c]--orderPrice=[%-14lld]--orderQty=[%-13lld]--trdResTypCod=[%-2s]--trnTypId=[%c]--orderStatus=[%c]--remPeakQty=[%-13lld]--peakSizeQty=[%-13lld]--ordrResCod=[%c]\n",
		 orderCS->orderNo,
		 orderCS->orderNoOld,
		 orderCS->orderNoOld1,
		 orderCS->orderProcessNo,
		 orderCS->orderProcessNoOld,
		 orderCS->orderProcessNoOld1,
		 orderCS->orderTime,
		 orderCS->isinCod,
		 orderCS->pbuId,
		 orderCS->orderBSType,
		 orderCS->orderType,
		 orderCS->orderPrice,
		 orderCS->orderQty,
		 orderCS->trdResTypCod,
		 orderCS->trnTypId,
		 orderCS->orderStatus,
		 orderCS->remPeakQty,
		 orderCS->peakSizeQty,
		 orderCS->ordrResCod);

        write(fp, buf, strlen(buf));
    }
}
