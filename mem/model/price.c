/******************************************************************
** 文件名:      price.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.9.11
** 描 述:       价位信息定义
** 修订记录：
** 修订日期     修订人   SIR    修复的bug或者修订原因
**-----------------------------------------------------------------
** 2005.12.12   Song Fang       修改UpdateBidByWithdraw()与UpdateOfferByWithdraw()函数。
**                              对应的“问题汇总”中的问题序号为[数据生成子系统]->[32]。
** 2013.09.27   LIBO     00811  卖价位队列循环使用时判断条件错误
**                              
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/price.c,v 1.1 2012/03/29 01:19:53 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:53 $
 */


#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <fcntl.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <unistd.h>

#include    "price.h"
#include    "model.h"
#include	"stock.h"
#include    "../memory.h"
#include    "../env/env.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"
#include    "../util/time/common_time.h"
#include    "../util/log/log.h"

static const char _moduleName[] = "price.c";

/*
 * 内部函数声明
 */
static int  NextBid();                              /* 返回下一可用的空闲Bid位置 */
static int  NextOffer();                            /* 返回下一可用的空闲Offer位置 */
static void InitBid(BID_CS*);                       /* 初始化指定位置的BID_CS */
static void InitOffer(OFFER_CS*);                   /* 初始化指定位置的OFFER_CS */

static void UpdateBidNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int bidIndex);
static void InsertBidNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int nextBidIndex);
static void InsertLastBidNodeByOrder(STOCK_CS *stock, ORDER_CS *order);
static int  AddNewBidNode(STOCK_CS *stock, ORDER_CS *order);

static void UpdateOfferNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int offerIndex);
static void InsertOfferNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int nextBidIndex);
static void InsertLastOfferNodeByOrder(STOCK_CS *stock, ORDER_CS *order);
static int  AddNewOfferNode(STOCK_CS *stock, ORDER_CS *order);

static int	GetBidOrderTotalNum(BID_CS*,char*);
static int	GetOfferOrderTotalNum(OFFER_CS*,char*);
/*****************************************************************
** 函数名:      GetBidList
** 输 入:       无
** 输 出:       BID_CS[]
** 功能描述:    返回委买价位列表的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BID_CS*
GetBidList() {
    return (BID_CS*) GetShareMemory() + GetShareMemAllocationStats()->bidOffset;
}


/*****************************************************************
** 函数名:      GetOfferList
** 输 入:       无
** 输 出:       OFFER_CS[]
** 功能描述:    返回委卖价位列表的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
OFFER_CS*
GetOfferList() {
    return (OFFER_CS*) GetShareMemory() + GetShareMemAllocationStats()->offerOffset;
}


/*****************************************************************
** 函数名:      GetStockBidList
** 输 入:       stockNo - 股票列表入口号
** 输 出:       BID_CS[]
** 功能描述:    返回指定股票的委买价位列表
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BID_CS*
GetStockBidList(int stockNo) {
    return (BID_CS*) GetShareMemory() + GetShareMemAllocationStats()->bidOffset
            + GetStock(stockNo)->highestBid;
}


/*****************************************************************
** 函数名:      GetStockOfferList
** 输 入:       stockNo - 股票列表入口号
** 输 出:       OFFER_CS[]
** 功能描述:    返回指定股票的委卖价位列表
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
OFFER_CS*
GetStockOfferList(int stockNo) {
    return (OFFER_CS*) GetShareMemory() + GetShareMemAllocationStats()->offerOffset
            + GetStock(stockNo)->lowestOffer;
}


/*****************************************************************
** 函数名:      GetBid
** 输 入:       无
** 输 出:       BID_CS*
** 功能描述:    返回指定位置的BID_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BID_CS*
GetBid(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->bidMax) {
        return NULL;
    }
    return GetBidList() + index;
}


/*****************************************************************
** 函数名:      GetOffer
** 输 入:       无
** 输 出:       OFFER_CS*
** 功能描述:    返回指定位置的OFFER_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
OFFER_CS*
GetOffer(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->offerMax) {
        return NULL;
    }
    return GetOfferList() + index;
}


/*****************************************************************
** 函数名:      GetBidNumber
** 输 入:       无
** 输 出:       Bid总数（包含已丢弃的空闲位置）
** 功能描述:    返回Bid总数（包含已丢弃的空闲位置）
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetBidNumber() {
    return GetShareMemCurrentStats()->nextBid;
}


/*****************************************************************
** 函数名:      GetOfferNumber
** 输 入:       无
** 输 出:       Offer总数（包含已丢弃的空闲位置）
** 功能描述:    返回Offer总数（包含已丢弃的空闲位置）
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetOfferNumber() {
    return GetShareMemCurrentStats()->nextOffer;
}


/*****************************************************************
** 函数名:      UpdateBidByOrder
** 输 入:       无
** 输 出:       无
** 功能描述:    委托加工处理时委买价位处理
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
UpdateBidByOrder(ORDER_CS *order) {
    STOCK_CS    *stock      = NULL;
    BID_CS      *bid        = NULL;
    int         bidIndex    = -1;

    if ((order == NULL) || ((stock = GetStock(order->stockNo)) == NULL)) {
        return;
    }

    bidIndex = stock->highestBid;
    while (bidIndex >= 0) {
        bid = GetBid(bidIndex);
        if (bid->price == order->orderPrice) {
            /*
             * 更新价位
             */
         UpdateBidNodeByOrder(stock, order, bidIndex);
            return;
        }

        if (bid->price < order->orderPrice) {
            /*
             * 插入新价位
             */
            InsertBidNodeByOrder(stock, order, bidIndex);
            return;
        }

        bidIndex = bid->next;
    }
    /*
     * 增加新价位
     */
    InsertLastBidNodeByOrder(stock, order);
}


/*****************************************************************
** 函数名:      UpdateOfferByOrder
** 输 入:       无
** 输 出:       无
** 功能描述:    委托加工处理时委卖价位处理
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
UpdateOfferByOrder(ORDER_CS *order) {
    STOCK_CS    *stock      = NULL;
    OFFER_CS    *offer      = NULL;
    int         offerIndex  = -1;

    if (! order || ! (stock = GetStock(order->stockNo))) {
        return;
    }

    offerIndex = stock->lowestOffer;

    while (offerIndex >= 0) {
        offer = GetOffer(offerIndex);

        if (offer->price == order->orderPrice) {
            /*
             * 更新价位
             */
            UpdateOfferNodeByOrder(stock, order, offerIndex);
            return;
        }

        if (offer->price > order->orderPrice) {
            /*
             * 插入新价位
             */
            InsertOfferNodeByOrder(stock, order, offerIndex);
            return;
        }

        offerIndex = offer->next;
    }

    /*
     * 增加新价位
     */
    InsertLastOfferNodeByOrder(stock, order);
}


/*****************************************************************
** 函数名:      UpdateBidByWithdraw
** 输 入:       无
** 输 出:       无
** 功能描述:    成交、撤单加工处理时的委买价位处理
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:		Li Bo
** 日 期:		2006.9.27
** 版本:        0.1
****************************************************************/
void
UpdateBidByWithdraw(ORDER_CS *order, long long amount) {
    STOCK_CS    *stock      = NULL;
    BID_CS      *bid        = NULL;
    int			orderNum	= 0;
    int         bidIndex    = -1;

    if (! order || ! (stock = GetStock(order->stockNo))) {
        return;
    }

    bidIndex = order->priceLevel;
    if (! (bid = GetBid(bidIndex))) {
        return;
    }
	//更新交易时段统计
	BidStatDel(bid, order, amount);
	
    //更新价位信息
    if(IsTradingPhase(order)){
    	bid->orderAmount -= amount;
    }

    //如果订单剩余数量为0，或者是冰山订单揭示数量为0，从价位队列中删除
 /*   if ((order->balance - amount == 0) || (IsIceOrder(order) && (order->remPeakQty - amount == 0))){
        if(IsTradingPhase(order)){
	    	bid->orderNumber --;
	    }

        //更新同价位委托列表
        if (order->prevPrice >= 0) {
            GetOrder(order->prevPrice)->nextPrice = order->nextPrice;
        } else {
            bid->firstOrder = order->nextPrice;
        }
        
        if (order->nextPrice >= 0) {
            GetOrder(order->nextPrice)->prevPrice = order->prevPrice;
        } else {
        	bid->lastOrder = order->prevPrice;
        }
        
        order->prevPrice = \
        		order->nextPrice = \
        		order->priceLevel = NULL_INDEX;
    }*/

	orderNum = GetBidOrderTotalNum(bid, stock->prcsStsValCod);
	
    //删除价位节点
    if (orderNum == 0) {
        stock->bidNumber --;

        //更新价位列表
        if (bid->prev >= 0) {
            GetBid(bid->prev)->next = bid->next;
        } else {
        	stock->highestBid = bid->next;
        }

        if (bid->next >= 0) {
            GetBid(bid->next)->prev = bid->prev;
        } else {
        	stock->lowestBid = bid->prev;
        }

        //重置价位信息
        InitBid(bid);
    }
}


/*****************************************************************
** 函数名:      UpdateOfferByWithdraw
** 输 入:       无
** 输 出:       无
** 功能描述:    成交、撤单加工处理时的委卖价位处理
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
UpdateOfferByWithdraw(ORDER_CS *order, long long amount) {
    STOCK_CS    *stock      = NULL;
    OFFER_CS    *offer      = NULL;
    int			orderNum	= 0;
    int         offerIndex  = -1;

    if (! order || ! (stock = GetStock(order->stockNo))) {
        return;
    }

    offerIndex = order->priceLevel;
    if (! (offer = GetOffer(offerIndex))) {
        return;
    }

	OfferStatDel(offer, order, amount);
	
    //更新价位信息
    if(IsTradingPhase(order)){
    	offer->orderAmount -= amount;
    }
    if ((order->balance - amount == 0) || (IsIceOrder(order) && (order->remPeakQty - amount == 0))) {
        if(IsTradingPhase(order)){
	    	offer->orderNumber --;
	    }

        //更新同价位委托列表
        if (order->prevPrice >= 0) {
            GetOrder(order->prevPrice)->nextPrice = order->nextPrice;
        } else {
            offer->firstOrder = order->nextPrice;
        }
        
        if (order->nextPrice >= 0) {
            GetOrder(order->nextPrice)->prevPrice = order->prevPrice;
        } else {
        	offer->lastOrder = order->prevPrice;
        }
        
        order->prevPrice = \
        		order->nextPrice = \
        		order->priceLevel = NULL_INDEX;
    }

	orderNum = GetOfferOrderTotalNum(offer, stock->prcsStsValCod);
	
    //删除价位节点
    if (orderNum == 0) {
        stock->offerNumber --;
        
        //更新价位列表
        if (offer->prev >= 0) {
            GetOffer(offer->prev)->next = offer->next;
        } else {
        	stock->lowestOffer = offer->next;
        }
        
        if (offer->next >= 0) {
            GetOffer(offer->next)->prev = offer->prev;
        } else {
        	stock->highestOffer = offer->prev;
        }
        
        //重置价位信息
        InitOffer(offer);
    }
}


/*****************************************************************
** 函数名:      IsHaveOpponent
** 输 入:       order
** 输 出:       TRUE:倒挂 FALSE:未倒挂 
** 功能描述:    判断订单是否倒挂
** 全局变量:    无
** 调用模块:    无
** 作 者:       Li Bo
** 日 期:       2006.9.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL		
IsHaveOpponent(ORDER_CS* order){
	STOCK_CS* stock = NULL;
	BID_CS*   bid	= NULL;
	OFFER_CS* offer = NULL;
	ORDER_CS* pOrder = NULL;
	
	if(order == NULL){
		return FALSE;
	}
	
	if(order->orderQty == 0){
		return FALSE;
	}
	
	stock = GetStock(order->stockNo);
	if(stock == NULL){
		return FALSE;
	}

	//判断证券的交易时段是否需要检测倒挂
	
	if(IsBuyOrder(order)){
		offer = GetOffer(stock->lowestOffer);
		
		while(offer != NULL){
			if(order->orderPrice < offer->price){
				return FALSE;
			}
			if(offer->orderNumber == 0){
				offer = GetOffer(offer->next);
			}
			else if(offer->orderNumber > 0){
				break;
			}
		}
		
		if(offer == NULL){
			return FALSE;
		}
		
	}
	else if(IsSellOrder(order)){
		bid = GetBid(stock->highestBid);
		
		while(bid != NULL){
			if(order->orderPrice > bid->price){
				return FALSE;
			}
			if(bid->orderNumber == 0){
				bid = GetBid(bid->next);
			}
			else if(bid->orderNumber > 0){
				break;
			}
		}
		if(bid == NULL){
			return FALSE;
		}
		
    }
	
	//ERROR("价格倒挂","当前交易时段[%s],股票ISINCode=[%s],orderNo=[%lld]--orderPrice=[%lld]--trdResTypCod=[%s]--orderBSType=[%c],对手订单orderNo=[%lld]--orderPrice=[%lld]--trdResTypCod=[%s]--orderBSType=[%c]--balance=[%lld]",\
		stock->prcsStsValCod,stock->isinCod, order->orderNo, order->orderPrice, order->trdResTypCod, order->orderBSType, pOrder->orderNo, pOrder->orderPrice, pOrder->trdResTypCod, pOrder->orderBSType, pOrder->balance \
		);
	return TRUE;
}


/*****************************************************************
** 函数名:      BidStatAdd
** 输 入:       bid:要更新的bid节点，order:要更新的订单信息
** 输 出:       无
** 功能描述:    向买信息中增加交易时段统计信息
** 全局变量:    无
** 调用模块:    无
** 作 者:       Li Bo
** 日 期:       2006.9.27
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
BidStatAdd(BID_CS* bid, ORDER_CS* order){
	int i = 0;
	
	if(bid == NULL || order == NULL){
		return;
	}
	
	//确定交易时段标记
	if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
		i = 0;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
		i = 1;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
		i = 2;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
		i = 3;
	}
	else{
		//其他交易时段不需要统计
		return;
	}
	
	//把订单信息添加到交易时段统计信息
	bid->tradePhaseOrderNum[i]++;
	bid->tradePhaseOrderAmount[i] += order->balance;
}


/*****************************************************************
** 函数名:      BidStatAdd
** 输 入:       offer:要更新的bid节点，order:要更新的订单信息
** 输 出:       无
** 功能描述:    向卖信息中增加交易时段统计信息
** 全局变量:    无
** 调用模块:    无
** 作 者:       Li Bo
** 日 期:       2006.9.27
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
OfferStatAdd(OFFER_CS* offer, ORDER_CS* order){
	int i = 0;
	
	if(offer == NULL || order == NULL){
		return;
	}
	
	//确定交易时段标记
	if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
		i = 0;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
		i = 1;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
		i = 2;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
		i = 3;
	}
	else{
		return;
	}
	
	//把订单信息添加到交易时段统计信息
	offer->tradePhaseOrderNum[i]++;
	offer->tradePhaseOrderAmount[i] += order->balance;
}


/*****************************************************************
** 函数名:      BidStatDel
** 输 入:       bid:要更新的bid节点，order:要更新的订单信息, amount：要更新的数量
** 输 出:       无
** 功能描述:    向买信息中减少交易时段统计信息
** 全局变量:    无
** 调用模块:    无
** 作 者:       Li Bo
** 日 期:       2006.9.27
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
BidStatDel(BID_CS* bid, ORDER_CS* order, long long amount){
	int i = 0;
	
	if(bid == NULL || order == NULL || amount < 0){
		return;
	}
	
	//确定交易时段标记
	if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
		i = 0;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
		i = 1;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
		i = 2;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
		i = 3;
	}
	else{
		//其他交易时段不需要统计
		return;
	}
	
	//把订单信息添加到交易时段统计信息
	bid->tradePhaseOrderAmount[i] -= amount;
	if(order->balance - amount == 0){
		bid->tradePhaseOrderNum[i]--;
	}
}


/*****************************************************************
** 函数名:      OfferStatDel
** 输 入:       offer:要更新的 offer 节点，order:要更新的订单信息, amount：要更新的数量
** 输 出:       无
** 功能描述:    向卖信息中减少交易时段统计信息
** 全局变量:    无
** 调用模块:    无
** 作 者:       Li Bo
** 日 期:       2006.9.27
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
OfferStatDel(OFFER_CS* offer, ORDER_CS* order, long long amount){
	int i = 0;
	
	if(offer == NULL || order == NULL || amount < 0){
		return;
	}
	
	//确定交易时段标记
	if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
		i = 0;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
		i = 1;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
		i = 2;
	}
	else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
		i = 3;
	}
	else{
		return;
	}
	
	//把订单信息添加到交易时段统计信息
	offer->tradePhaseOrderAmount[i] -= amount;
	if(order->balance - amount == 0){
		offer->tradePhaseOrderNum[i]--;
	}
}


/*****************************************************************
** 函数名:      UpdateBidNodeByOrder
** 输 入:       无
** 输 出:       无
** 功能描述:    更新委买价位节点
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
UpdateBidNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int bidIndex) {
    BID_CS      *bid        = NULL;
    ORDER_CS	*pOrder		= NULL;
    int         lastOrder   = -1;

    bid = GetBid(bidIndex);
	
	order->priceLevel = bidIndex;

	//要加入的订单不是报价，或者价格、时间优先
    order->prevPrice = bid->lastOrder;
    order->nextPrice = NULL_INDEX;

    lastOrder = bid->lastOrder;
    bid->lastOrder = order->orderProcessNo;
    if (lastOrder >= 0) {
      GetOrder(lastOrder)->nextPrice = order->orderProcessNo;
    }
    
    //BidStatAdd(bid, order);
}


/*****************************************************************
** 函数名:      InsertBidNodeByOrder
** 输 入:       无
** 输 出:       无
** 功能描述:    插入新委买价位节点
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
InsertBidNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int nextBidIndex) {
    int     bidIndex    = -1;
    BID_CS  *newBid     = NULL;
    BID_CS  *nextBid    = NULL;
    BID_CS  *prevBid    = NULL;
	
    bidIndex = AddNewBidNode(stock, order);

    newBid = GetBid(bidIndex);
    nextBid = GetBid(nextBidIndex);
    prevBid = GetBid(nextBid->prev);

    if (stock->highestBid == nextBidIndex || ! prevBid) {
        /*
         * 处理价位列表头节点
         */
        stock->highestBid = bidIndex;
    }

    newBid->next = nextBidIndex;
    newBid->prev = nextBid->prev;

    if (prevBid) {
        prevBid->next = bidIndex;
    }

    if (nextBid) {
    	nextBid->prev = bidIndex;
    }
}


/*****************************************************************
** 函数名:      InsertLastBidNodeByOrder
** 输 入:       无
** 输 出:       无
** 功能描述:    插入末委买价位节点
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
InsertLastBidNodeByOrder(STOCK_CS *stock, ORDER_CS *order) {
    int bidIndex = -1;

    bidIndex = AddNewBidNode(stock, order);

    printf("pricequeue  bidIndex  %d \ ",bidIndex);
    if (stock->highestBid < 0) {
        stock->highestBid = bidIndex;
        stock->lowestBid = bidIndex;
    } else {
        GetBid(stock->lowestBid)->next = bidIndex;
        GetBid(bidIndex)->prev = stock->lowestBid;
        stock->lowestBid = bidIndex;
    }
}


/*****************************************************************
** 函数名:      AddNewBidNode
** 输 入:       无
** 输 出:       new Bid index
** 功能描述:    增加委买价位节点
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static int
AddNewBidNode(STOCK_CS *stock, ORDER_CS *order) {
    int     bidIndex    = -1;
    int     orderIndex  = -1;
    BID_CS  *bid        = NULL;

    orderIndex = order->orderProcessNo;
    bidIndex = NextBid();
    bid = GetBid(bidIndex);

    InitBid(bid);

    bid->price = order->orderPrice;
    if(IsTradingPhase(order)){
    	bid->orderNumber = 1;
	    if(IsIceOrder(order)){
	    	TRACE(_moduleName, "冰山订单(增加委买节点)，开始处理价格统计信息");
	    	bid->orderAmount = order->remPeakQty;
	    	TRACE(_moduleName, "冰山订单(增加委买节点)，处理价格统计信息结束 现委托数量[%lld],冰山订单揭示数量[%lld]",bid->orderAmount,order->remPeakQty);
		}
		else{
			bid->orderAmount = order->balance;
		}
    }
    
    bid->firstOrder = orderIndex;
    bid->lastOrder = orderIndex;

    order->priceLevel = bidIndex;
    order->prevPrice = order->nextPrice = NULL_INDEX;
    
    BidStatAdd(bid, order);

    /*
     * 更新Stock信息
     */
    stock->bidNumber ++;

    return bidIndex;
}


/*****************************************************************
** 函数名:      UpdateOfferNodeByOrder
** 输 入:       无
** 输 出:       无
** 功能描述:    更新委卖价位节点
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
UpdateOfferNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int offerIndex) {
    OFFER_CS    *offer      = NULL;
    ORDER_CS	*pOrder		= NULL;
    int         lastOrder   = -1;

    offer = GetOffer(offerIndex);

    order->priceLevel = offerIndex;

	//要加入的订单不是报价，或者价格、时间优先
    order->prevPrice = offer->lastOrder;
    order->nextPrice = NULL_INDEX;

    lastOrder = offer->lastOrder;
    offer->lastOrder = order->orderProcessNo;
    if (lastOrder >= 0) {
      GetOrder(lastOrder)->nextPrice = order->orderProcessNo;
    }

	
    //OfferStatAdd(offer, order);
}


/*****************************************************************
** 函数名:      InsertOfferNodeByOrder
** 输 入:       无
** 输 出:       无
** 功能描述:    插入新委卖价位节点
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
InsertOfferNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int nextOfferIndex) {
    int         offerIndex  = -1;
    OFFER_CS    *newOffer   = NULL;
    OFFER_CS    *nextOffer  = NULL;
    OFFER_CS    *prevOffer  = NULL;

    offerIndex = AddNewOfferNode(stock, order);
    printf(" offer  index   %d \n ",offerIndex);

    newOffer = GetOffer(offerIndex);
    nextOffer = GetOffer(nextOfferIndex);
    prevOffer = GetOffer(nextOffer->prev);

    if (stock->lowestOffer == nextOfferIndex || ! prevOffer) {
        /*
         * 处理价位列表头节点
         */
        stock->lowestOffer = offerIndex;
    }

    newOffer->next = nextOfferIndex;
    newOffer->prev = nextOffer->prev;

    if (prevOffer) {
        prevOffer->next = offerIndex;
    }
    
    if (nextOffer) {
    	nextOffer->prev = offerIndex;
    }
}


/*****************************************************************
** 函数名:      InsertLastOfferNodeByOrder
** 输 入:       无
** 输 出:       无
** 功能描述:    插入末委卖价位节点
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
InsertLastOfferNodeByOrder(STOCK_CS *stock, ORDER_CS *order) {
    int offerIndex = -1;

    offerIndex = AddNewOfferNode(stock, order);

    printf("price queue  offerIndex %d \n", offerIndex);
    if (stock->lowestOffer < 0) {
        stock->lowestOffer = offerIndex;
        stock->highestOffer = offerIndex;
    } else {
        GetOffer(stock->highestOffer)->next = offerIndex;
        GetOffer(offerIndex)->prev = stock->highestOffer;
        stock->highestOffer = offerIndex;
    }
}


/*****************************************************************
** 函数名:      AddNewOfferNode
** 输 入:       无
** 输 出:       new Offer index
** 功能描述:    增加委卖价位节点
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.9.5
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static int
AddNewOfferNode(STOCK_CS *stock, ORDER_CS *order) {
    int         offerIndex  = -1;
    int         orderIndex  = -1;
    OFFER_CS    *offer      = NULL;

    orderIndex = order->orderProcessNo;
    offerIndex = NextOffer();
    offer = GetOffer(offerIndex);

    InitOffer(offer);

    offer->price = order->orderPrice;
    /*if(IsTradingPhase(order)){
	    offer->orderNumber = 1;
	    if(IsIceOrder(order)){
	    	TRACE(_moduleName, "冰山订单(增加委卖节点)，开始处理价格统计信息");
	    	offer->orderAmount = order->remPeakQty;
	    	TRACE(_moduleName, "冰山订单(增加委卖节点)，处理价格统计信息结束 现委托数量[%lld],冰山订单揭示数量[%lld]",offer->orderAmount,order->remPeakQty);
	    }
	    else{
	    	offer->orderAmount = order->balance;
	    }
	}*/
    offer->orderAmount = order->balance;
    offer->firstOrder = orderIndex;
    offer->lastOrder = orderIndex;

    order->priceLevel = offerIndex;
    order->prevPrice = order->nextPrice = NULL_INDEX;

	//OfferStatAdd(offer, order);
    /*
     * 更新Stock信息
     */
    stock->offerNumber ++;

    return offerIndex;
}


/*****************************************************************
** 函数名:      NextBid
** 输 入:       无
** 输 出:       下一可用的空闲Bid的位置
** 功能描述:    返回下一可用的空闲Bid位置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static int
NextBid() {
  /* 修改为非递归算法 */
	int i;
	if (GetShareMemCurrentStats()->nextBid >= GetShareMemConfigStats()->bidMax) {
		GetShareMemCurrentStats()->nextBid = 0;
	}
	
	for ( i=0;i<GetShareMemConfigStats()->bidMax;i++){
		if (GetBid(GetShareMemCurrentStats()->nextBid)->price > 0) {
			GetShareMemCurrentStats()->nextBid++;
			if (GetShareMemCurrentStats()->nextBid >= GetShareMemConfigStats()->bidMax) {
				GetShareMemCurrentStats()->nextBid = 0;
			}
		}else{
			return GetShareMemCurrentStats()->nextBid++;
		} 
	}
    
    if (i >= GetShareMemConfigStats()->bidMax) {
		while(1){
			FATAL("Price.NextBid()", "委买价位数量超出预定义的最大委买价位数量！ Current Number: %d", GetShareMemCurrentStats()->nextBid);
			sleep(1);
		}
	}
	return -1;
}


/*****************************************************************
** 函数名:      NextOffer
** 输 入:       无
** 输 出:       下一可用的空闲Offer的位置
** 功能描述:    返回下一可用的空闲Offer位置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static int
NextOffer() {
  /* 修改为非递归算法 */
	int i;
	if (GetShareMemCurrentStats()->nextOffer >= GetShareMemConfigStats()->offerMax) {
		GetShareMemCurrentStats()->nextOffer = 0;
	}

	for ( i=0;i<GetShareMemConfigStats()->offerMax;i++){
/**** SIR 00811 BEGIN ***/	
		if (GetOffer(GetShareMemCurrentStats()->nextOffer)->price > 0) {
/**** SIR 00811 END ***/	
			GetShareMemCurrentStats()->nextOffer++;
			if (GetShareMemCurrentStats()->nextOffer >= GetShareMemConfigStats()->offerMax) {
				GetShareMemCurrentStats()->nextOffer = 0;
			}
		}else{
			return GetShareMemCurrentStats()->nextOffer++;
		} 
    }
	if (i >= GetShareMemConfigStats()->offerMax) {
		while(1){
			FATAL("Price.NextBid()", "委买价位数量超出预定义的最大委买价位数量！ Current Number: %d", GetShareMemCurrentStats()->nextOffer);
			sleep(1);
		}
	}
	return -1;
}


/*****************************************************************
** 函数名:      InitBid
** 输 入:       无
** 输 出:       无
** 功能描述:    初始化指定位置的BID_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
InitBid(BID_CS *target) {
    bzero(target, sizeof(BID_CS));
    target->firstOrder = target->lastOrder = \
            target->prev = target->next = NULL_INDEX;
}


/*****************************************************************
** 函数名:      InitOffer
** 输 入:       无
** 输 出:       无
** 功能描述:    初始化指定位置的OFFER_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
InitOffer(OFFER_CS *target) {
    bzero(target, sizeof(OFFER_CS));
    target->firstOrder = target->lastOrder = \
            target->prev = target->next = NULL_INDEX;
}


/*****************************************************************
** 函数名:      GetBidOrderTotalNum
** 输 入:       bid
** 输 出:       无
** 功能描述:    获得指定买方价格节点的委托总笔数
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.10.17
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static int	
GetBidOrderTotalNum(BID_CS* bid, char *prcsStsValCod){
	int		number = 0;
	
	number = bid->orderNumber + bid->tradePhaseOrderNum[0] + bid->tradePhaseOrderNum[1] + bid->tradePhaseOrderNum[2] + bid->tradePhaseOrderNum[3];
	
	if(strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL,strlen(STOCK_TRADING_RESTRICTION_OCALL))==0){
		number -= (bid->tradePhaseOrderNum[0] + bid->tradePhaseOrderNum[1]);
	}
	else if(strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_ICALL,strlen(STOCK_TRADING_RESTRICTION_ICALL))==0 || strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL,strlen(STOCK_TRADING_RESTRICTION_OCALL))==0){
		number -= bid->tradePhaseOrderNum[1];
	}
	else if(strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_IOBB,strlen(STOCK_TRADING_RESTRICTION_IOBB))==0 || strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_IPOBB,strlen(STOCK_TRADING_RESTRICTION_IPOBB))==0 || strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_OOBB,strlen(STOCK_TRADING_RESTRICTION_OOBB))==0 || strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_OPOBB,strlen(STOCK_TRADING_RESTRICTION_OPOBB))==0){
		number -= bid->tradePhaseOrderNum[2];
	}
	else if(strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_FCALL,strlen(STOCK_TRADING_RESTRICTION_FCALL))==0){
		number -= bid->tradePhaseOrderNum[3];
	}
	else{
	}
	
	return number;
}

/*****************************************************************
** 函数名:      GetOfferOrderTotalNum
** 输 入:       offer
** 输 出:       无
** 功能描述:    获得指定卖方价格节点的委托总笔数
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.10.17
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static int
GetOfferOrderTotalNum(OFFER_CS* offer, char *prcsStsValCod){
	int  number = 0;
	
	number = offer->orderNumber + offer->tradePhaseOrderNum[0] + offer->tradePhaseOrderNum[1] + offer->tradePhaseOrderNum[2] + offer->tradePhaseOrderNum[3];
	
	if(strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL,strlen(STOCK_TRADING_RESTRICTION_OCALL))==0){
		number -= (offer->tradePhaseOrderNum[0] + offer->tradePhaseOrderNum[1]);
	}
	else if(strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_ICALL,strlen(STOCK_TRADING_RESTRICTION_ICALL))==0 || strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL,strlen(STOCK_TRADING_RESTRICTION_OCALL))==0){
		number -= offer->tradePhaseOrderNum[1];
	}
	else if(strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_IOBB,strlen(STOCK_TRADING_RESTRICTION_IOBB))==0 || strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_IPOBB,strlen(STOCK_TRADING_RESTRICTION_IPOBB))==0 || strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_OOBB,strlen(STOCK_TRADING_RESTRICTION_OOBB))==0 || strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_OPOBB,strlen(STOCK_TRADING_RESTRICTION_OPOBB))==0){
		number -= offer->tradePhaseOrderNum[2];
	}
	else if(strncmp(prcsStsValCod,STOCK_TRADING_RESTRICTION_FCALL,strlen(STOCK_TRADING_RESTRICTION_FCALL))==0){
		number -= offer->tradePhaseOrderNum[3];
	}
	else{
	}
	
	return number;
}


/*****************************************************************
** 函数名:      PrintBidInfoToFile
** 输 入:       fp 文件描述符
** 输 出:       无
** 功能描述:    打印委买价位信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       weihy
** 日 期:       2005.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
PrintBidInfoToFile(int fp) {
    BID_CS      *priceLevel = NULL;
    char        buf[1024];
    int         number = 0;
    int         i = 0;

    number = GetBidNumber();

    sprintf(buf, "#****委买价位信息****\n#委买价位数: %d\n\n", number);
    write(fp, buf, strlen(buf));

    for (i = 0; i < number; i++) {
        priceLevel = GetBid(i);

        sprintf(buf, "index: %d\t" \
            "price: %lld\t" \
            "orderNumber: %d\t" \
            "orderAmount: %lld\t" \
            "firstOrder: %d\t" \
            "lastOrder: %d\t" \
            "prev: %d\t" \
            "next: %d\n"
            , i
            , priceLevel->price
            , priceLevel->orderNumber
            , priceLevel->orderAmount
            , priceLevel->firstOrder
            , priceLevel->lastOrder
            , priceLevel->prev
            , priceLevel->next);

        write(fp, buf, strlen(buf));
    }
}


/*****************************************************************
** 函数名:      PrintBidInfoToFile
** 输 入:       fp 文件描述符
** 输 出:       无
** 功能描述:    打印委卖价位信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       weihy
** 日 期:       2005.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
PrintOfferInfoToFile(int fp) {
    OFFER_CS    *priceLevel = NULL;
    char        buf[1024];
    int         number = 0;
    int         i = 0;

    number = GetOfferNumber();

    sprintf(buf, "#****委卖价位信息****\n#委卖价位数: %d\n\n", number);
    write(fp, buf, strlen(buf));

    for (i = 0; i < number; i++) {
        priceLevel = GetOffer(i);

        sprintf(buf, "index: %-4d\t" \
            "price: %-4lld\t" \
            "orderNumber: %-4d\t" \
            "orderAmount: %-4lld\t" \
            "firstOrder: %-4d\t" \
            "lastOrder: %-4d\t" \
            "prev: %-4d\t" \
            "next: %-4d\n"
            , i
            , priceLevel->price
            , priceLevel->orderNumber
            , priceLevel->orderAmount
            , priceLevel->firstOrder
            , priceLevel->lastOrder
            , priceLevel->prev
            , priceLevel->next);

        write(fp, buf, strlen(buf));
    }
}


/*****************************************************************
** 函数名:      PrintStockSnapshotToFile
** 输 入:       无
** 输 出:       TRUE FALSE
** 功能描述:    打印证券快照信息到文件
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.10.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL	
PrintStockSnapshotToFile(){
    int     	fp = 0;
    char		buf[512];
    char    	filePath[256];
    STOCK_CS*	stock = NULL;
    BID_CS*		bid = NULL;
    OFFER_CS*	offer = NULL;
    ORDER_CS*	order = NULL;
    int			i = 0;

	if (!IsHomeValid()) {
        return FALSE;
    }
    
    sprintf(filePath, "%s/StockSnapshot%s.txt", GetHome(), GetTime());
    
	if ((fp = open(filePath, O_WRONLY | O_CREAT | O_APPEND,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
        printf("ERROR: 创建文件[%s]失败! \n", filePath);
        return FALSE;
    }
    
    for(i = 0; i < GetStockNumber(); i++){
    	stock = GetStock(i);
    	bid = GetBid(stock->highestBid);
    	while(bid != NULL){
    		memset(buf, 0, 512);
    		sprintf(buf, "证券代码[%s]，买入价[%lld]，订单排名队列\n", stock->stockId, bid->price);
    		//write(fp, buf, strlen(buf));
    		
    		order = GetOrder(bid->firstOrder);
		    while(order != NULL){
		    	memset(buf, 0, 512);
		    	sprintf(buf, "%5d|%16llu|%15lld|%15lld|%15d|%15lld|%15lld|%c|%15lld|%c|%c|%2s|%12s|%8s|\n",
		    		stock->isinIndex,
		    		order->orderNo,
		    		order->orderPrice,
		    		order->balance,
		    		order->orderTime,
		    		order->peakSizeQty,
		    		order->remPeakQty,
		    		order->orderBSType,
		    		order->orderQty - order->balance,
		    		order->orderStatus,
		    		order->orderType,
		    		order->trdResTypCod,
		    		order->isinCod,
		    		order->pbuId
		    		);
		    	write(fp, buf, strlen(buf));
		    	order = GetOrder(order->nextPrice);
		    }
		    bid = GetBid(bid->next);
    	}
    	
    	offer = GetOffer(stock->lowestOffer);
    	while(offer != NULL){
    		memset(buf, 0, 512);
    		sprintf(buf, "证券代码[%s]，卖出价[%lld]，订单排名队列\n", stock->stockId, offer->price);
    		//write(fp, buf, strlen(buf));
    		
    		order = GetOrder(offer->firstOrder);
		    while(order != NULL){
		    	memset(buf, 0, 512);
		    	sprintf(buf, "%5d|%16llu|%15lld|%15lld|%15d|%15lld|%15lld|%c|%15lld|%c|%c|%2s|%12s|%8s|\n",
		    		stock->isinIndex,
		    		order->orderNo,
		    		order->orderPrice,
		    		order->balance,
		    		order->orderTime,
		    		order->peakSizeQty,
		    		order->remPeakQty,
		    		order->orderBSType,
		    		order->orderQty - order->balance,
		    		order->orderStatus,
		    		order->orderType,
		    		order->trdResTypCod,
		    		order->isinCod,
		    		order->pbuId
		    		);
		    	write(fp, buf, strlen(buf));
		    	order = GetOrder(order->nextPrice);
		    }
		    offer = GetOffer(offer->next);
    	}
    }
    
    close(fp);
    return TRUE;
}
