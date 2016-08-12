/******************************************************************
** 文件名:      trade.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.9.11
** 描 述:       Trade信息定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
** 2006.03.23   Wei Huanyin 1、IsValidTrade函数返回值由char改为int
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/trade.c,v 1.1 2012/03/29 01:19:54 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:54 $
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>

#include    "trade.h"
#include    "model.h"
#include    "../memory.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"
#include    "../util/log/log.h"

/*
 * 模块名称
 */
static const char   _moduleName[] = "trade_cs";

/*****************************************************************
** 函数名:      GetTradeList
** 输 入:       无
** 输 出:       TRADE_CS[]
** 功能描述:    返回Trade数组的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
TRADE_CS*
GetTradeList() {
    return (TRADE_CS*) GetShareMemory() + GetShareMemAllocationStats()->tradeOffset;
}


/*****************************************************************
** 函数名:      GetTrade
** 输 入:       无
** 输 出:       TRADE_CS*
** 功能描述:    返回指定位置的TRADE_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
TRADE_CS*
GetTrade(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->tradeMax) {
        return NULL;
    }
    return GetTradeList() + index;
}


/*****************************************************************
** 函数名:      GetTradeNumber
** 输 入:       无
** 输 出:       Trade总数
** 功能描述:    返回Trade数量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetTradeNumber() {
    return GetShareMemCurrentStats()->nextTrade;
}


/*****************************************************************
** 函数名:      GetLastTrade
** 输 入:       无
** 输 出:       TRADE_CS*
** 功能描述:    返回最后一笔成交
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.10.18
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
TRADE_CS*
GetLastTrade() {
    return GetTrade(GetTradeNumber() - 1);
}


/*****************************************************************
** 函数名:      AddTrade
** 输 入:       无
** 输 出:       新增的Trade的位置
** 功能描述:    向Trade数组中增加Trade条目
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
AddTrade(TRADE_TE *teTrade) {
	STOCK_CS*	stock = NULL;
    TRADE_CS*	target = NULL;
    ORDER_CS*	order = NULL;
    int index	= GetShareMemCurrentStats()->nextTrade;
    
    /*
     * 检查内存是否越界
     */
    if (index >= GetShareMemConfigStats()->tradeMax) {
        ERROR(_moduleName,"AddTrade成交数量超出预定义的最大成交数量");
        return -1;
    }

    target = GetTrade(index);

	memcpy(target->isinCod, teTrade->isinCod, STOCK_ISIN_CODE_LENGTH);
	memcpy(target->pbuId, teTrade->pbuId, PBU_FIRM_LENGTH);
    target->tradeNo			= teTrade->tradeNo;
    target->tradeTime		= teTrade->tradeTime;
    target->orderProcessNo	= teTrade->orderProcessNo;
    target->orderNo			= teTrade->orderNo;
    target->orderType		= teTrade->orderType;
    target->price			= teTrade->price / ONE_HUNDRAD;
    target->qty				= teTrade->qty;
    target->mktVal			= teTrade->mktVal;
    target->type			= teTrade->type;
    target->trnTypId		= teTrade->trnTypId;
    target->tradeBSType		= teTrade->tradeBSType;
    target->tradeBSFlag		= ' ';
    target->remPeakQty		= teTrade->remPeakQty;
    target->peakSizeQty		= teTrade->peakSizeQty;
    target->ordrEntTim		= teTrade->ordrEntTim;

	target->tradeProcessNo	= index;
	if(target->orderProcessNo == -1){
		stock = GetStockByIsinCod(teTrade->isinCod);
		if(stock == NULL){
	    	return -1;
	    }
		target->stockNo = stock->stockNo;
	}
	else{
		order = GetOrder(target->orderProcessNo);
		if(order == NULL){
			return -1;
		}
		target->stockNo = order->stockNo;
	}
	
    target->index = target->nextStock = NULL_INDEX;

    GetShareMemCurrentStats()->nextTrade++;
    return index;
}


/*****************************************************************
** 函数名:      GetWorkedTradeNumber
** 输 入:       无
** 输 出:       已处理过的成交总数
** 功能描述:    返回当前已处理过的成交数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetWorkedTradeNumber() {
    return GetMemTradeStats()->workedTradeNumber;
}


/*****************************************************************
** 函数名:      SetWorkedTradeNumber
** 输 入:       当前已处理过的成交数
** 输 出:       无
** 功能描述:    设置当前已处理过的成交数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetWorkedTradeNumber(int workedNumber) {
    GetMemTradeStats()->workedTradeNumber = workedNumber;
}


/*****************************************************************
** 函数名:      IncreaseWorkedTradeNumber
** 输 入:       无
** 输 出:       无
** 功能描述:    递增当前已处理过的成交数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
IncreaseWorkedTradeNumber() {
    GetMemTradeStats()->workedTradeNumber++;
}


/*****************************************************************
** 函数名:      GetSentTradeNumber
** 输 入:       无
** 输 出:       已处理过的成交总数
** 功能描述:    返回当前已发送的成交数量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.25
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetSentTradeNumber() {
    return GetMemTradeStats()->sentTradeNumber;
}


/*****************************************************************
** 函数名:      SetSentTradeNumber
** 输 入:       当前已处理过的成交数
** 输 出:       无
** 功能描述:    设置当前已发送的成交数量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.25
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetSentTradeNumber(int sentTradeNumber) {
    GetMemTradeStats()->sentTradeNumber = sentTradeNumber;
}


/*****************************************************************
** 函数名:      IncreaseWorkedTradeNumber
** 输 入:       无
** 输 出:       无
** 功能描述:    递增当前已发送的成交数量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.25
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
IncreaseSentTradeNumber() {
    GetMemTradeStats()->sentTradeNumber++;
}


/*****************************************************************
** 函数名:      IsPendingVendTrade
** 输 入:       TRADE_CS*
** 输 出:       BOOL
** 功能描述:    返回指定的成交是否需要加工
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.16
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsPendingTrade(TRADE_CS *trade) {
    if (! trade || trade->type != TRADE_TYPE_NORMAL_TRD
            || trade->price <= 0 || trade->qty <= 0) {
        return FALSE;
    }
    return TRUE;
}


/*****************************************************************
** 函数名:      IsBuyTrade
** 输 入:       TRADE_CS*
** 输 出:       BOOL
** 功能描述:    返回是否是买类型成交
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL 
IsBuyTrade(TRADE_CS* tradeCS){
	return tradeCS->tradeBSType == 'B';
}


/*****************************************************************
** 函数名:      IsSellTrade
** 输 入:       TRADE_CS*
** 输 出:       BOOL
** 功能描述:    返回是否是卖类型成交
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL 
IsSellTrade(TRADE_CS* tradeCS){
	return tradeCS->tradeBSType == 'S';
}


/*****************************************************************
** 函数名:      IsMarketPriceTrade
** 输 入:       TRADE_CS*
** 输 出:       BOOL
** 功能描述:    返回是否是市价委托成交
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2010.5.4
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL 
IsMarketPriceTrade(TRADE_CS* tradeCS){
	return tradeCS->orderType == 'M';
}



/*****************************************************************
** 函数名:      IsValidTrade
** 输 入:       TRADE_CS: 成交信息
** 输 出:       无
** 功能描述:    判断该笔成交数据是否是符合条件的数据，如符合条件返回'0'，否则返回'1'
** 全局变量:
** 调用模块:    无
** 作 者:       weihy
** 日 期:       2005.2.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
IsValidTrade(TRADE_CS *trade) {
    if (IsPendingTrade(trade)) {
        return VALID_TRADE;
    } else {
        return INVALID_TRADE;
    }
}


/*****************************************************************
** 函数名:      PrintTradeInfoToFile
** 输 入:       文件描述符
** 输 出:       无
** 功能描述:    打印成交信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
PrintTradeInfoToFile(int fp){
    TRADE_CS    *trade      = NULL;
    char        buf[1024];
    int         number      = 0;
    int         i           = 0;

    number = GetTradeNumber();

    sprintf(buf, "#****成交信息****\n#成交总数: %d\n\n", number);
    write(fp, buf, strlen(buf));

    for (i = 0; i < number; i++) {
        trade = GetTrade(i);

		sprintf(buf, "[TRADE] isinCod=[%-12s]--pbuId=[%-8s]--tradeNo=[%-16lld]--tradeProcessNo=[%-9d]--tradeTime=[%-8d]--orderNo=[%lld]--orderProcessNo=[%-9d]--orderType=[%c]--tradePrice=[%-16lld]--tradeQty=[%-16lld]--mktVal=[%-16lld]--type=[%c]--tradeBSType=[%c]--tradeBSFlag=[%c]--remPeakQty=[%-13lld]--peakSizeQty=[%-13lld]--ordrEntTim=[%-8ld]\n",
				trade->isinCod,
				trade->pbuId,
				trade->tradeNo,
				trade->tradeProcessNo,
				trade->tradeTime,
				trade->orderNo,
				trade->orderProcessNo,
				trade->orderType,
				trade->price,
				trade->qty,
				trade->mktVal,
				trade->type,
				trade->tradeBSType,
				trade->tradeBSFlag,
				trade->remPeakQty,
				trade->peakSizeQty,
				trade->ordrEntTim);
                
        write(fp, buf, strlen(buf));
    }
}
