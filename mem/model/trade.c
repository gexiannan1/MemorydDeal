/******************************************************************
** �ļ���:      trade.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.9.11
** �� ��:       Trade��Ϣ����
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
** 2006.03.23   Wei Huanyin 1��IsValidTrade��������ֵ��char��Ϊint
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
 * ģ������
 */
static const char   _moduleName[] = "trade_cs";

/*****************************************************************
** ������:      GetTradeList
** �� ��:       ��
** �� ��:       TRADE_CS[]
** ��������:    ����Trade�������ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
TRADE_CS*
GetTradeList() {
    return (TRADE_CS*) GetShareMemory() + GetShareMemAllocationStats()->tradeOffset;
}


/*****************************************************************
** ������:      GetTrade
** �� ��:       ��
** �� ��:       TRADE_CS*
** ��������:    ����ָ��λ�õ�TRADE_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
TRADE_CS*
GetTrade(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->tradeMax) {
        return NULL;
    }
    return GetTradeList() + index;
}


/*****************************************************************
** ������:      GetTradeNumber
** �� ��:       ��
** �� ��:       Trade����
** ��������:    ����Trade����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetTradeNumber() {
    return GetShareMemCurrentStats()->nextTrade;
}


/*****************************************************************
** ������:      GetLastTrade
** �� ��:       ��
** �� ��:       TRADE_CS*
** ��������:    �������һ�ʳɽ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.10.18
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
TRADE_CS*
GetLastTrade() {
    return GetTrade(GetTradeNumber() - 1);
}


/*****************************************************************
** ������:      AddTrade
** �� ��:       ��
** �� ��:       ������Trade��λ��
** ��������:    ��Trade����������Trade��Ŀ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
AddTrade(TRADE_TE *teTrade) {
	STOCK_CS*	stock = NULL;
    TRADE_CS*	target = NULL;
    ORDER_CS*	order = NULL;
    int index	= GetShareMemCurrentStats()->nextTrade;
    
    /*
     * ����ڴ��Ƿ�Խ��
     */
    if (index >= GetShareMemConfigStats()->tradeMax) {
        ERROR(_moduleName,"AddTrade�ɽ���������Ԥ��������ɽ�����");
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
** ������:      GetWorkedTradeNumber
** �� ��:       ��
** �� ��:       �Ѵ�����ĳɽ�����
** ��������:    ���ص�ǰ�Ѵ�����ĳɽ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetWorkedTradeNumber() {
    return GetMemTradeStats()->workedTradeNumber;
}


/*****************************************************************
** ������:      SetWorkedTradeNumber
** �� ��:       ��ǰ�Ѵ�����ĳɽ���
** �� ��:       ��
** ��������:    ���õ�ǰ�Ѵ�����ĳɽ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetWorkedTradeNumber(int workedNumber) {
    GetMemTradeStats()->workedTradeNumber = workedNumber;
}


/*****************************************************************
** ������:      IncreaseWorkedTradeNumber
** �� ��:       ��
** �� ��:       ��
** ��������:    ������ǰ�Ѵ�����ĳɽ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
IncreaseWorkedTradeNumber() {
    GetMemTradeStats()->workedTradeNumber++;
}


/*****************************************************************
** ������:      GetSentTradeNumber
** �� ��:       ��
** �� ��:       �Ѵ�����ĳɽ�����
** ��������:    ���ص�ǰ�ѷ��͵ĳɽ�����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.25
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetSentTradeNumber() {
    return GetMemTradeStats()->sentTradeNumber;
}


/*****************************************************************
** ������:      SetSentTradeNumber
** �� ��:       ��ǰ�Ѵ�����ĳɽ���
** �� ��:       ��
** ��������:    ���õ�ǰ�ѷ��͵ĳɽ�����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.25
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetSentTradeNumber(int sentTradeNumber) {
    GetMemTradeStats()->sentTradeNumber = sentTradeNumber;
}


/*****************************************************************
** ������:      IncreaseWorkedTradeNumber
** �� ��:       ��
** �� ��:       ��
** ��������:    ������ǰ�ѷ��͵ĳɽ�����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.25
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
IncreaseSentTradeNumber() {
    GetMemTradeStats()->sentTradeNumber++;
}


/*****************************************************************
** ������:      IsPendingVendTrade
** �� ��:       TRADE_CS*
** �� ��:       BOOL
** ��������:    ����ָ���ĳɽ��Ƿ���Ҫ�ӹ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.16
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsBuyTrade
** �� ��:       TRADE_CS*
** �� ��:       BOOL
** ��������:    �����Ƿ��������ͳɽ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL 
IsBuyTrade(TRADE_CS* tradeCS){
	return tradeCS->tradeBSType == 'B';
}


/*****************************************************************
** ������:      IsSellTrade
** �� ��:       TRADE_CS*
** �� ��:       BOOL
** ��������:    �����Ƿ��������ͳɽ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL 
IsSellTrade(TRADE_CS* tradeCS){
	return tradeCS->tradeBSType == 'S';
}


/*****************************************************************
** ������:      IsMarketPriceTrade
** �� ��:       TRADE_CS*
** �� ��:       BOOL
** ��������:    �����Ƿ����м�ί�гɽ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2010.5.4
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL 
IsMarketPriceTrade(TRADE_CS* tradeCS){
	return tradeCS->orderType == 'M';
}



/*****************************************************************
** ������:      IsValidTrade
** �� ��:       TRADE_CS: �ɽ���Ϣ
** �� ��:       ��
** ��������:    �жϸñʳɽ������Ƿ��Ƿ������������ݣ��������������'0'�����򷵻�'1'
** ȫ�ֱ���:
** ����ģ��:    ��
** �� ��:       weihy
** �� ��:       2005.2.15
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      PrintTradeInfoToFile
** �� ��:       �ļ�������
** �� ��:       ��
** ��������:    ��ӡ�ɽ���Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintTradeInfoToFile(int fp){
    TRADE_CS    *trade      = NULL;
    char        buf[1024];
    int         number      = 0;
    int         i           = 0;

    number = GetTradeNumber();

    sprintf(buf, "#****�ɽ���Ϣ****\n#�ɽ�����: %d\n\n", number);
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
