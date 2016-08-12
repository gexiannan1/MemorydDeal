/******************************************************************
** �ļ���:      stock.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.9.6
** �� ��:       Stock��Ϣ����
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
** 2006.1.4     Song Fang   ���ӷ��ص���ģʽ�ĳɽ���Ϣ�Ľӿ�
** 2006.03.23   Wei Huanyin 1������lastPrice�ֶδ���
** 2006.04.15   Wei Huanyin 1�����ӵ���ծȯ��̬���ݱ�־������
** 2006.05.30   Wei Huanyin 1�����Ӽ��Ͼ��۽���ʱ�������
** 2006.06.16   zhuhui		1�����Ӹ������⼯�Ͼ�����Ϣ����
** 2007.04.04	Li Bo		1����PrintStockInfoToFile����������setId��Ϣ���
							2����AddStock����������setId�Ĵ���
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
 * ģ������
 */
static const char   _moduleName[] = "stock";

/*
 * �ڲ���������
 */
static int CompIsinCod(STOCK_CS *p1, STOCK_CS *p2);					/* ���ַ���Ҫ�ıȽϺ��� */


/*****************************************************************
** ������:      GetStockList
** �� ��:       ��
** �� ��:       STOCK_CS[]
** ��������:    ���ع����ڴ���Stock�������ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
STOCK_CS*
GetStockList() {
    return (STOCK_CS*) GetShareMemory() + GetShareMemAllocationStats()->stockOffset;
}


/*****************************************************************
** ������:      GetStock
** �� ��:       ��
** �� ��:       STOCK_CS*
** ��������:    ����ָ��λ�õ�STOCK_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
STOCK_CS*
GetStock(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->stockMax) {
        return NULL;
    }
    return GetStockList() + index;
}


/*****************************************************************
** ������:      GetStockByIsinCod
** �� ��:       isinCod
** �� ��:       STOCK_CS*
** ��������:    ����ָ��isinCod��Ӧ��STOCK_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
STOCK_CS*
GetStockByIsinCod(char *isinCod) {
    STOCK_CS    *stock  = NULL;
    STOCK_CS	stockCmp;
    
 /*   if(strlen(isinCod) != STOCK_ISIN_CODE_LENGTH - 1){
    	ERROR(_moduleName, "������Ʊʧ�ܣ���ƱisinCod=[%s]����[%d]����", isinCod, strlen(isinCod));
    	return NULL;
    }*/
    
    memcpy(stockCmp.isinCod, isinCod, STOCK_ISIN_CODE_LENGTH);
    
    stock = (STOCK_CS*)bsearch(&stockCmp, GetStockList(),
		GetStockNumber(), sizeof(STOCK_CS), (int(*)(const void *,const void *))CompIsinCod);
    printf("stock   %0x  \n", stock);

    return stock;
}


/*****************************************************************
** ������:      GetStockByIsinIndex
** �� ��:       isinIndex
** �� ��:       STOCK_CS*
** ��������:    ����ָ��isinIndex��Ӧ��STOCK_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.26
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      GetStockByStockId
** �� ��:       ��
** �� ��:       STOCK_CS*
** ��������:    ����ָ��stockId��Ӧ��STOCK_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.7
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      CompIsinCod
** �� ��:       STOCK_CS
** �� ��:       int
** ��������:    ���ַ���Ҫ�ıȽϺ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.13
** �� ��:		Li Bo 
** �� ��:		2009.11.12
** �汾:        0.1
** �޸����ݣ�	�ȽϺ�����strcmp�ĳ�strncmp
****************************************************************/
static int 
CompIsinCod(STOCK_CS *p1, STOCK_CS *p2){ 
	return strncmp(p1->isinCod, p2->isinCod, STOCK_ISIN_CODE_LENGTH - 1);
}

/*****************************************************************
** ������:      GetStockNumber
** �� ��:       ��
** �� ��:       Stock����
** ��������:    ����Stock����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetStockNumber() {
    return GetShareMemCurrentStats()->nextStock;
}


/*****************************************************************
** ������:      AddStock
** �� ��:       ��
** �� ��:       ������Stock��λ��
** ��������:    ��Stock����������Stock��Ŀ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
AddStock(STOCK_INFO *pStockInfo) {
    STOCK_CS    *target = NULL;
    int         index   = GetShareMemCurrentStats()->nextStock;
    int			insertIndex = 0;
	int			i		= 0;
	
    /*
     * ����ڴ��Ƿ�Խ��
     */
    if (index >= GetShareMemConfigStats()->stockMax) {
        FATAL("Stock.AddStock()", "֤ȯ��������Ԥ��������֤ȯ������ Current Number: %d"
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
     * ��ʼ��Stock ָ�� �� ���⾺������ӯ�෽��
     */
    target->surplusBidAskIndicator = ' ';
    /*�������ʱ��ʼ��Ϊ-1�������ɽ�ʱ����Ϊ0��������ȫ�ɽ�ʱ������ɽ���ʱ*/
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
** ����:      UpdateStockCategory
** �� ��:       STOCK_CATEGORY_MON*
** �� ��:       BOOL
** ��������:    ����֤ȯƷ������
** ȫ�����:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.29
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
UpdateStockCategory(STOCK_CATEGORY_MON *stockCategory) {
    //STOCK_CS    *stock  = NULL;

    //if (stockCategory && (stock = GetStockById(stockCategory->stockId))) {
        /*
         * ����֤ȯƷ���б�
         */
        //AddCategoryStock(stockCategory->categoryId, stock);
        //return TRUE;
    //}
    return FALSE;
}


/*****************************************************************
** ������:      UpdateStockIndustry
** �� ��:       STOCK_INDUSTRY*��shareType
** �� ��:       BOOL
** ��������:    ����֤ȯ��ҵ����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.10.13
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ������ҵ����A��B��
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
** ������:      IsShare
** �� ��:       stockNo
** �� ��:       BOOL
** ��������:    ����stockNo��Ӧ��֤ȯ�����Ƿ��ǹ�Ʊ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsAShare
** �� ��:       stockNo
** �� ��:       BOOL
** ��������:    ����stockNo��Ӧ��֤ȯ�����Ƿ���A��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsBShare
** �� ��:       stockNo
** �� ��:       BOOL
** ��������:    ����stockNo��Ӧ��֤ȯ�����Ƿ���B��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsIndex
** �� ��:       stockNo
** �� ��:       BOOL
** ��������:    ����stockNo��Ӧ��֤ȯ�����Ƿ���ָ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsIndex(STOCK_CS *stock) {
    if (stock) {
        return stock->stockType == STOCK_TYPE_INDEX;
    }
    return FALSE;
}


/*****************************************************************
** ������:      IsValidStock
** �� ��:       stockNo
** �� ��:       BOOL
** ��������:    ����stockNo��Ӧ��֤ȯ�Ƿ���Ч��ƷƷ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Wei Huanyin
** �� ��:       2006.03.16
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsValidStock(STOCK_CS *stock) {
    if (stock) {
        return stock->categoryNo >= 0;
    }
    return FALSE;
}


/*****************************************************************
** ������:      IsStockReceived
** �� ��:       ��
** �� ��:       BOOL
** ��������:    ���ع�Ʊ�Ƿ���ȫ���������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.11.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsStockReceived() {
    return GetMemTradeStats()->isStockReceived == 1;
}


/*****************************************************************
** ������:      SetStockReceived
** �� ��:       BOOL
** �� ��:       ��
** ��������:    ���ù�Ʊ�Ƿ���ȫ���������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.11.13
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsStockIndustryLoaded
** �� ��:       ��
** �� ��:       BOOL
** ��������:    ���ع�Ʊ��ҵ�����Ƿ��Ѽ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.11.15
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsStockIndustryLoaded() {
    return GetMemTradeStats()->isStockIndustryLoaded == 1;
}


/*****************************************************************
** ������:      SetStockIndustryLoaded
** �� ��:       BOOL
** �� ��:       ��
** ��������:    ���ù�Ʊ��ҵ�����Ƿ��Ѽ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.11.15
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsStockIndustryLoaded
** �� ��:       ��
** �� ��:       BOOL
** ��������:    ������ͨծȯ�����Ƿ��Ѽ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Wei Huanyin
** �� ��:       2006.4.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsCommonBondLoaded() {
    return GetMemTradeStats()->isCommonBondLoaded == 1;
}


/*****************************************************************
** ������:      SetStockIndustryLoaded
** �� ��:       BOOL
** �� ��:       ��
** ��������:    ������ͨծȯ�����Ƿ��Ѽ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Wei Huanyin
** �� ��:       2006.4.11
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsControlSegmentLoaded
** �� ��:       ��
** �� ��:       BOOL
** ��������:    ���ز�ƷȺ������Ϣ�����Ƿ��Ѽ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.10.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsControlSegmentLoaded() {
    return GetMemTradeStats()->isControlSegmentLoaded == 1;
}


/*****************************************************************
** ������:      SetControlSegmentLoaded
** �� ��:       BOOL
** �� ��:       ��
** ��������:    ���ò�ƷȺ������Ϣ�����Ƿ��Ѽ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.10.13
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsStockIndustryLoaded
** �� ��:       ��
** �� ��:       BOOL
** ��������:    ���ؿ�ת��ծȯ�����Ƿ��Ѽ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Wei Huanyin
** �� ��:       2006.4.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsConvertibleBondLoaded() {
    return GetMemTradeStats()->isConvertibleBondLoaded == 1;
}


/*****************************************************************
** ������:      SetStockIndustryLoaded
** �� ��:       BOOL
** �� ��:       ��
** ��������:    ���ÿ�ת��ծȯ�Ƿ��Ѽ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Wei Huanyin
** �� ��:       2006.4.11
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsTradePhase
** �� ��:       STOCK_CS *stock
** �� ��:       BOOL
** ��������:    �������⼯�Ͼ��۵Ľ���ʱ���ж��Ƿ���Է�������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.9.27
** �� ��:		Li Bo
** �� ��:		2009.11.26
** �汾:        0.1
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
** ������:      IsTradePhaseTr
** �� ��:       STOCK_CS *stock
** �� ��:       BOOL
** ��������:    �����Ƿ��ǲ�Ʒ����ʱ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.9.27
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsvaidBidNumber
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ǰ����ʱ�ε���Ч��λ������
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       wugb
** �� ��:       2006.10.19
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsvaidOfferNumber
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ǰ����ʱ�ε���Ч��λ������
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       wugb
** �� ��:       2006.10.19
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      CheckTradePhase
** �� ��:       stock
** �� ��:       TRUE,FALSE
** ��������:    ���֤ȯ�Ľ���ʱ���Ƿ���Ҫ���Է��ͣ��Ƿ���Ҫ��⵹�ң�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2007.1.18
** �� ��:		
** �� ��:		
** �汾:        0.1
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
	
	//������ʱ���� ���� ʱ�����ǰһ������ʱ������ǰ������鵹�ң����ܷ���
	if(strncmp(stock->prcsStsValCod, STOCK_TRADING_RESTRICTION_BREAK, strlen(STOCK_TRADING_RESTRICTION_BREAK))==0 &&
		strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_PRETR, strlen(STOCK_TRADING_RESTRICTION_PRETR)) == 0){
		return FALSE;
	}
	//������ʱ����ͣ��ʱ�����ǰһ������ʱ���� ���м��Ͼ��� ���� ���м��Ͼ��� ʱ����鵹�ң����ܷ���
	if(strncmp(stock->prcsStsValCod, STOCK_TRADING_RESTRICTION_HALT, strlen(STOCK_TRADING_RESTRICTION_HALT))==0 &&
		(strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_OCALL, strlen(STOCK_TRADING_RESTRICTION_OCALL)) == 0 ||
		strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_ICALL, strlen(STOCK_TRADING_RESTRICTION_ICALL)) == 0)){
		return FALSE;
	}
	//������ʱ����ͣ��ʱ�����ǰһ������ʱ���� ���м��Ͼ��� ʱ����鵹�ң����ܷ���
	if(strncmp(stock->prcsStsValCod, STOCK_TRADING_RESTRICTION_SUSP, strlen(STOCK_TRADING_RESTRICTION_SUSP))==0 &&
		strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_OCALL, strlen(STOCK_TRADING_RESTRICTION_OCALL)) == 0){
		return FALSE;
	}
	//������ʱ���� ��ǰ�����Ͼ����ڼ䡢������ƽ��ǰ�ڡ�������ƽ���ڼ� ʱ����鵹�ң����ܷ��ͣ����������Ҫ��鵹��
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
** ������:      IsPriceAcross
** �� ��:       stock
** �� ��:       TRUE(���ڵ���),FALSE(�����ڵ���)
** ��������:    ���֤ȯ���Ƿ���ڼ۸񽻲棨�Ƿ񵹹ң�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2007.1.18
** �� ��:		
** �� ��:		
** �汾:        0.1
****************************************************************/
BOOL	
IsPriceAcross(STOCK_CS* stock){
	BID_CS* bid = NULL;
	OFFER_CS* offer = NULL;
	ORDER_CS* bOrder = NULL;
	ORDER_CS* sOrder = NULL;
	bid = GetBid(stock->highestBid);
	offer = GetOffer(stock->lowestOffer);
	
	//�ж�֤ȯ�Ľ���ʱ���Ƿ���Ҫ��⵹��
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
		//���ڵ���
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
		ERROR("�۸񵹹�","���ڵ���,ʱ��[%s],isincod=[%s],�򶩵�orderNo=[%lld]--trdResTypCod=[%s]---price=[%lld]--,������orderNo=[%lld]--trdResTypCod=[%s]---price=[%lld]--"\
			,stock->prcsStsValCod,stock->isinCod,bOrder->orderNo,bOrder->trdResTypCod,bid->price,sOrder->orderNo,sOrder->trdResTypCod,offer->price);
		return TRUE;
	}
	return FALSE;
}


/*****************************************************************
** ������:      StockLock
** �� ��:       stockָ��
** �� ��:       ��
** ��������:    ��ȡ�����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2009.7.24
** �� ��:		
** �� ��:		
** �汾:        0.1
****************************************************************/
void StockLock(STOCK_CS* stock, int process){
	int i = 0;
	/* struct timespec slptm;
	slptm.tv_sec = 0;
    slptm.tv_nsec = 50000;*/
	/*
	�������ԭ��
	    �������ֻ������2�����̵������
	    ���������̱��Ϊ0��1��ͬʱ��Ӧ���������ΪlockFlag[0]��lockFlag[1]���ڹ���������һ��˳����turn��ʾ��ȡ��
	    �Ľ��̡���������Ҫ��ȡ�����ʱ������������������ʶ����ΪTRUE��Ȼ��ѹ�����˳���Ǹĳ�������̱�ʶ����0��1��
	    Ȼ�����ѭ���жϣ����������Ľ��̻������������̣��������ȡ�����Ľ��̻�˯�ߵȴ�
	���磺
	    ��ĳ������Ҫ��ȡ�������ʱ�򣬴�ʱ����һ�����̲���Ҫ����������������ΪFALSE����ô������̿���ͨ��whileѭ
	    ����������Ĵ�������
	    ��0��1���̼���ͬʱҪ��ȡ�������ʱ�򣬼���0�����Ƚ������������ôturn��ֵ����1����0���̽���whileѭ��ʱ����
	    ��ѭ��������洦�����̣���1���̽���whileѭ��ʱ��ֻ����whileѭ����ȴ���ֱ��0���̰��������������ΪFALSE
	*/
	
	int other = 1 - process;
	stock->lockFLag[process] = TRUE;
	stock->turn = process;
	while(stock->turn == process && stock->lockFLag[other] == TRUE){
		i++;
		//nanosleep˯��ʱ��ȡ����ϵͳʱ���ж�
		//nanosleep(&slptm, NULL);
		
		//usleep˯��ʱ���Ǻ��뼶��
		usleep(50);
		if(i>200){
			stock->lockFLag[other] = FALSE;
			WARN(_moduleName, "��[%d]�����̵ȴ�ʱ�䳬��200��ѭ����ǿ�н���", process);
			break;
		}
	}

	return;
}



/*****************************************************************
** ������:      StockUnlock
** �� ��:       stockָ��
** �� ��:       ��
** ��������:    �ͷ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2009.7.24
** �� ��:		
** �� ��:		
** �汾:        0.1
****************************************************************/
void StockUnlock(STOCK_CS* stock, int process){
	stock->lockFLag[process] = FALSE;
	return;
}


/*****************************************************************
** ������:      PrintStockInfoToFile
** �� ��:       �ļ�������
** �� ��:       ��
** ��������:    ��ӡ��Ʊ��Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.14
** �� ��:		Li Bo
** �� ��:		2006.11.1
** �汾:        0.1
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

    sprintf(buf, "#****֤ȯ��Ϣ****\n#֤ȯ����: %d\n\n", number);
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
** ������:      PrintStockToFile
** �� ��:       �ļ�������
** �� ��:       ��
** ��������:    ��ӡ������Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.11.1
** �� ��:	  	
** �� ��:		
** �汾:        0.1
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
        printf("ERROR: �����ļ�[%s]ʧ��! \n", filePath);
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
** ������:      PrintInsideMarketToFile
** �� ��:       ��
** �� ��:       TRUE FALSE
** ��������:    ��ӡ�г�������Ϣ��ָ�����ļ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.10.9
** �� ��:
** �� ��:
** �汾:        0.1
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
        printf("ERROR: �����ļ�[%s]ʧ��! \n", filePath);
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
** ������:      PrintShow2007SnapshotToFile
** �� ��:       ��
** �� ��:       TRUE FALSE
** ��������:    ��ӡshow2007������Ϣ��ָ�����ļ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.10.9
** �� ��:
** �� ��:
** �汾:        0.1
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
        printf("ERROR: �����ļ�[%s]ʧ��! \n", filePath);
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
** ������:      PrintStock
** �� ��:       stock
** �� ��:       ��
** ��������:    ��ӡ֤ȯ��Ϣ����־��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.7
** �� ��:
** �� ��:
** �汾:        0.1
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
