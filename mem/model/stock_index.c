/******************************************************************
** 文件名:      stock_index.c
** 版 本:       0.1
** 创建人:      Li Bo
** 日 期:       2006.8.30
** 描 述:       股票指数管理函数
** 修订记录：
** 修订日期     修订人      SIR         修复的bug或者修订原因
**-----------------------------------------------------------------
** 2011.5.10    wangyx      00016       增加今收盘指数
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/stock_index.c,v 1.1 2012/03/29 01:19:54 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:54 $
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include	<unistd.h>

#include	"stock_index.h"
#include    "../memory/memory.h"
#include    "../../util/log/log.h"


/*
 * 常量定义
 */
 
/*
 * 模块名称
 */
static const char   _moduleName[] = "stock_index";

/*
 * 属性
 */


/*
 * 内部函数声明
 */
 
/*****************************************************************
** 函数名:      GetStockIndexHead
** 输 入:		无
** 输 出:       股票指数存储首地址
** 功能描述:    返回股票指数存储首地址
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.8.30
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
STOCK_INDEX_CS* 
GetStockIndexHead(){
	return (STOCK_INDEX_CS*) GetShareMemory() + GetShareMemAllocationStats()->stockIndexOffset;
}


/*****************************************************************
** 函数名:      GetStockIndex
** 输 入:		index
** 输 出:       股票指数存储地址
** 功能描述:    返回指定位置的STOCK_INDEX_CS
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.8.30
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
STOCK_INDEX_CS*		
GetStockIndex(int index){
	return GetStockIndexHead() + index;
}


/*****************************************************************
** 函数名:      GetStockIndexByIsinCod
** 输 入:		股票分类代码
** 输 出:       股票指数存储地址
** 功能描述:    返回股票指数存储地址
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.8.30
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
STOCK_INDEX_CS* 
GetStockIndexByIsinCod(char* isinCod){
	int i = 0;
	STOCK_INDEX_CS* pStockIndex = NULL;
	
	pStockIndex = GetStockIndexHead();
	
	if(pStockIndex == NULL){
		return NULL;
	}
	
	for(i = 0; i < GetShareMemCurrentStats()->nextStockIndex; i++,pStockIndex++){
		if(strcmp(isinCod, pStockIndex->detailedIndex.isinCod) == 0){
			return pStockIndex;
		}
	}
	
	return NULL;
}

/*****************************************************************
** 函数名:      GetStockIndexNumber
** 输 入:       无
** 输 出:       StockIndex总数
** 功能描述:    返回StockIndex数量
** 全局变量:    无
** 调用模块:
** 作 者:       wugb
** 日 期:       2006.9.5
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因

****************************************************************/
int
GetStockIndexNumber() {
    return GetShareMemCurrentStats()->nextStockIndex;
}  


/*****************************************************************
** 函数名:      AddStockIndexInfo
** 输 入:       指数信息
** 输 出:       添加到共享内存的位置
** 功能描述:    添加指数信息到共享内存
** 全局变量:    无
** 调用模块:    无
** 作 者:       Li Bo
** 日 期:       2006.9.4
** 修 改:       
** 日 期:       
** 版本:        0.1
****************************************************************/
int	
AddStockIndexInfo(STOCK_INFO* pStockIndexInfo){
	STOCK_INDEX_CS		*target = NULL;
    int         		index   = GetShareMemCurrentStats()->nextStockIndex;

    //检查内存是否越界
    if (index >= GetShareMemConfigStats()->stockIndexMax) {
        ERROR(_moduleName, "添加指数isinCod=[%s]时失败，指数数量超出预定义的最大指数数量！ Current Number: %d"
                ,pStockIndexInfo->isinCod, index);
        return -1;
    }
    
    target = GetStockIndex(index);
    
    memcpy(target->detailedIndex.isinCod, pStockIndexInfo->isinCod, STOCK_ISIN_CODE_LENGTH);
    memcpy(target->indexCode, pStockIndexInfo->stockId, STOCK_ID_LENGTH);
    
    GetShareMemCurrentStats()->nextStockIndex ++;
	return index;
}


/*****************************************************************
** 函数名:      UpdateDetailedIndex
** 输 入:       需要更新的指数信息
** 输 出:       TRUE FALSE
** 功能描述:    向共享内存中更新指数信息
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.6
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因

****************************************************************/
BOOL 
UpdateDetailedIndex(DETAILED_INDEX* pStockIndex){
	STOCK_INDEX_CS	*target = NULL;
    
    if(pStockIndex == NULL){
    	ERROR(_moduleName, "UpdateDetailedIndex 输入参数不能为NULL");
    	return FALSE;	
    }
    
    target = GetStockIndexByIsinCod(pStockIndex->isinCod);
    
    if(target == NULL){
    	ERROR(_moduleName, "UpdateDetailedIndex 未找到对应的指数信息 isinCod=[%s]", pStockIndex->isinCod);
    	return FALSE;
    }
    
    memcpy(&(target->detailedIndex), pStockIndex, sizeof(DETAILED_INDEX));
    /*target->detailedIndex.indexHigh /= ONE_HUNDRAD;
    target->detailedIndex.indexLow /= ONE_HUNDRAD;
    target->detailedIndex.latestIndex /= ONE_HUNDRAD;*/
    if(target->detailedIndex.latestClosingIndex < 0){
    	target->detailedIndex.latestClosingIndex = 0;
    }
    if(target->detailedIndex.openingIndex < 0){
    	target->detailedIndex.openingIndex = 0;
    }
    if(target->detailedIndex.tradeMoney < 0){
    	target->detailedIndex.tradeMoney = 0;
    }
    if(target->detailedIndex.indexHigh < 0){
    	target->detailedIndex.indexHigh = 0;
    }
    if(target->detailedIndex.indexLow < 0){
    	target->detailedIndex.indexLow = 0;
    }
    if(target->detailedIndex.latestIndex < 0){
    	target->detailedIndex.latestIndex = 0;
    }
/*** SIR 00016 BEGIN */	
	if(target->detailedIndex.closeIndex < 0 ){
		target->detailedIndex.closeIndex = 0;
	}
/*** SIR 00016 END */	
    if(target->detailedIndex.tradeAmount < 0){
    	target->detailedIndex.tradeAmount = 0;
    }
    if(target->detailedIndex.netChg == -9999999999999ll){
    	target->detailedIndex.netChg = 0;
    }
    if(target->detailedIndex.ratioChg == -9999999999999ll){
    	target->detailedIndex.ratioChg = 0;
    }

    return TRUE;
}



/*****************************************************************
** 函数名:      PrintStockIndex
** 输 入:       stockIndex
** 输 出:       无
** 功能描述:    输出指数信息
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.8
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因

****************************************************************/
void 
PrintStockIndex(STOCK_INDEX_CS* stockIndex){
	if(stockIndex == NULL){
		return;
	}
	
	TRACE(_moduleName, "[STOCK_INDEX_CS]--isinCod=[%s]--indexCode=[%s]--latestClosingIndex=[%lld]--openingIndex=[%lld]--tradeMoney=[%lld]--indexHigh=[%lld]--indexLow=[%lld]--latestIndex=[%lld]--tradeTime=[%s]--tradeAmount=[%lld]--netChg=[%lld]--ratioChg=[%lld]", 
		stockIndex->detailedIndex.isinCod, 
		stockIndex->indexCode,
		stockIndex->detailedIndex.latestClosingIndex,
		stockIndex->detailedIndex.openingIndex,
		stockIndex->detailedIndex.tradeMoney,
		stockIndex->detailedIndex.indexHigh,
		stockIndex->detailedIndex.indexLow,
		stockIndex->detailedIndex.latestIndex,
		stockIndex->detailedIndex.tradeTime,
		stockIndex->detailedIndex.tradeAmount,
		stockIndex->detailedIndex.netChg,
		stockIndex->detailedIndex.ratioChg);
}



/*****************************************************************
** 函数名:      PrintStockIndexToFile
** 输 入:       fp
** 输 出:       无
** 功能描述:    输出指数信息
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.12.8
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因

****************************************************************/
void 
PrintStockIndexToFile(int fp){
	char			buf[4096];
    STOCK_INDEX_CS*	stockIndex = NULL;
    int         	number = 0;
    int         	i = 0;

    number = GetStockIndexNumber();

    sprintf(buf, "#****指数信息****\n#指数总数: %d\n\n", number);
    write(fp, buf, strlen(buf));
    
    for (i = 0; i < number; i++) {
        stockIndex = GetStockIndex(i);
		
		memset(buf, 0, 4096);
    	sprintf(buf, "isinCod=[%16s]--indexCode=[%16s]--latestClosingIndex=[%16lld]--openingIndex=[%16lld]--tradeMoney=[%16lld]--indexHigh=[%16lld]--indexLow=[%16lld]--latestIndex=[%16lld]--tradeTime=[%16s]--tradeAmount=[%16lld]--netChg=[%16lld]--ratioChg=[%16lld]\n", 
			stockIndex->detailedIndex.isinCod, 
			stockIndex->indexCode,
			stockIndex->detailedIndex.latestClosingIndex,
			stockIndex->detailedIndex.openingIndex,
			stockIndex->detailedIndex.tradeMoney,
			stockIndex->detailedIndex.indexHigh,
			stockIndex->detailedIndex.indexLow,
			stockIndex->detailedIndex.latestIndex,
			stockIndex->detailedIndex.tradeTime,
			stockIndex->detailedIndex.tradeAmount,
			stockIndex->detailedIndex.netChg,
			stockIndex->detailedIndex.ratioChg);
    	write(fp, buf, strlen(buf));
    }
}
