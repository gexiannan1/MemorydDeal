/******************************************************************
** 文件名:      inside_market.c
** 版 本:       0.1
** 创建人:      Li Bo
** 日 期:       2006.9.7
** 描 述:       市场行情信息管理函数
** 修订记录：
** 修订日期     修订人    SIR    修复的bug或者修订原因
**-----------------------------------------------------------------
** 2012.12.12   LIBO      00380  券种空指针调用导致进程退出
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/inside_market.c,v 1.2 2012/12/12 03:27:22 tc\boli Exp $
 * $Revision: 1.2 $
 * $Date: 2012/12/12 03:27:22 $
 */

#include    <string.h>
#include	"inside_market.h"
#include	"model_length_define.h"
#include	"category.h"
#include    "../memory/memory.h"
#include    "../../util/log/log.h"



/*
 * 常量定义
 */
 
/*
 * 模块名称
 */
static const char   _moduleName[] = "inside_market";

/*
 * 属性
 */


/*
 * 内部函数声明
 */
 
/*****************************************************************
** 函数名:      UpdateInsideMarket
** 输 入:       需要更新的市场行情信息
** 输 出:       更新到共享内存的位置
** 功能描述:    向共享内存中更新市场行情信息
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.6
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2009.11.26	Li Bo		
****************************************************************/
int 
UpdateInsideMarket(INSIDE_MARKET* pInsideMarket){
	STOCK_CS* target = NULL;
/****SIR 00380 BEGIN ****/
	CATEGORY_CS* pCategory = NULL;
/****SIR 00380 END   ****/
	

	target = GetStockByIsinCod(pInsideMarket->isinCod);

	if(target == NULL){
		ERROR(_moduleName, "从共享内存获得Stock信息失败isinCod=[%s]", pInsideMarket->isinCod);
		return -1;
	}

	target->volaIndicator[0]		= pInsideMarket->volInd;
	if(pInsideMarket->openPrice > 0){
		target->openPrice = pInsideMarket->openPrice / ONE_HUNDRAD;
	}
	if(pInsideMarket->closePrice > 0){
		target->closePrice = pInsideMarket->closePrice / ONE_HUNDRAD;
		//2011.5.31 公共行情不再更新最新价
		//target->lastPrice = target->closePrice;
	}
	if(pInsideMarket->prevClosingPrice > 0){
		target->prevClosingPrice = pInsideMarket->prevClosingPrice / ONE_HUNDRAD;
	}
	if(pInsideMarket->fixPrc > 0){
		target->fixPrc = pInsideMarket->fixPrc / ONE_HUNDRAD;
	}
	target->moiInd					= pInsideMarket->moiInd;
	target->bstOrdPrcInd			= pInsideMarket->bstOrdPrcInd;		
	target->openInterest			= pInsideMarket->openInterest;
	target->deriLstStlPrc			= pInsideMarket->deriLstStlPrc;

	/*
	 * 2006.11.26
	 * 增加公共行情市场状态读取，如果虚拟集合竞价价格不大于0，把最新虚拟集合竞价价格赋值为0
	 */
	memset(target->commonPrcsStsValCod, 0, 6);
	memcpy(target->commonPrcsStsValCod, pInsideMarket->prcsStsValCod, 6);
	if(pInsideMarket->virtualOpenPrice > 0){
		target->lstAuctPrc = pInsideMarket->virtualOpenPrice / ONE_HUNDRAD;
	}
	else{
		target->lstAuctPrc = 0;
	}
	
	target->lstAuctQty				= pInsideMarket->virtualMatchAmount;
	target->surplusQty				= pInsideMarket->virtualUnMatchAmount;
	target->surplusBidAskIndicator	= pInsideMarket->srpBidAskInd;
	
	if((pInsideMarket->highPrice > 0) && (target->highPrice < pInsideMarket->highPrice / ONE_HUNDRAD)){
		target->highPrice = pInsideMarket->highPrice / ONE_HUNDRAD;
	}
	if((target->lowPrice == 0) && (pInsideMarket->lowPrice > 0)){
		target->lowPrice = pInsideMarket->lowPrice / ONE_HUNDRAD;
	}
	else if((pInsideMarket->lowPrice > 0) && (target->lowPrice > pInsideMarket->lowPrice / ONE_HUNDRAD)){
		target->lowPrice = pInsideMarket->lowPrice / ONE_HUNDRAD;
	}
/****SIR 00380 BEGIN ****/	
	pCategory = GetCategory(target->categoryNo);
	if(pCategory)
	{
		if(pCategory->id == CATEGORY_ID_ETF){
			if(pInsideMarket->iopv > 0){
				target->iopv = pInsideMarket->iopv / ONE_HUNDRAD;
			}
		}
		else if (pCategory->id == CATEGORY_ID_WARRANT){
			target->totWarExQty = pInsideMarket->totWarExQty;
			target->totWarClQty = pInsideMarket->totWarClQty / ONE_HUNDRAD;
			target->totWarWrQty = pInsideMarket->totWarWrQty / ONE_HUNDRAD;
		}
	}
/****SIR 00380 END   ****/	
	/////////////////////////
	//PrintStock(target);
	/////////////////////////
	return target - GetStockList();
}
