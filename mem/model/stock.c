/******************************************************************
** 文件名:      stock.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.9.6
** 描 述:       Stock信息定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
** 2006.1.4     Song Fang   增加返回单边模式的成交信息的接口
** 2006.03.23   Wei Huanyin 1、增加lastPrice字段处理
** 2006.04.15   Wei Huanyin 1、增加导入债券静态数据标志处理函数
** 2006.05.30   Wei Huanyin 1、增加集合竞价结束时相关数据
** 2006.06.16   zhuhui		1、增加更新虚拟集合竞价信息函数
** 2007.04.04	Li Bo		1、在PrintStockInfoToFile函数中增加setId信息输出
							2、在AddStock函数中增加setId的处理
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/stock.c,v 1.1 2012/03/29 01:19:54 tc\boli Exp $
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

#include    "stock.h"
#include    "model.h"
#include    "../memory.h"
//#include    "../static_file/stock_industry.h"
#include    "../env/env.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"
#include    "../util/log/log.h"
#include    "../util/time/common_time.h"

/*
 * 模块名称
 */
static const char   _moduleName[] = "stock";

/*
 * 内部函数声明
 */
static int CompIsinCod(STOCK_CS *p1, STOCK_CS *p2);					/* 二分法需要的比较函数 */


/*****************************************************************
** 函数名:      GetStockList
** 输 入:       无
** 输 出:       STOCK_CS[]
** 功能描述:    返回共享内存中Stock数组的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
STOCK_CS*
GetStockList() {
    return (STOCK_CS*) GetShareMemory() + GetShareMemAllocationStats()->stockOffset;
}


/*****************************************************************
** 函数名:      GetStock
** 输 入:       无
** 输 出:       STOCK_CS*
** 功能描述:    返回指定位置的STOCK_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
STOCK_CS*
GetStock(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->stockMax) {
        return NULL;
    }
    return GetStockList() + index;
}


/*****************************************************************
** 函数名:      GetStockByIsinCod
** 输 入:       isinCod
** 输 出:       STOCK_CS*
** 功能描述:    返回指定isinCod对应的STOCK_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
STOCK_CS*
GetStockByIsinCod(char *isinCod) {
    STOCK_CS    *stock  = NULL;
    STOCK_CS	stockCmp;
    
 /*   if(strlen(isinCod) != STOCK_ISIN_CODE_LENGTH - 1){
    	ERROR(_moduleName, "搜索股票失败，股票isinCod=[%s]长度[%d]错误", isinCod, strlen(isinCod));
    	return NULL;
    }*/
    
    memcpy(stockCmp.isinCod, isinCod, STOCK_ISIN_CODE_LENGTH);
    
    stock = (STOCK_CS*)bsearch(&stockCmp, GetStockList(),
		GetStockNumber(), sizeof(STOCK_CS), (int(*)(const void *,const void *))CompIsinCod);
    printf("stock   %0x  \n", stock);

    return stock;
}


/*****************************************************************
** 函数名:      GetStockByIsinIndex
** 输 入:       isinIndex
** 输 出:       STOCK_CS*
** 功能描述:    返回指定isinIndex对应的STOCK_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
STOCK_CS*	
GetStockByIsinIndex(int isinIndex){
	int         i       = 0;
    STOCK_CS    *stock  = NULL;
    
    for (i = 0; i < GetStockNumber(); i++) {
        stock = GetStock(i);
        if (stock->isinIndex == isinIndex){
            return stock;
        }
    }
    
    return NULL;
}

/*****************************************************************
** 函数名:      GetStockByStockId
** 输 入:       无
** 输 出:       STOCK_CS*
** 功能描述:    返回指定stockId对应的STOCK_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
STOCK_CS*
GetStockByStockId(char *stockId) {
    int         i       = 0;
    STOCK_CS    *stock  = NULL;

    for (i = 0; i < GetStockNumber(); i++) {
        stock = GetStock(i);
        if (strcmp(stock->stockId, stockId) == 0) {
            return stock;
        }
    }
    return NULL;
}


/*****************************************************************
** 函数名:      CompIsinCod
** 输 入:       STOCK_CS
** 输 出:       int
** 功能描述:    二分法需要的比较函数
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.13
** 修 改:		Li Bo 
** 日 期:		2009.11.12
** 版本:        0.1
** 修改内容：	比较函数由strcmp改成strncmp
****************************************************************/
static int 
CompIsinCod(STOCK_CS *p1, STOCK_CS *p2){ 
	return strncmp(p1->isinCod, p2->isinCod, STOCK_ISIN_CODE_LENGTH - 1);
}

/*****************************************************************
** 函数名:      GetStockNumber
** 输 入:       无
** 输 出:       Stock总数
** 功能描述:    返回Stock数量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetStockNumber() {
    return GetShareMemCurrentStats()->nextStock;
}


/*****************************************************************
** 函数名:      AddStock
** 输 入:       无
** 输 出:       新增的Stock的位置
** 功能描述:    向Stock数组中增加Stock条目
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
AddStock(STOCK_INFO *pStockInfo) {
    STOCK_CS    *target = NULL;
    int         index   = GetShareMemCurrentStats()->nextStock;
    int			insertIndex = 0;
	int			i		= 0;
	
    /*
     * 检查内存是否越界
     */
    if (index >= GetShareMemConfigStats()->stockMax) {
        FATAL("Stock.AddStock()", "证券数量超出预定义的最大证券数量！ Current Number: %d"
                , index);
        return -1;
    }

    target = GetStockList();
	
	for(i = 0; i < index; i++){
		if(strcmp(target->isinCod, pStockInfo->isinCod) > 0){
			break;
		}
		target++;
	}
	
	memmove(target + 1, target, sizeof(STOCK_CS) * (index - (target - GetStockList())));
	
	target->setId = pStockInfo->setId;
	memcpy(target->isinCod, pStockInfo->isinCod, STOCK_ISIN_CODE_LENGTH);
    memcpy(target->stockId, pStockInfo->stockId, STOCK_ID_LENGTH);
    memcpy(target->stockShortName, pStockInfo->stockShortName, STOCK_SHORTNAME_LENGTH);
    memcpy(target->conSegCode, pStockInfo->conSegCode, 5);
    target->isinIndex = pStockInfo->isinIndex;
    target->lotSize = pStockInfo->lotSize;
    
    /*
     * 初始化Stock 指针 和 虚拟竞价买卖盈余方向
     */
    target->surplusBidAskIndicator = ' ';
    /*买卖最长需时初始化为-1，立即成交时更新为0，其他完全成交时计算最长成交需时*/
    target->bidTradeMaxDuration = \
    target->offerTradeMaxDuration = -1;
    target->categoryNo = \
    target->industryNo = \
    target->nextCategory = \
            target->channelId = target->channelNextStock = \
            target->firstOrderBuy = target->lastOrderBuy = \
            target->firstOrderSell = target->lastOrderSell = \
            target->firstTrade = target->lastTrade = \
            target->firstWithdraw = target->lastWithdraw = \
            target->highestBid = target->lowestBid = \
            target->highestOffer = target->lowestOffer = NULL_INDEX;

    #ifdef USE_PBU
    	target->lastBuyOrderSortOffset = target->lastSellOrderSortOffset = NULL_INDEX;
    
		for(i = 0; i < PBU_TRADE_SORT_LENGTH; i++){
		    target->pbuTradeBuySort[i] = -1;
		    target->pbuTradeSellSort[i] = -1;
		}
    #endif
    
    insertIndex = target - GetStockList();
    for(i = insertIndex; i <= index; i++){
    	target->stockNo = i;
    	target++;
    }
    
    GetShareMemCurrentStats()->nextStock++;
    
    return insertIndex;
}

 
/***************************************************************
** 函数:      UpdateStockCategory
** 输 入:       STOCK_CATEGORY_MON*
** 输 出:       BOOL
** 功能描述:    更新证券品种配置
** 全句变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.29
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
UpdateStockCategory(STOCK_CATEGORY_MON *stockCategory) {
    //STOCK_CS    *stock  = NULL;

    //if (stockCategory && (stock = GetStockById(stockCategory->stockId))) {
        /*
         * 更新证券品种列表
         */
        //AddCategoryStock(stockCategory->categoryId, stock);
        //return TRUE;
    //}
    return FALSE;
}


/*****************************************************************
** 函数名:      UpdateStockIndustry
** 输 入:       STOCK_INDUSTRY*，shareType
** 输 出:       BOOL
** 功能描述:    更新证券行业配置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.10.13
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        区分行业类型A、B股
****************************************************************/
/*BOOL
UpdateStockIndustry(STOCK_INDUSTRY *stockIndustry,int shareType) {
    STOCK_CS    *stock  = NULL;

    if (stockIndustry && (stock = GetStockByIsinCod(stockIndustry->isinCod))) {
        if(AddIndustryStock(stockIndustry->industryId, stock, shareType) != -1){
        	return TRUE;
        }
    }
    return FALSE;
}*/

/*****************************************************************
** 函数名:      IsShare
** 输 入:       stockNo
** 输 出:       BOOL
** 功能描述:    返回stockNo对应的证券类型是否是股票
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsShare(STOCK_CS *stock) {
    CATEGORY_CS *category = NULL;

    if (stock && (category = GetCategory(stock->categoryNo))) {
        return category->id == CATEGORY_ID_ASHARE
                || category->id == CATEGORY_ID_BSHARE;
    }
    return FALSE;
}

/*****************************************************************
** 函数名:      IsAShare
** 输 入:       stockNo
** 输 出:       BOOL
** 功能描述:    返回stockNo对应的证券类型是否是A股
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsAShare(STOCK_CS *stock) {
    CATEGORY_CS *category = NULL;

    if (stock && (category = GetCategory(stock->categoryNo))) {
        return category->id == CATEGORY_ID_ASHARE;
    }
    return FALSE;
}

/*****************************************************************
** 函数名:      IsBShare
** 输 入:       stockNo
** 输 出:       BOOL
** 功能描述:    返回stockNo对应的证券类型是否是B股
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsBShare(STOCK_CS *stock) {
    CATEGORY_CS *category = NULL;

    if (stock && (category = GetCategory(stock->categoryNo))) {
        return  category->id == CATEGORY_ID_BSHARE;
    }
    return FALSE;
}

/*****************************************************************
** 函数名:      IsIndex
** 输 入:       stockNo
** 输 出:       BOOL
** 功能描述:    返回stockNo对应的证券类型是否是指数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsIndex(STOCK_CS *stock) {
    if (stock) {
        return stock->stockType == STOCK_TYPE_INDEX;
    }
    return FALSE;
}


/*****************************************************************
** 函数名:      IsValidStock
** 输 入:       stockNo
** 输 出:       BOOL
** 功能描述:    返回stockNo对应的证券是否有效产品品种
** 全局变量:    无
** 调用模块:
** 作 者:       Wei Huanyin
** 日 期:       2006.03.16
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsValidStock(STOCK_CS *stock) {
    if (stock) {
        return stock->categoryNo >= 0;
    }
    return FALSE;
}


/*****************************************************************
** 函数名:      IsStockReceived
** 输 入:       无
** 输 出:       BOOL
** 功能描述:    返回股票是否已全部接收完毕
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.11.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsStockReceived() {
    return GetMemTradeStats()->isStockReceived == 1;
}


/*****************************************************************
** 函数名:      SetStockReceived
** 输 入:       BOOL
** 输 出:       无
** 功能描述:    设置股票是否已全部接收完毕
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.11.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetStockReceived(BOOL isStockReceived) {
    if (isStockReceived) {
        GetMemTradeStats()->isStockReceived = 1;
    } else {
        GetMemTradeStats()->isStockReceived = 0;
    }
}


/*****************************************************************
** 函数名:      IsStockIndustryLoaded
** 输 入:       无
** 输 出:       BOOL
** 功能描述:    返回股票行业配置是否已加载完毕
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.11.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsStockIndustryLoaded() {
    return GetMemTradeStats()->isStockIndustryLoaded == 1;
}


/*****************************************************************
** 函数名:      SetStockIndustryLoaded
** 输 入:       BOOL
** 输 出:       无
** 功能描述:    设置股票行业配置是否已加载完毕
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.11.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetStockIndustryLoaded(BOOL isStockIndustryLoaded) {
    if (isStockIndustryLoaded) {
        GetMemTradeStats()->isStockIndustryLoaded = 1;
    } else {
        GetMemTradeStats()->isStockIndustryLoaded = 0;
    }
}


/*****************************************************************
** 函数名:      IsStockIndustryLoaded
** 输 入:       无
** 输 出:       BOOL
** 功能描述:    返回普通债券数据是否已加载完毕
** 全局变量:    无
** 调用模块:
** 作 者:       Wei Huanyin
** 日 期:       2006.4.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsCommonBondLoaded() {
    return GetMemTradeStats()->isCommonBondLoaded == 1;
}


/*****************************************************************
** 函数名:      SetStockIndustryLoaded
** 输 入:       BOOL
** 输 出:       无
** 功能描述:    设置普通债券数据是否已加载完毕
** 全局变量:    无
** 调用模块:
** 作 者:       Wei Huanyin
** 日 期:       2006.4.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetCommonBondLoaded(BOOL isCommonBondLoaded) {
    if (isCommonBondLoaded) {
        GetMemTradeStats()->isCommonBondLoaded = 1;
    } else {
        GetMemTradeStats()->isCommonBondLoaded = 0;
    }
}


/*****************************************************************
** 函数名:      IsControlSegmentLoaded
** 输 入:       无
** 输 出:       BOOL
** 功能描述:    返回产品群基础信息数据是否已加载完毕
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.10.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsControlSegmentLoaded() {
    return GetMemTradeStats()->isControlSegmentLoaded == 1;
}


/*****************************************************************
** 函数名:      SetControlSegmentLoaded
** 输 入:       BOOL
** 输 出:       无
** 功能描述:    设置产品群基础信息数据是否已加载完毕
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.10.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetControlSegmentLoaded(BOOL isControlSegmentLoaded) {
    if (isControlSegmentLoaded) {
        GetMemTradeStats()->isControlSegmentLoaded = 1;
    } else {
        GetMemTradeStats()->isControlSegmentLoaded = 0;
    }
}


/*****************************************************************
** 函数名:      IsStockIndustryLoaded
** 输 入:       无
** 输 出:       BOOL
** 功能描述:    返回可转换债券数据是否已加载完毕
** 全局变量:    无
** 调用模块:
** 作 者:       Wei Huanyin
** 日 期:       2006.4.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsConvertibleBondLoaded() {
    return GetMemTradeStats()->isConvertibleBondLoaded == 1;
}


/*****************************************************************
** 函数名:      SetStockIndustryLoaded
** 输 入:       BOOL
** 输 出:       无
** 功能描述:    设置可转换债券是否已加载完毕
** 全局变量:    无
** 调用模块:
** 作 者:       Wei Huanyin
** 日 期:       2006.4.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetConvertibleBondLoaded(BOOL isConvertibleBondLoaded) {
    if (isConvertibleBondLoaded) {
        GetMemTradeStats()->isConvertibleBondLoaded = 1;
    } else {
        GetMemTradeStats()->isConvertibleBondLoaded = 0;
    }
}


/*****************************************************************
** 函数名:      IsTradePhase
** 输 入:       STOCK_CS *stock
** 输 出:       BOOL
** 功能描述:    根据虚拟集合竞价的交易时段判断是否可以发送数据
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.9.27
** 修 改:		Li Bo
** 日 期:		2009.11.26
** 版本:        0.1
****************************************************************/
BOOL
IsTradePhaseBB(STOCK_CS *stock) {
	
	if(strncmp(stock->commonPrcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL,strlen(STOCK_TRADING_RESTRICTION_OCALL))==0 
		||strncmp(stock->commonPrcsStsValCod,STOCK_TRADING_RESTRICTION_OPOBB,strlen(STOCK_TRADING_RESTRICTION_OPOBB))==0 
		||strncmp(stock->commonPrcsStsValCod,STOCK_TRADING_RESTRICTION_OOBB,strlen(STOCK_TRADING_RESTRICTION_OOBB))==0){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

/*****************************************************************
** 函数名:      IsTradePhaseTr
** 输 入:       STOCK_CS *stock
** 输 出:       BOOL
** 功能描述:    返回是否是产品其他时段
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.9.27
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsTradePhaseTr(STOCK_CS *stock) {

	if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_TRADE,strlen(STOCK_TRADING_RESTRICTION_TRADE))==0
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_BREAK,strlen(STOCK_TRADING_RESTRICTION_BREAK))==0 
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_HALT,strlen(STOCK_TRADING_RESTRICTION_HALT))==0 
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_SUSP,strlen(STOCK_TRADING_RESTRICTION_SUSP))==0 
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_CLOSE,strlen(STOCK_TRADING_RESTRICTION_CLOSE))==0
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_POSTR,strlen(STOCK_TRADING_RESTRICTION_POSTR))==0
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_ENDTR,strlen(STOCK_TRADING_RESTRICTION_ENDTR))==0
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_BETW,strlen(STOCK_TRADING_RESTRICTION_BETW))==0){
						
   		return CheckTradePhase(stock);
   			    
	}
	else{
		return FALSE;
	}
	

}

/*****************************************************************
** 函数名:      IsvaidBidNumber
** 输 入:       无
** 输 出:       无
** 功能描述:    当前交易时段的有效价位个数买
** 全局变量:    无
** 调用模块:    无
** 作 者:       wugb
** 日 期:       2006.10.19
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
IsvaidBidNumber(STOCK_CS  *stock) {
    		BID_CS*		bid = NULL;
    		int 		vaidNumber=0;
    		bid = GetBid(stock->highestBid);
			if(bid==NULL){
				return vaidNumber;
			}
    		if(bid->orderNumber>0){
    				vaidNumber++;
    		}
    		
    		while((bid = GetBid(bid->next))!=NULL)
    		{
    			if(bid->orderNumber>0){
    				vaidNumber++;
    			}
    		}
    		
    		return vaidNumber;
}

/*****************************************************************
** 函数名:      IsvaidOfferNumber
** 输 入:       无
** 输 出:       无
** 功能描述:    当前交易时段的有效价位个数卖
** 全局变量:    无
** 调用模块:    无
** 作 者:       wugb
** 日 期:       2006.10.19
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
IsvaidOfferNumber(STOCK_CS  *stock) {
    		OFFER_CS*	offer = NULL;
    		int 		vaidNumber=0;
    		
    		offer = GetOffer(stock->lowestOffer);
			if(offer==NULL){
				return vaidNumber;
			}
    		if(offer->orderNumber>0){
    				vaidNumber++;
    		}
    		
    		while((offer = GetOffer(offer->next))!=NULL)
    		{
    			if(offer->orderNumber>0){
    				vaidNumber++;
    			}
    		}
    		return vaidNumber;
}


/*****************************************************************
** 函数名:      CheckTradePhase
** 输 入:       stock
** 输 出:       TRUE,FALSE
** 功能描述:    检测证券的交易时段是否需要可以发送（是否需要检测倒挂）
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2007.1.18
** 修 改:		
** 日 期:		
** 版本:        0.1
****************************************************************/
BOOL	
CheckTradePhase(STOCK_CS* stock){
	if(!(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_TRADE,strlen(STOCK_TRADING_RESTRICTION_TRADE))==0
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_BREAK,strlen(STOCK_TRADING_RESTRICTION_BREAK))==0 
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_HALT,strlen(STOCK_TRADING_RESTRICTION_HALT))==0 
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_SUSP,strlen(STOCK_TRADING_RESTRICTION_SUSP))==0 
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_CLOSE,strlen(STOCK_TRADING_RESTRICTION_CLOSE))==0
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_POSTR,strlen(STOCK_TRADING_RESTRICTION_POSTR))==0
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_ENDTR,strlen(STOCK_TRADING_RESTRICTION_ENDTR))==0
		||strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_BETW,strlen(STOCK_TRADING_RESTRICTION_BETW))==0)){
		return FALSE;
	}
	
	//当交易时段是 休市 时，如果前一个交易时段是盘前，不检查倒挂，不能发送
	if(strncmp(stock->prcsStsValCod, STOCK_TRADING_RESTRICTION_BREAK, strlen(STOCK_TRADING_RESTRICTION_BREAK))==0 &&
		strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_PRETR, strlen(STOCK_TRADING_RESTRICTION_PRETR)) == 0){
		return FALSE;
	}
	//当交易时段是停盘时，如果前一个交易时段是 开市集合竞价 或者 盘中集合竞价 时不检查倒挂，不能发送
	if(strncmp(stock->prcsStsValCod, STOCK_TRADING_RESTRICTION_HALT, strlen(STOCK_TRADING_RESTRICTION_HALT))==0 &&
		(strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_OCALL, strlen(STOCK_TRADING_RESTRICTION_OCALL)) == 0 ||
		strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_ICALL, strlen(STOCK_TRADING_RESTRICTION_ICALL)) == 0)){
		return FALSE;
	}
	//当交易时段是停牌时，如果前一个交易时段是 开市集合竞价 时不检查倒挂，不能发送
	if(strncmp(stock->prcsStsValCod, STOCK_TRADING_RESTRICTION_SUSP, strlen(STOCK_TRADING_RESTRICTION_SUSP))==0 &&
		strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_OCALL, strlen(STOCK_TRADING_RESTRICTION_OCALL)) == 0){
		return FALSE;
	}
	//当交易时段是 盘前、集合竞价期间、订单簿平衡前期、订单簿平衡期间 时不检查倒挂，不能发送，其他情况都要检查倒挂
	if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_PRETR, strlen(STOCK_TRADING_RESTRICTION_PRETR))!=0  && 
		strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_ICALL, strlen(STOCK_TRADING_RESTRICTION_ICALL))!=0 && 
		strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL, strlen(STOCK_TRADING_RESTRICTION_OCALL))!=0 &&
		strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IOBB, strlen(STOCK_TRADING_RESTRICTION_IOBB))!=0 &&
		strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IPOBB, strlen(STOCK_TRADING_RESTRICTION_IPOBB))!=0 &&
		strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OOBB, strlen(STOCK_TRADING_RESTRICTION_OOBB))!=0 &&
		strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OPOBB, strlen(STOCK_TRADING_RESTRICTION_OPOBB))!=0){
		return TRUE;
	}
	return FALSE;
}


/*****************************************************************
** 函数名:      IsPriceAcross
** 输 入:       stock
** 输 出:       TRUE(存在倒挂),FALSE(不存在倒挂)
** 功能描述:    检测证券中是否存在价格交叉（是否倒挂）
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2007.1.18
** 修 改:		
** 日 期:		
** 版本:        0.1
****************************************************************/
BOOL	
IsPriceAcross(STOCK_CS* stock){
	BID_CS* bid = NULL;
	OFFER_CS* offer = NULL;
	ORDER_CS* bOrder = NULL;
	ORDER_CS* sOrder = NULL;
	bid = GetBid(stock->highestBid);
	offer = GetOffer(stock->lowestOffer);
	
	//判断证券的交易时段是否需要检测倒挂
	if(!CheckTradePhase(stock)){
		return FALSE;
	}
	
	while(bid != NULL){
		if(bid->orderNumber > 0){
			break;
		}
		bid = GetBid(bid->next);
	}
	while(offer != NULL){
		if(offer->orderNumber > 0){
			break;
		}
		offer = GetOffer(offer->next);
	}
	if((bid != NULL) && (offer != NULL) && (bid->price >= offer->price)){
		//存在倒挂
		bOrder = GetOrder(bid->firstOrder);
		while(bOrder != NULL){
			if(IsTradingPhase(bOrder)){
				break;
			}
			bOrder = GetOrder(bOrder->nextPrice);
		}
		sOrder = GetOrder(offer->firstOrder);
		while(sOrder != NULL){
			if(IsTradingPhase(sOrder)){
				break;
			}
			sOrder = GetOrder(sOrder->nextPrice);
		}
		ERROR("价格倒挂","存在倒挂,时段[%s],isincod=[%s],买订单orderNo=[%lld]--trdResTypCod=[%s]---price=[%lld]--,卖订单orderNo=[%lld]--trdResTypCod=[%s]---price=[%lld]--"\
			,stock->prcsStsValCod,stock->isinCod,bOrder->orderNo,bOrder->trdResTypCod,bid->price,sOrder->orderNo,sOrder->trdResTypCod,offer->price);
		return TRUE;
	}
	return FALSE;
}


/*****************************************************************
** 函数名:      StockLock
** 输 入:       stock指针
** 输 出:       无
** 功能描述:    获取软件锁
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2009.7.24
** 修 改:		
** 日 期:		
** 版本:        0.1
****************************************************************/
void StockLock(STOCK_CS* stock, int process){
	int i = 0;
	/* struct timespec slptm;
	slptm.tv_sec = 0;
    slptm.tv_nsec = 50000;*/
	/*
	软件锁的原理：
	    本软件锁只适用于2个进程的情况。
	    给两个进程编号为0和1，同时对应的锁定标记为lockFlag[0]和lockFlag[1]，在公共区设置一个顺序标记turn表示获取锁
	    的进程。当进程需要获取软件锁时，它会把自身的锁定标识设置为TRUE，然后把公共区顺序标记改成自身进程标识（即0或1）
	    然后进入循环判断，满足条件的进程会进入下面的流程，不满足获取条件的进程会睡眠等待
	例如：
	    当某个进程要获取软件锁的时候，此时另外一个进程不需要锁定，因此锁定标记为FALSE，那么这个进程可以通过while循
	    环进入下面的处理流程
	    当0和1进程几乎同时要获取软件锁的时候，假设0进程先进入软件锁，那么turn的值会是1，当0进程进入while循环时会离
	    开循环进入后面处理流程，而1进程进入while循环时，只能在while循环里等待，直到0进程把自身锁定标记置为FALSE
	*/
	
	int other = 1 - process;
	stock->lockFLag[process] = TRUE;
	stock->turn = process;
	while(stock->turn == process && stock->lockFLag[other] == TRUE){
		i++;
		//nanosleep睡眠时间取决于系统时钟中断
		//nanosleep(&slptm, NULL);
		
		//usleep睡眠时间是毫秒级别
		usleep(50);
		if(i>200){
			stock->lockFLag[other] = FALSE;
			WARN(_moduleName, "第[%d]个进程等待时间超过200次循环，强行解锁", process);
			break;
		}
	}

	return;
}



/*****************************************************************
** 函数名:      StockUnlock
** 输 入:       stock指针
** 输 出:       无
** 功能描述:    释放软件锁
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2009.7.24
** 修 改:		
** 日 期:		
** 版本:        0.1
****************************************************************/
void StockUnlock(STOCK_CS* stock, int process){
	stock->lockFLag[process] = FALSE;
	return;
}


/*****************************************************************
** 函数名:      PrintStockInfoToFile
** 输 入:       文件描述符
** 输 出:       无
** 功能描述:    打印股票信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.14
** 修 改:		Li Bo
** 日 期:		2006.11.1
** 版本:        0.1
****************************************************************/
BOOL
PrintStockInfoToFile(int fp){
    char			buf[4096];
    STOCK_CS*		stock = NULL;
    int         	number = 0;
    int         	i = 0;
    char			moiInd = ' ';
    char			matchingPriority = ' ';
    char			bstOrdPrcInd = ' ';
	char			surplusBidAskIndicator = ' ';
#ifdef USE_PBU
	int				buyOrderListNum = -1;
	int				sellOrderListNum = -1;
	char			listNumTemp[8];
#endif
    number = GetStockNumber();

    sprintf(buf, "#****证券信息****\n#证券总数: %d\n\n", number);
    write(fp, buf, strlen(buf));

    for (i = 0; i < number; i++) {
        stock = GetStock(i);
		
		memset(buf, 0, 4096);

		if(stock->moiInd == 0){
			moiInd = ' ';
		}
		else{
			moiInd = stock->moiInd;
		}
		if(stock->matchingPriority == 0){
			matchingPriority = ' ';
		}
		else{
			matchingPriority = stock->matchingPriority;
		}
		if(stock->bstOrdPrcInd == 0){
			bstOrdPrcInd = ' ';
		}
		else{
			bstOrdPrcInd = stock->bstOrdPrcInd;
		}
		if(stock->surplusBidAskIndicator == 0){
			surplusBidAskIndicator = ' ';
		}
		else{
			surplusBidAskIndicator = stock->surplusBidAskIndicator;
		}
		
		#ifdef USE_PBU
			buyOrderListNum = stock->lastBuyOrderSortOffset;
			sellOrderListNum = stock->lastSellOrderSortOffset;
		#endif
		
        sprintf(buf, "stockNo=[%6d]--isinCod=[%12s]--isinIndex=[%6d]--stockId=[%8s]--stockShortName=[%12s]--setId=[%4d]--stockType=[%6d]--lotSize=[%9d]--currencyType=[%4s]--conSegCode=[%5s]--matchingPriority=[%c]--categoryNo=[%9d]--industryNo=[%9d]--openPrice=[%15lld]--closePrice=[%15lld]--prevClosingPrice=[%15lld]--highPrice=[%15lld]--lowPrice=[%15lld]--lastPrice=[%15lld]--netChg=[%15lld]--fixPrc=[%15lld]--bond.cashDate=[%9d]--bond.cashFrequency=[%9d]--bond.interestpaymentDate=[%9d]--bond.bondValue=[%9d]--bond.boundInterestRate=[%9d]--bond.startDate=[%9d]--bond.bondPublishPrice=[%9d]--bond.bondType=[%9d]--bond.preInterestpaymentDate=[%9d]--bond.nextInterestpaymentDate=[%9d]--bond.categoryType=[%9d]--bond.workDate=[%9d]--bond.yieldToMaturity=[%9d]--bond.preLastPrice=[%9d]--tradeNumber=[%9d]--tradeAmount=[%15lld]--tradeMoney=[%15lld]--marketTradeNumber=[%9d]--marketTradeAmount=[%15lld]--marketTradeMoney=[%15lld]--moiInd=[%c]--commonPrcsStsValCod=[%6s]--prcsStsValCod=[%6s]--prePrcsStsValCod=[%6s]--volaIndicator=[%3s]--bstOrdPrcInd=[%c]--iopv=[%15lld]--totWarExQty=[%15lld]--totWarClQty=[%15lld]--totWarWrQty=[%15lld]--lastTradeTime=[%9d]--openInterest=[%15lld]--deriLstStlPrc=[%15lld]--lstAuctPrc=[%15lld]--lstAuctQty=[%15lld]--surplusQty=[%15lld]--surplusBidAskIndicator=[%c]--orderBuyNumber=[%9d]--orderBuyAmount=[%15lld]--orderBuyMoney=[%15lld]--orderSellNumber=[%9d]--orderSellAmount=[%15lld]--orderSellMoney=[%15lld]--withdrawBuyNumber=[%9d]--withdrawBuyAmount=[%15lld]--withdrawBuyMoney=[%15lld]--withdrawSellNumber=[%9d]--withdrawSellAmount=[%15lld]--withdrawSellMoney=[%15lld]--bidNumber=[%9d]--offerNumber=[%9d]",
        	stock->stockNo,
			stock->isinCod,
			stock->isinIndex,
			stock->stockId,
			stock->stockShortName,
			stock->setId,
			stock->stockType,
			stock->lotSize,
			stock->currencyType,
			stock->conSegCode,
			matchingPriority,
			stock->categoryNo,
			stock->industryNo,
			stock->openPrice,
			stock->closePrice,
			stock->prevClosingPrice,
			stock->highPrice,
			stock->lowPrice,
			stock->lastPrice,
			stock->netChg,
			stock->fixPrc,
			stock->bond.cashDate,
			stock->bond.cashFrequency,
			stock->bond.interestpaymentDate,
			stock->bond.bondValue,
			stock->bond.boundInterestRate,
			stock->bond.startDate,
			stock->bond.bondPublishPrice,
			stock->bond.bondType,
			stock->bond.preInterestpaymentDate,
			stock->bond.nextInterestpaymentDate,
			stock->bond.categoryType,
			stock->bond.workDate,
			stock->bond.yieldToMaturity,
			stock->bond.preLastPrice,
			stock->tradeNumber,
			stock->tradeAmount,
			stock->tradeMoney,
			stock->marketTradeNumber,
        	stock->marketTradeAmount,
        	stock->marketTradeMoney,
			moiInd,
			stock->commonPrcsStsValCod,
			stock->prcsStsValCod,
			stock->prePrcsStsValCod,
			stock->volaIndicator,
			bstOrdPrcInd,
			stock->iopv,
			stock->totWarExQty,
			stock->totWarClQty,
			stock->totWarWrQty,
			stock->lastTradeTime,
			stock->openInterest,
			stock->deriLstStlPrc,
			stock->lstAuctPrc,
			stock->lstAuctQty,
			stock->surplusQty,
			surplusBidAskIndicator,
			stock->orderBuyNumber,
			stock->orderBuyAmount,
			stock->orderBuyMoney,
			stock->orderSellNumber,
			stock->orderSellAmount,
			stock->orderSellMoney,
			stock->withdrawBuyNumber,
			stock->withdrawBuyAmount,
			stock->withdrawBuyMoney,
			stock->withdrawSellNumber,
			stock->withdrawSellAmount,
			stock->withdrawSellMoney,
			stock->bidNumber,
			stock->offerNumber);
		
		#ifdef USE_PBU
			memset(listNumTemp, 0, 8);
			sprintf(listNumTemp, "%6d", stock->lastBuyOrderSortOffset);
			strcat(buf, "--PBUBuyOrderListNumber=[");
			strcat(buf, listNumTemp);
			strcat(buf, "]");
			memset(listNumTemp, 0, 8);
			sprintf(listNumTemp, "%6d", stock->lastSellOrderSortOffset);
			strcat(buf, "--PBUSellOrderListNumber=[");
			strcat(buf, listNumTemp);
			strcat(buf, "]");
		#endif
		strcat(buf, "\n");
		
        write(fp, buf, strlen(buf));
    }
    
    #ifdef USE_PBU
    	PBU_FIRM* pbu = NULL;
    	for(i = 0; i < GetPBUNumber(); i++){
    		pbu = GetPBUFirmByPbuNo(i);
    		memset(buf, 0, 4096);
    		sprintf(buf, "pbuNo=[%d]--pbuFirm=[%s]\n", i, pbu->pbuFirm);
    		write(fp, buf, strlen(buf));
    	}
    #endif 
    return TRUE;
}         
          
          
/*****************************************************************
** 函数名:      PrintStockToFile
** 输 入:       文件描述符
** 输 出:       无
** 功能描述:    打印快照信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.11.1
** 修 改:	  	
** 日 期:		
** 版本:        0.1
****************************************************************/
BOOL      
PrintStockToFile(){
    char	  		buf[2048];
	int		  		fp = 0;
    char	  		filePath[256];
    char			stockType = 0;
    char	    		industryId[INDUSTRY_ID_LENGTH];
    STOCK_CS*		stock = NULL;
    INDUSTRY_CS*	industry = NULL;
    int		  		i = 0;
          
	if (!IsHomeValid()) {
        return FALSE;
    }     
          
    sprintf(filePath, "%s/StockInfoSnapshot%s.txt", GetHome(), GetTime());
          
	if ((fp = open(filePath, O_WRONLY | O_CREAT | O_APPEND,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
        printf("ERROR: 创建文件[%s]失败! \n", filePath);
        return FALSE;
    }     
          
    for (i = 0; i < GetStockNumber(); i++) {
        stock = GetStock(i);
        memset(industryId, 0, INDUSTRY_ID_LENGTH);
        if(IsAShare(stock)){
        	  stockType = 'A';
        	  industry = GetIndustry(stock->industryNo, CATEGORY_ID_ASHARE);
        	  strcpy(industryId, industry->industryId);
        } 
    	else if(IsBShare(stock)){
    		  stockType = 'B';
    		  industry = GetIndustry(stock->industryNo, CATEGORY_ID_BSHARE);
    		  strcpy(industryId, industry->industryId);
    	} 
    	else{
    		  stockType = 'O';
    		  strcpy(industryId, " ");
    	} 
        memset(buf, 0, 2048);
        sprintf(buf,"%12s|%c|%4s|%15lld|%15lld|%15lld|%15lld|%15lld|%15lld|%15lld|%15lld|\n",
        	stock->isinCod,
        	stockType,
        	industryId,
        	stock->orderBuyAmount,
        	stock->orderBuyMoney,
        	stock->orderSellAmount,
        	stock->orderSellMoney,
        	stock->tradeAmount,
        	stock->tradeMoney,
        	stock->marketTradeAmount,
        	stock->marketTradeMoney
        	);
        write(fp, buf, strlen(buf));
    }
    close(fp);
    return TRUE;
}


/*****************************************************************
** 函数名:      PrintInsideMarketToFile
** 输 入:       无
** 输 出:       TRUE FALSE
** 功能描述:    打印市场行情信息到指定的文件
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.10.9
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
PrintInsideMarketToFile(){
    char        buf[2048];
	int     	fp = 0;
    char    	filePath[256];
    STOCK_CS*	stock = NULL;
    char		moiInd;
    char		bstOrdPrcInd;
    char		surplusBidAskIndicator;
    int			i = 0;

	if (!IsHomeValid()) {
        return FALSE;
    }
    
    sprintf(filePath, "%s/InsideMarketSnapshot%s.txt", GetHome(), GetTime());
    
	if ((fp = open(filePath, O_WRONLY | O_CREAT | O_APPEND,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
        printf("ERROR: 创建文件[%s]失败! \n", filePath);
        return FALSE;
    }
	
	for (i = 0; i < GetStockNumber(); i++) {
        stock = GetStock(i);
        memset(buf, 0, 2048);
        if(stock->moiInd == 0){
        	moiInd = ' ';
        }
        else{
        	moiInd = stock->moiInd;
        }
        if(stock->bstOrdPrcInd == 0){
        	bstOrdPrcInd = ' ';
        }
        else{
        	bstOrdPrcInd = stock->bstOrdPrcInd;
        }
        if(stock->surplusBidAskIndicator == 0){
        	surplusBidAskIndicator = ' ';
        }
        else {
        	surplusBidAskIndicator = stock->surplusBidAskIndicator;
        }
        sprintf(buf,"%12s|%15lld|%15lld|%15lld|%15lld|%15lld|%15lld|%15lld|%15d|%15lld|%15lld|%c|%5s|%c|%15lld|%15lld|%15lld|%15lld|%15lld|%15lld|%15lld|%15lld|%15lld|%c|\n",
        	stock->isinCod,
        	stock->prevClosingPrice,
        	stock->openPrice,
        	stock->highPrice,
        	stock->lowPrice,
        	stock->lastPrice,
        	stock->netChg,
        	stock->fixPrc,
        	stock->tradeNumber,
        	stock->tradeAmount,
        	stock->tradeMoney,
        	moiInd,
        	stock->prcsStsValCod,
        	bstOrdPrcInd,
        	stock->iopv,
        	stock->totWarExQty,
        	stock->totWarClQty,
        	stock->totWarWrQty,
        	stock->openInterest,
        	stock->deriLstStlPrc,
        	stock->lstAuctPrc,
        	stock->lstAuctQty,
        	stock->surplusQty,
        	surplusBidAskIndicator
        	);
        write(fp, buf, strlen(buf));
    }
    close(fp);
    return TRUE;
}


/*****************************************************************
** 函数名:      PrintShow2007SnapshotToFile
** 输 入:       无
** 输 出:       TRUE FALSE
** 功能描述:    打印show2007快照信息到指定的文件
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.10.9
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL		
PrintShow2007SnapshotToFile(){
	char        buf[4096];
	char		buf1[2048];
	char		bufTemp[256];
	int     	fp = 0;
    char    	filePath[256];
    STOCK_CS*	stock = NULL;
    int			i = 0;
    int			j = 0;
    BID_CS*		bid = NULL;
    OFFER_CS*	offer = NULL;
    char		surplusBidAskIndicator = 0;

	if (!IsHomeValid()) {
        return FALSE;
    }
    
    sprintf(filePath, "%s/Show2007Snapshot%s.txt", GetHome(), GetTime());
    
	if ((fp = open(filePath, O_WRONLY | O_CREAT | O_APPEND,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
        printf("ERROR: 创建文件[%s]失败! \n", filePath);
        return FALSE;
    }
	
	for (i = 0; i < GetStockNumber(); i++) {
        stock = GetStock(i);

        bid = GetBid(stock->highestBid);
        offer = GetOffer(stock->lowestOffer);
        memset(buf1, 0, 2048);
        for(j = 0; j < 10; j++){
        	memset(bufTemp, 0, 256);
        	if((bid == NULL) && (offer == NULL)){
        		sprintf(bufTemp,"               |               |               |               |               |               |");
        	}
        	else if((bid == NULL) && (offer != NULL)){
        		sprintf(bufTemp,"               |%15lld|               |%15lld|               |%15d|",offer->price, offer->orderAmount, offer->orderNumber);
        		offer = GetOffer(offer->next);
        	}
        	else if((bid != NULL) && (offer == NULL)){
        		sprintf(bufTemp,"%15lld|               |%15lld|               |%15d|               |",bid->price, bid->orderAmount, bid->orderNumber);
        		bid = GetBid(bid->next);
        	}
        	else if((bid != NULL) && (offer != NULL)){
        		sprintf(bufTemp,"%15lld|%15lld|%15lld|%15lld|%15d|%15d|",bid->price, offer->price, bid->orderAmount, offer->orderAmount, bid->orderNumber, offer->orderNumber);
        		offer = GetOffer(offer->next);
        		bid = GetBid(bid->next);
        	}
        	strcat(buf1, bufTemp);
        }
        
        memset(buf, 0, 4096);
        if(stock->surplusBidAskIndicator == 0){
        	surplusBidAskIndicator = ' ';
        }
        else{
        	surplusBidAskIndicator = stock->surplusBidAskIndicator;
        }
        sprintf(buf,"%12s|%8s|%12s| | |%15lld|%15lld| |%15d|%15lld| |%15lld|%15lld|%15lld|%15lld| |%15lld| | | |%c| |%5s| | | | |%16lld| |%16lld|%16lld|%16lld|%16lld|%s\n",
        	stock->isinCod,
        	stock->stockId,
        	stock->stockShortName,
        	stock->netChg,
        	stock->lastPrice,
        	stock->lastTradeTime,
        	stock->prevClosingPrice,
        	stock->openPrice,
        	stock->fixPrc,
        	stock->lowPrice,
        	stock->highPrice,
        	stock->lstAuctQty,
        	surplusBidAskIndicator,
        	stock->prcsStsValCod,
        	stock->openInterest,
        	stock->iopv,
        	stock->totWarExQty,
        	stock->totWarClQty,
        	stock->totWarWrQty,
        	buf1
        	);
        write(fp, buf, strlen(buf));
    }
    close(fp);
    return TRUE;
}


/*****************************************************************
** 函数名:      PrintStock
** 输 入:       stock
** 输 出:       无
** 功能描述:    打印证券信息到日志中
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void		
PrintStock(STOCK_CS* stock){
	char moiInd;
	
	if(stock == NULL){
		return;
	}
	
	if(stock->moiInd == 0){
		moiInd = ' ';
	}
	else{
		moiInd = stock->moiInd;
	}
	
	TRACE(_moduleName, "stockNo=[%6d]--isinCod=[%12s]--isinIndex=[%6d]--stockId=[%8s]--stockShortName=[%12s]--stockType=[%6d]--lotSize=[%9d]--currencyType=[%4s]--conSegCode=[%5s]--matchingPriority=[%c]--categoryNo=[%9d]--industryNo=[%9d]--openPrice=[%15lld]--closePrice=[%15lld]--prevClosingPrice=[%15lld]--highPrice=[%15lld]--lowPrice=[%15lld]--lastPrice=[%15lld]--netChg=[%15lld]--fixPrc=[%15lld]--bond.cashDate=[%9d]--bond.cashFrequency=[%9d]--bond.interestpaymentDate=[%9d]--bond.bondValue=[%9d]--bond.boundInterestRate=[%9d]--bond.startDate=[%9d]--bond.bondPublishPrice=[%9d]--bond.bondType=[%9d]--bond.preInterestpaymentDate=[%9d]--bond.nextInterestpaymentDate=[%9d]--bond.categoryType=[%9d]--bond.workDate=[%9d]--bond.yieldToMaturity=[%9d]--bond.preLastPrice=[%9d]--tradeNumber=[%9d]--tradeAmount=[%15lld]--tradeMoney=[%15lld]--marketTradeNumber=[%9d]--marketTradeAmount=[%15lld]--marketTradeMoney=[%15lld]--moiInd=[%c]--prcsStsValCod=[%6s]--volaIndicator=[%3s]--bstOrdPrcInd=[%c]--iopv=[%15lld]--totWarExQty=[%15lld]--totWarClQty=[%15lld]--totWarWrQty=[%15lld]--lastTradeTime=[%9lu]--openInterest=[%15lld]--deriLstStlPrc=[%15lld]--lstAuctPrc=[%15lld]--lstAuctQty=[%15lld]--surplusQty=[%15lld]--surplusBidAskIndicator=[%c]--orderBuyNumber=[%9d]--orderBuyAmount=[%15lld]--orderBuyMoney=[%15lld]--orderSellNumber=[%9d]--orderSellAmount=[%15lld]--orderSellMoney=[%15lld]--withdrawBuyNumber=[%9d]--withdrawBuyAmount=[%15lld]--withdrawBuyMoney=[%15lld]--withdrawSellNumber=[%9d]--withdrawSellAmount=[%15lld]--withdrawSellMoney=[%15lld]--bidNumber=[%9d]--offerNumber=[%9d]\n",
        	stock->stockNo,
			stock->isinCod,
			stock->isinIndex,
			stock->stockId,
			stock->stockShortName,
			stock->stockType,
			stock->lotSize,
			stock->currencyType,
			stock->conSegCode,
			stock->matchingPriority,
			stock->categoryNo,
			stock->industryNo,
			stock->openPrice,
			stock->closePrice,
			stock->prevClosingPrice,
			stock->highPrice,
			stock->lowPrice,
			stock->lastPrice,
			stock->netChg,
			stock->fixPrc,
			stock->bond.cashDate,
			stock->bond.cashFrequency,
			stock->bond.interestpaymentDate,
			stock->bond.bondValue,
			stock->bond.boundInterestRate,
			stock->bond.startDate,
			stock->bond.bondPublishPrice,
			stock->bond.bondType,
			stock->bond.preInterestpaymentDate,
			stock->bond.nextInterestpaymentDate,
			stock->bond.categoryType,
			stock->bond.workDate,
			stock->bond.yieldToMaturity,
			stock->bond.preLastPrice,
			stock->tradeNumber,
			stock->tradeAmount,
			stock->tradeMoney,
			stock->marketTradeNumber,
        	stock->marketTradeAmount,
        	stock->marketTradeMoney,
			moiInd,
			stock->prcsStsValCod,
			stock->volaIndicator,
			stock->bstOrdPrcInd,
			stock->iopv,
			stock->totWarExQty,
			stock->totWarClQty,
			stock->totWarWrQty,
			stock->lastTradeTime,
			stock->openInterest,
			stock->deriLstStlPrc,
			stock->lstAuctPrc,
			stock->lstAuctQty,
			stock->surplusQty,
			stock->surplusBidAskIndicator,
			stock->orderBuyNumber,
			stock->orderBuyAmount,
			stock->orderBuyMoney,
			stock->orderSellNumber,
			stock->orderSellAmount,
			stock->orderSellMoney,
			stock->withdrawBuyNumber,
			stock->withdrawBuyAmount,
			stock->withdrawBuyMoney,
			stock->withdrawSellNumber,
			stock->withdrawSellAmount,
			stock->withdrawSellMoney,
			stock->bidNumber,
			stock->offerNumber
		);
}
