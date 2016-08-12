/******************************************************************
** �ļ���:      order.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.9.11
** �� ��:       Order��Ϣ����
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * ģ������
 */
static const char   _moduleName[] = "order_cs";


/*
 * �ڲ���������
 */

/*****************************************************************
** ������:      GetOrderList
** �� ��:       ��
** �� ��:       ORDER_CS[]
** ��������:    ����Order�������ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
ORDER_CS*
GetOrderList() {
    return (ORDER_CS*) GetShareMemory() + GetShareMemAllocationStats()->orderOffset;
}


/*****************************************************************
** ������:      GetOrder
** �� ��:       ��
** �� ��:       ORDER_CS*
** ��������:    ����ָ��λ�õ�ORDER_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1.1
****************************************************************/
ORDER_CS*
GetOrder(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->orderMax) {
        return NULL;
    }
    return GetOrderList() + index;
}


/*****************************************************************
** ������:      GetOrderNumber
** �� ��:       ��
** �� ��:       Order����
** ��������:    ����Order����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetOrderNumber() {
    return GetShareMemCurrentStats()->nextOrder;
}


/*****************************************************************
** ������:      PushOrder
** �� ��:       ��
** �� ��:       ������Order��λ��
** ��������:    ��Order����������Order��Ŀ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
PushOrder(ORDER_CS *order) {
    ORDER_CS    *target = NULL;
    STOCK_CS	*stock  = NULL;
    int         index   = GetShareMemCurrentStats()->nextOrder;

    //����ڴ��Ƿ�Խ��
    if (index >= GetShareMemConfigStats()->orderMax) {  
      ///printf("error index=%lld ordermax=%lld\n", index, GetShareMemConfigStats()->orderMax);
        ERROR("Order.PushOrder()", "��Ӷ���orderNo=[%lld]ʱʧ�ܣ�������������Ԥ�������󶩵������� Current Number: %d"
                , order->orderNo, index);
        return -1;
    }
	//printf("index=%lld ordermax=%lld\n", index, GetShareMemConfigStats()->orderMax);
	//���orderProcessNo����order����β�����ش���
    if(order->orderProcessNo < 0){
    	ERROR("Order.PushOrder()", "��Ӷ���orderNo=[%lld]ʱʧ�ܣ���������Ų���С��0", order->orderNo);
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

    //��ʼ��Order
    target->balance = order->orderQty;
    target->status = ORDER_STATUS_OPEN;
    
    stock = GetStockByIsinCod(order->isinCod);

    if(stock == NULL){
      target->stockNo = NULL_INDEX;
      GetShareMemCurrentStats()->nextOrder++;
      ERROR(_moduleName, "δ�ҵ�����orderNo=[%lld]��Ӧ��֤ȯisinCod=[%12s]", order->orderNo,order->isinCod);
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
** ������:      AttachOrder
** �� ��:       ��Ҫ��ӵ�order
** �� ��:       TRUE FALSE
** ��������:    ��order��Ŀ��ӵ���������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL 
AttachOrder(ORDER_CS* pOrderCS){
	STOCK_CS*	pStockCS	= NULL;
	
	if(pOrderCS == NULL){
		ERROR(_moduleName, "AttachOrder����������󣬲���ΪNULL");
		return FALSE;
	}
	
	//�������ʣ������Ϊ0������Ҫ���뵽�۸����
	if(pOrderCS->balance == 0){
		return TRUE;
	}
	
	//��ӵ��۸����
	if(IsBuyOrder(pOrderCS)){
		UpdateBidByOrder(pOrderCS);
	}
	else if(IsSellOrder(pOrderCS)){
		UpdateOfferByOrder(pOrderCS);
	}

	//����ͳ����Ϣ
	if(IsTradingPhase(pOrderCS)){
		pStockCS = GetStock(pOrderCS->stockNo);
		if(pStockCS == NULL){
			ERROR(_moduleName, "AttachOrder ��ȡ֤ȯ��Ϣʧ��");
			return FALSE;
		}
		UpdateStockAdd(pStockCS, pOrderCS);
	}
	
	return TRUE;
}


/*****************************************************************
** ������:      DetachOrder
** �� ��:       ��Ҫ�����order
** �� ��:       TRUE FALSE
** ��������:    �Ӷ�������ɾ��order��Ŀ����order�����������룩
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL 
DetachOrder(ORDER_CS* pOrderCS){
	STOCK_CS*	pStockCS	= NULL;
	
	if(pOrderCS == NULL){
		ERROR(_moduleName, "DetachOrder ����������󣬲���ΪNULL");
		return FALSE;
	}
	
	//����۸����
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
	
	//����ͳ����Ϣ
	if(IsTradingPhase(pOrderCS)){
		pStockCS = GetStock(pOrderCS->stockNo);
		if(pStockCS == NULL){
			ERROR(_moduleName, "DetachOrder ��ȡ��Ʊ��Ϣʧ��");
			return FALSE;
		}
		UpdateStockDel(pStockCS, pOrderCS);
	}
	
	return TRUE;
}


/*****************************************************************
** ������:      StockStatAdd
** �� ��:       ORDER_CS*
** �� ��:       ��
** ��������:    �Ѷ�����Ϣ��ӵ�����ʱ��ͳ����Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.27
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
StockStatAdd(ORDER_CS* pOrderCS){
	STOCK_CS* stockCS = NULL;
	int		  bsType = 0; 
	int		  trdType = 0;
	int		  i = 0;
	
	if(pOrderCS == NULL){
		ERROR(_moduleName, "StockStatAdd �����������ΪNULL");
		return ;
	}
	
	stockCS = GetStock(pOrderCS->stockNo);
	if(stockCS == NULL){
		ERROR(_moduleName, "StockStatAdd ��ȡ��Ʊ��Ϣʧ��");
		return;
	}
	
	//ȷ��������ǣ���Ϊ0 ��Ϊ1
	if(IsBuyOrder(pOrderCS)){
		bsType = 0;
	}
	else if(IsSellOrder(pOrderCS)){
		bsType = 1;
	}
	
	//ȷ������ʱ�α��
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
		//�������Ͳ���Ҫͳ��
		return;
	}
	
	//��Ҫ�ı��ͳ����Ϣ�±�i�Ļ��
	i = bsType + trdType;
	
	//�Ѷ�����Ϣ��ӵ�����ʱ��ͳ����Ϣ
	stockCS->tradePhaseStat[i].number++;
	stockCS->tradePhaseStat[i].amount += pOrderCS->balance;
	stockCS->tradePhaseStat[i].money += (stockCS->lotSize * pOrderCS->balance / TEN_THOUSAND * pOrderCS->orderPrice);
	
}


/*****************************************************************
** ������:      StockStatDel
** �� ��:       ORDER_CS* amountΪ��Ҫɾ��������
** �� ��:       ��
** ��������:    �ӽ���ʱ��ͳ��ɾ��Order��Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.27
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
StockStatDel(ORDER_CS* pOrderCS, long long amount){
	STOCK_CS* stockCS = NULL;
	int		  bsType = 0; 
	int		  trdType = 0;
	int		  i = 0;
	
	if(pOrderCS == NULL){
		ERROR(_moduleName, "StockStatDel �����������ΪNULL");
		return ;
	}
	
	stockCS = GetStock(pOrderCS->stockNo);
	if(stockCS == NULL){
		ERROR(_moduleName, "StockStatDel ��ȡ��Ʊ��Ϣʧ��");
		return;
	}
	
	//ȷ��������ǣ���Ϊ0 ��Ϊ1
	if(IsBuyOrder(pOrderCS)){
		bsType = 0;
	}
	else if(IsSellOrder(pOrderCS)){
		bsType = 1;
	}
	
	//ȷ������ʱ�α��
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
		//�������Ͳ���Ҫͳ��
		return;
	}
	
	//��Ҫ�ı��ͳ����Ϣ�±�i�Ļ��
	i = bsType + trdType;
	
	//�ӽ���ʱ��ͳ����Ϣ��ɾ��������Ϣ
	stockCS->tradePhaseStat[i].amount -= amount;
	stockCS->tradePhaseStat[i].money -= (stockCS->lotSize * amount / TEN_THOUSAND * pOrderCS->orderPrice);
	if(pOrderCS->balance - amount == 0){
		stockCS->tradePhaseStat[i].number--;
	}
}


/*****************************************************************
** ������:      UpdateStockAdd
** �� ��:       STOCK_CS* pStockCS, ORDER_CS* pOrderCS
** �� ��:       ��
** ��������:    ����֤ȯ��Ϣ�����Ӷ�Ӧ������ͳ��
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       wugb
** �� ��:       2006.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
UpdateStockAdd(STOCK_CS* pStockCS, ORDER_CS* pOrderCS){
	if(IsBuyOrder(pOrderCS)){
		pStockCS->orderBuyNumber++;
		if(IsIceOrder(pOrderCS)){
			TRACE(_moduleName,"��ʼ����֤ȯ��Ϣ(����)������������[%lld]",pStockCS->orderBuyAmount);
			pStockCS->orderBuyAmount += pOrderCS->remPeakQty;
			pStockCS->orderBuyMoney += (pStockCS->lotSize * pOrderCS->remPeakQty / TEN_THOUSAND * pOrderCS->orderPrice);
			TRACE(_moduleName,"����֤ȯ��Ϣ(����)��������ɽ������ʾ����[%lld]������������[%lld]",pOrderCS->remPeakQty,pStockCS->orderBuyAmount);
		}
		else{
			pStockCS->orderBuyAmount += pOrderCS->balance;
			pStockCS->orderBuyMoney += (pStockCS->lotSize * pOrderCS->balance / TEN_THOUSAND * pOrderCS->orderPrice);
		}
		
	}
	else if(IsSellOrder(pOrderCS)){
		pStockCS->orderSellNumber++;
		if(IsIceOrder(pOrderCS)){
			TRACE(_moduleName,"��ʼ����֤ȯ��Ϣ(����)������������[%lld]",pStockCS->orderBuyAmount);
			pStockCS->orderSellAmount += pOrderCS->remPeakQty;
			pStockCS->orderSellMoney += (pStockCS->lotSize * pOrderCS->remPeakQty / TEN_THOUSAND * pOrderCS->orderPrice);
			TRACE(_moduleName,"����֤ȯ��Ϣ(����)��������ɽ������ʾ����[%lld]������������[%lld]",pOrderCS->remPeakQty,pStockCS->orderBuyAmount);
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
** ������:      UpdateStockDel
** �� ��:       STOCK_CS* pStockCS, ORDER_CS* pOrderCS
** �� ��:       ��
** ��������:    ����֤ȯ��Ϣ��ȥ����Ӧ������ͳ��
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       wugb
** �� ��:       2006.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
UpdateStockDel(STOCK_CS* pStockCS, ORDER_CS* pOrderCS){
	if(IsBuyOrder(pOrderCS)){
		pStockCS->orderBuyNumber--;
		if(IsIceOrder(pOrderCS)){
			TRACE(_moduleName,"��ʼ����֤ȯ��Ϣ(����)������������[%lld]",pStockCS->orderBuyAmount);
			pStockCS->orderBuyAmount -= pOrderCS->remPeakQty;
			pStockCS->orderBuyMoney -= (pStockCS->lotSize * pOrderCS->remPeakQty / TEN_THOUSAND * pOrderCS->orderPrice);
			TRACE(_moduleName,"����֤ȯ��Ϣ(����)��������ɽ������ʾ����[%lld]������������[%lld]",pOrderCS->remPeakQty,pStockCS->orderBuyAmount);
		}
		else{
			pStockCS->orderBuyAmount -= pOrderCS->balance;
			pStockCS->orderBuyMoney -= (pStockCS->lotSize * pOrderCS->balance / TEN_THOUSAND * pOrderCS->orderPrice);
		}
	}
	else if(IsSellOrder(pOrderCS)){
		pStockCS->orderSellNumber--;
		if(IsIceOrder(pOrderCS)){
			TRACE(_moduleName,"��ʼ����֤ȯ��Ϣ(����)������������[%lld]",pStockCS->orderBuyAmount);
			pStockCS->orderSellAmount -= pOrderCS->remPeakQty;
			pStockCS->orderSellMoney -= (pStockCS->lotSize * pOrderCS->remPeakQty / TEN_THOUSAND * pOrderCS->orderPrice);
			TRACE(_moduleName,"����֤ȯ��Ϣ(����)��������ɽ������ʾ����[%lld]������������[%lld]",pOrderCS->remPeakQty,pStockCS->orderBuyAmount);
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
** ������:      IsTradingPhase
** �� ��:       ORDER_CS *order
** �� ��:       BOOL
** ��������:    ����Order�Ƿ�μӲ�Ʒ����ʱ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.9.9
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsTradingPhase(ORDER_CS *order) {
	STOCK_CS	*stock                  = NULL;
	
	stock = GetStock(order->stockNo);
	//ֻ�μӿ��̼��Ͼ���
	if(strcmp(order->trdResTypCod,ORDER_TRADING_RESTRICTION_OA)==0){
		//OCALL - ���м��Ͼ���
		if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL,strlen(STOCK_TRADING_RESTRICTION_OCALL))==0){
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	//���Ͼ����ڼ���Ч
	if(strcmp(order->trdResTypCod,ORDER_TRADING_RESTRICTION_AU)==0){
		//OCALL - ���м��Ͼ��� ,ICALL - ���м��Ͼ���
		if((strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_ICALL,strlen(STOCK_TRADING_RESTRICTION_ICALL))==0) || (strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL,strlen(STOCK_TRADING_RESTRICTION_OCALL))==0)){
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	//�ڶ�����ƽ��ʱ������
	if(strcmp(order->trdResTypCod,ORDER_TRADING_RESTRICTION_SU)==0){
		//IOBB - ���ж�����ƽ�⣬IPOBB - ���ж�����ƽ��ǰ�ڣ�OOBB  - ���ж�����ƽ�⣬OPOBB - ���ж�����ƽ��ǰ��	
		if((strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IOBB,strlen(STOCK_TRADING_RESTRICTION_IOBB))==0) || (strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IPOBB,strlen(STOCK_TRADING_RESTRICTION_IPOBB))==0) || (strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OOBB,strlen(STOCK_TRADING_RESTRICTION_OOBB))==0) || (strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OPOBB,strlen(STOCK_TRADING_RESTRICTION_OPOBB))==0)){
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	
	//�ڹ̶��۸�ʱ����Ч
	if(strcmp(order->trdResTypCod,ORDER_TRADING_RESTRICTION_FP)==0){
		//FCALL - �̶��۸񼯺Ͼ���
		if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_FCALL,strlen(STOCK_TRADING_RESTRICTION_FCALL))==0){
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	
	//������
	if(strcmp(order->trdResTypCod,ORDER_TRADING_RESTRICTION_SPACE)==0){ 
		return TRUE;
	}
	return TRUE;
} 



/*****************************************************************
** ������:      IsIceOrder
** �� ��:       orderCS
** �� ��:       TRUE FALSE
** ��������:    �ж϶����Ƿ�Ϊ��ɽ����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.23
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL	
IsIceOrder(ORDER_CS* orderCS){
	return orderCS->orderType == 'I';
}



/*****************************************************************
** ������:      GetWorkedOrderNumber
** �� ��:       ��
** �� ��:       �Ѵ������ί������
** ��������:    ���ص�ǰ�Ѵ������ί����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetWorkedOrderNumber() {
    return GetMemTradeStats()->workedOrderNumber;
}


/*****************************************************************
** ������:      SetWorkedOrderNumber
** �� ��:       ��ǰ�Ѵ������ί����
** �� ��:       ��
** ��������:    ���õ�ǰ�Ѵ������ί����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetWorkedOrderNumber(int workedNumber) {
    GetMemTradeStats()->workedOrderNumber = workedNumber;
}


/*****************************************************************
** ������:      IncreaseWorkedOrderNumber
** �� ��:       ��
** �� ��:       ��
** ��������:    ������ǰ�Ѵ������ί����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
IncreaseWorkedOrderNumber() {
    GetMemTradeStats()->workedOrderNumber++;
}

/*****************************************************************
** ������:      IsBuyOrder
** �� ��:       ORDER_CS*
** �� ��:       BOOL
** ��������:    ����ָ����ί���Ƿ�������ί��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.19
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsBuyOrder(ORDER_CS *order) {
    return order->orderBSType == 1;
}


/*****************************************************************
** ������:      IsSellOrder
** �� ��:       ORDER_CS*
** �� ��:       BOOL
** ��������:    ����ָ����ί���Ƿ�������ί��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.19
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsSellOrder(ORDER_CS *order) {
    return order->orderBSType ==  2;
}


/*****************************************************************
** ������:      IsOrderOutmoded
** �� ��:       ORDER_CS*
** �� ��:       BOOL
** ��������:    ����ί���Ƿ��ѹ�ʱ������ȫ���ɽ��򳷵���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2006.3.2
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsOrderOutmoded(ORDER_CS *order) {
    return order->balance <= 0;
}


/*****************************************************************
** ������:      IsOrderProcessComplete
** �� ��:       ��
** �� ��:       BOOL
** ��������:    ����ί���Ƿ���ȫ���������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2006.3.2
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsOrderProcessComplete() {
    return GetWorkedOrderNumber() >= GetOrderNumber();
}


/*****************************************************************
** ������:      PrintOrderCS
** �� ��:       orderCS
** �� ��:       ��
** ��������:    ���order�������ļ�
** ȫ�ֱ���:    ��
** ����ģ��:    
** �� ��:       Li Bo
** �� ��:       2006.10.12
** �� ��:		
** �� ��:		
** �汾:        0.1
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
** ������:      PrintOrderInfoToFile
** �� ��:       �ļ�������
** �� ��:       ��
** ��������:    ��ӡί����Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.15
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintOrderInfoToFile(int fp){
    ORDER_CS    *orderCS  = NULL;
    char        buf[1024];
    int         number  = 0;
    int         i       = 0;

    number = GetOrderNumber();

    sprintf(buf, "#****ί����Ϣ****\n#ί������: %d\n\n", number);
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
