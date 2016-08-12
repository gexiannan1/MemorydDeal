/******************************************************************
** �ļ���:      stock_pbu.c
** �� ��:       0.1
** ������:      Li Bo
** �� ��:       2006.12.12
** �� ��:       Stock�е�PBU��Ϣ����
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/stock_pbu.c,v 1.1 2012/03/29 01:19:54 tc\boli Exp $
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

#include	"model.h"
#include	"../memory.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"
#include    "../util/log/log.h"
#include    "stock.h"


/*
 * ģ������
 */
static const char   _moduleName[] = "stock_pbu";

/*
 * �ڲ���������
 */
static PBU_HEAD*	GetPBUTable();										/* ����PBU�����б����ʼλ�� */
static int*		   	GetPBUBuySort();									/* ����PBU��ί����������ʼλ�� */
static int*			GetPBUSellSort();									/* ����PBU��ί����������ʼλ�� */
static PBU_DATA*	GetPBUData();										/* ����PBU���ݵ���ʼλ�� */

//static void			UpdatePBUBuyOrderSortList(STOCK_CS*, int, int, int);		/* pbu��ί���������� */
//static void			UpdatePBUSellOrderSortList(STOCK_CS*, int, int, int);		/* pbu��ί���������� */

//static void			UpdatePBUBuyTradeSortList(STOCK_CS*, int);			/* pbu��ɽ��������� */
//static void			UpdatePBUSellTradeSortList(STOCK_CS*, int);			/* pbu���ɽ��������� */

static int 			CompFirm(PBU_FIRM *p1, PBU_FIRM *p2);				/* ���ַ���Ҫ�ıȽϺ��� */



/*****************************************************************
** ������:      InitPBU
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ʼ��pbu�ڴ���������pbu�����б�pbu����������ʼ��ֵ��Ϊ0������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
InitPBU(){
	#ifdef USE_PBU
		PBU_HEAD*	pbuHead = NULL;
		int*		pbuBuySort = NULL;
		int*		pbuSellSort = NULL;
		int			i = 0;
		int			j = 0;
		
		MEM_CONFIG_SHARE *memConfig = GetShareMemConfigStats();
		
		pbuHead = GetPBUTable();
		pbuBuySort = GetPBUBuySort();
		pbuSellSort = GetPBUSellSort();
		for(i = 0; i < memConfig->stockMax; i++){
			for(j = 0; j < memConfig->pbuNumberMax; j++, pbuHead++, pbuBuySort++, pbuSellSort++){
				pbuHead->pbuDataAdr = -1;
				pbuHead->pbuBuyOrderSortNo = -1;
				pbuHead->pbuSellOrderSortNo = -1;
				
				*pbuBuySort = -1;
				*pbuSellSort = -1;
			}
		}
	#endif
}


/*****************************************************************
** ������:      GetPBUFirm
** �� ��:       ��
** �� ��:       PBU_FIRM*
** ��������:    ����PBU�ŵ���ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
PBU_FIRM*	
GetPBUFirm(){
	#ifdef USE_PBU
		return (PBU_FIRM*) GetShareMemory() + GetShareMemAllocationStats()->pbuFirmOffset;
	#endif
	
	return NULL;
}



/*****************************************************************
** ������:      GetPBUFirmByPbuNo
** �� ��:       int
** �� ��:       PBU_FIRM*
** ��������:    ����pbu��λ�û��pbu����Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
PBU_FIRM* 
GetPBUFirmByPbuNo(int pbuNo){
	#ifdef USE_PBU
		if(pbuNo < 0 || pbuNo >= GetShareMemConfigStats()->pbuNumberMax){
			return NULL;
		}
		return GetPBUFirm() + pbuNo;
	#endif
	
	return NULL;
}



/*****************************************************************
** ������:      GetPBUNoByFirm
** �� ��:       ��
** �� ��:       int
** ��������:    ����pbu�����ô�pbu�������б��е�������׵�ַ��λ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int 
GetPBUNoByFirm(char* firm){
    PBU_FIRM*		target = NULL;
    PBU_FIRM		firmComp;
    
    memcpy(firmComp.pbuFirm, firm, PBU_FIRM_LENGTH);
    
    target = (PBU_FIRM*)bsearch(&firmComp, GetPBUFirm(),
		GetPBUNumber(), sizeof(PBU_FIRM), (int(*)(const void *,const void *))CompFirm);
		
	if(target == NULL){
		return -1;
	}

    return target - GetPBUFirm();
}


/*****************************************************************
** ������:      GetPBUNumber
** �� ��:       ��
** �� ��:       int
** ��������:    ��õ�ǰ�Ѿ��洢��pbu��������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int	
GetPBUNumber(){
	#ifdef USE_PBU
		return GetShareMemCurrentStats()->nextPBUFirm;
	#endif
	
	return -1;
}


/*****************************************************************
** ������:      GetPBUDataNumber
** �� ��:       ��
** �� ��:       int
** ��������:    ��ȡ��ǰ�Ѿ��洢��PBU��������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int 
GetPBUDataNumber(){
	#ifdef USE_PBU
		return GetShareMemCurrentStats()->nextPBUData;
	#endif
	
	return -1;
}


/*****************************************************************
** ������:      GetPBUTableByStockNo
** �� ��:       stockNo firmIndex
** �� ��:       PBU_HEAD
** ��������:    ����stockNo��pbu�ں�����λ�÷��ض�Ӧ��pbu��Ϣ��ַ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
PBU_HEAD*	
GetPBUTableByStockNo(int stockNo, int firmIndex){
	#ifdef USE_PBU
		if(stockNo < 0 || firmIndex < 0 || stockNo >= GetShareMemConfigStats()->stockMax || firmIndex >= GetShareMemConfigStats()->pbuNumberMax){
			return NULL;
		}
		return GetPBUTable() + GetShareMemConfigStats()->pbuNumberMax * stockNo + firmIndex;
	#endif
	return NULL;
}


/*****************************************************************
** ������:      GetPBUBuySortByStockNo
** �� ��:       stockNo sortIndex
** �� ��:       int
** ��������:    ����stockNo���������ض�Ӧ��PBU��ί��������ַ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int*	
GetPBUBuySortByStockNo(int stockNo, int sortIndex){
	#ifdef USE_PBU
		if(stockNo < 0 || sortIndex < 0 || stockNo >= GetShareMemConfigStats()->stockMax || sortIndex >= GetShareMemConfigStats()->pbuNumberMax){
			return NULL;
		}
		return GetPBUBuySort() + GetShareMemConfigStats()->pbuNumberMax * stockNo + sortIndex;
	#endif
	return NULL;
}



/*****************************************************************
** ������:      GetPBUSellSortByStockNo
** �� ��:       stockNo sortIndex
** �� ��:       int
** ��������:    ����stockNo���������ض�Ӧ��PBU��ί��������ַ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int* 
GetPBUSellSortByStockNo(int stockNo, int sortIndex){ 
	#ifdef USE_PBU
		if(stockNo < 0 || sortIndex < 0 || stockNo >= GetShareMemConfigStats()->stockMax || sortIndex >= GetShareMemConfigStats()->pbuNumberMax){
			return NULL;
		}
		return GetPBUSellSort() + GetShareMemConfigStats()->pbuNumberMax * stockNo + sortIndex;
	#endif
	return NULL;
}


/*****************************************************************
** ������:      GetPBUDataByIndex
** �� ��:       index
** �� ��:       PBU_DATA
** ��������:    ���ݵ�ַ�Ż�ȡ��Ӧ��PBU���ݵ�ַ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
PBU_DATA* 
GetPBUDataByIndex(int index){
	#ifdef USE_PBU
		if(index < 0 || index >= GetShareMemConfigStats()->pbuDataNumberMax){
			return NULL;
		}
		return GetPBUData() + index;
	#endif
	return NULL;
}


/*****************************************************************
** ������:      AddFirm
** �� ��:       pbuFirm
** �� ��:       �����λ��
** ��������:    ���pbuϯλ�ŵ������ڴ�(ͬʱ����)
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int	
AddFirm(char* pbuFirm){
	#ifdef USE_PBU
		PBU_FIRM*	firm = NULL;
		int         index   = GetShareMemCurrentStats()->nextPBUFirm;
		int			insertIndex = 0;
		int			i		= 0;
		
		//����ڴ��Ƿ�Խ��
	    if (index >= GetShareMemConfigStats()->pbuNumberMax) {
	        FATAL("stock_pbu.AddFirm()", "pbu��������Ԥ��������pbu������ Current Number: %d"
	                , index);
	        return -1;
	    }
		
		firm = GetPBUFirm();
		
		for(i = 0; i < index; i++){
			if(strcmp(firm->pbuFirm, pbuFirm) > 0){
				break;
			}
			firm++;
		}
		
		memmove(firm + 1, firm, sizeof(PBU_FIRM) * (index - (firm - GetPBUFirm())));
		
		memcpy(firm->pbuFirm, pbuFirm, PBU_FIRM_LENGTH);
		
		GetShareMemCurrentStats()->nextPBUFirm++;
		
		return firm - GetPBUFirm();
	#endif
	return -1;
}


/*****************************************************************
** ������:      PBUOrderStatAdd
** �� ��:       stockCS, pbuData, order
** �� ��:       ��
** ��������:    pbu����ʱ��ί��ͳ�ƣ����ӣ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.16
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
PBUOrderStatAdd(STOCK_CS* stockCS, PBU_DATA* pbuData, ORDER_CS* order){
	if(pbuData == NULL || order == NULL){
		ERROR(_moduleName, "PBUOrderStatAdd�����������ΪNULL");
		return;
	}
	
	if(IsBuyOrder(order)){
		if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
			pbuData->orderOABuyAmount += order->balance;
			pbuData->orderOABuyMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
			pbuData->orderAUBuyAmount += order->balance;
			pbuData->orderAUBuyMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
			pbuData->orderSUBuyAmount += order->balance;
			pbuData->orderSUBuyMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
			pbuData->orderFPBuyAmount += order->balance;
			pbuData->orderFPBuyMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else{
			//�������Ͳ���Ҫͳ��
		}
	}
	else if(IsSellOrder(order)){
		if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
			pbuData->orderOASellAmount += order->balance;
			pbuData->orderOASellMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
			pbuData->orderAUSellAmount += order->balance;
			pbuData->orderAUSellMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
			pbuData->orderSUSellAmount += order->balance;
			pbuData->orderSUSellMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
			pbuData->orderFPSellAmount += order->balance;
			pbuData->orderFPSellMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
		}
		else{
			//�������Ͳ���Ҫͳ��
		}
	}
	
	//ά��pbu��ǰ����ʱ��ͳ����Ϣ
	if(IsTradingPhase(order)){
		if(IsBuyOrder(order)){
			if(IsIceOrder(order)){
				//��ɽ����ͳ�ƽ�ʾ����
				TRACE(_moduleName,"��ʼpbu��ǰ����ʱ��ͳ�ƣ���ɽ��������pbu[%s]����������[%lld]",order->pbuId, pbuData->orderBuyAmount);
				pbuData->orderBuyAmount += order->remPeakQty;
				pbuData->orderBuyMoney += (stockCS->lotSize * order->remPeakQty / TEN_THOUSAND * order->orderPrice);
				TRACE(_moduleName,"pbu��ǰ����ʱ��ͳ�ƽ�������ɽ������ʾ����[%lld]��pbu[%s]����������[%lld]",order->remPeakQty, order->pbuId, pbuData->orderBuyAmount);
			}
			else{
				pbuData->orderBuyAmount += order->balance;
				pbuData->orderBuyMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
			}
		}
		else if(IsSellOrder(order)){
			if(IsIceOrder(order)){
				//��ɽ����ͳ�ƽ�ʾ����
				TRACE(_moduleName,"��ʼpbu��ǰ����ʱ��ͳ�ƣ���ɽ��������pbu[%s]����������[%lld]",order->pbuId, pbuData->orderSellAmount);
				pbuData->orderSellAmount += order->remPeakQty;	
				pbuData->orderSellMoney += (stockCS->lotSize * order->remPeakQty / TEN_THOUSAND * order->orderPrice);
				TRACE(_moduleName,"pbu��ǰ����ʱ��ͳ�ƽ�������ɽ������ʾ����[%lld]��pbu[%s]����������[%lld]",order->remPeakQty, order->pbuId, pbuData->orderSellAmount);
			}
			else{
				pbuData->orderSellAmount += order->balance;	
				pbuData->orderSellMoney += (stockCS->lotSize * order->balance / TEN_THOUSAND * order->orderPrice);
			}
		}
	}
}


/*****************************************************************
** ������:      PBUOrderStatDel
** �� ��:       pbuData, order, amount
** �� ��:       ��
** ��������:    pbu����ʱ��ί��ͳ�ƣ����٣�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.16
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
PBUOrderStatDel(STOCK_CS* stockCS, PBU_DATA* pbuData, ORDER_CS* order, long long amount){
	if(pbuData == NULL || order == NULL){
		ERROR(_moduleName, "PBUOrderStatDel�����������ΪNULL");
		return;
	}
	
	if(amount < 0){
		ERROR(_moduleName, "PBUOrderStatDel�������amount����С��0");
		return;
	}

	if(IsBuyOrder(order)){
		if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
			pbuData->orderOABuyAmount -= amount;
			pbuData->orderOABuyMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
			pbuData->orderAUBuyAmount -= order->balance;
			pbuData->orderAUBuyMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
			pbuData->orderSUBuyAmount -= order->balance;
			pbuData->orderSUBuyMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
			pbuData->orderFPBuyAmount -= order->balance;
			pbuData->orderFPBuyMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else{
			//�������Ͳ���Ҫͳ��
		}
	}
	else if(IsSellOrder(order)){
		if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_OA) == 0){
			pbuData->orderOASellAmount -= amount;
			pbuData->orderOASellMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_AU) == 0){
			pbuData->orderAUSellAmount -= order->balance;
			pbuData->orderAUSellMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_SU) == 0){
			pbuData->orderSUSellAmount -= order->balance;
			pbuData->orderSUSellMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(strcmp(order->trdResTypCod, ORDER_TRADING_RESTRICTION_FP) == 0){
			pbuData->orderFPSellAmount -= order->balance;
			pbuData->orderFPSellMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else{
			//�������Ͳ���Ҫͳ��
		}
	}
	
	//ά��pbu��ǰ����ʱ��ͳ����Ϣ
	if(IsTradingPhase(order)){
		if(IsBuyOrder(order)){
			pbuData->orderBuyAmount -= amount;
			pbuData->orderBuyMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
		else if(IsSellOrder(order)){
			pbuData->orderSellAmount -= amount;	
			pbuData->orderSellMoney -= (stockCS->lotSize * amount / TEN_THOUSAND * order->orderPrice);
		}
	}
}


/*****************************************************************
** ������:      UpdatePBUTradeStat
** �� ��:       pbuData, trade
** �� ��:       ��
** ��������:    ά��pbu�ɽ�ͳ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.16
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
UpdatePBUTradeStat(PBU_DATA* pbuData, TRADE_CS* trade){
	if(pbuData == NULL || trade == NULL){
		ERROR(_moduleName, "UpdatePBUTradeStat�����������ΪNULL");
		return;
	}
	
	if(IsBuyTrade(trade)){
		pbuData->tradeBuyAmount += trade->qty;
		pbuData->tradeBuyMoney += trade->mktVal;
	}
	else if(IsSellTrade(trade)){
		pbuData->tradeSellAmount += trade->qty;
		pbuData->tradeSellMoney += trade->mktVal;
	}
}


/*****************************************************************
** ������:      AddNewPBUData
** �� ��:       pbuHead
** �� ��:       PBU_DATA*
** ��������:    ����µ�pbu����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.16
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
PBU_DATA*	
AddNewPBUData(PBU_HEAD* pbuHead){
	#ifdef USE_PBU
		int index = GetShareMemCurrentStats()->nextPBUData;
		
		if(index >= GetShareMemConfigStats()->pbuDataNumberMax){
			ERROR(_moduleName, "���pub����ʧ�ܣ�pbu������������Ԥ�������������� Current Number: %d", index);
			return NULL;
		}
		
		pbuHead->pbuDataAdr = index;
		GetShareMemCurrentStats()->nextPBUData++;
		return GetPBUDataByIndex(index);
	#endif
	return NULL;
}


/*****************************************************************
** ������:      UpdatePBUOrderSortList
** �� ��:       stock, pbuNo, order
** �� ��:       ��
** ��������:    ά��pbuί������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.17
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void	
UpdatePBUOrderSortList(STOCK_CS* stock, int pbuNo, ORDER_CS* order){
	PBU_HEAD*	pbuHead = NULL;
	PBU_HEAD*	pbuHeadCmp = NULL;
	PBU_DATA*	pbuData = NULL;
	PBU_DATA*	pbuDataCmp = NULL;
	int*		pbuSortList = NULL;
	int			sortNo = -1;
	int			begin = -1;
	int			end = -1;
	
	if(stock == NULL){
		ERROR(_moduleName, "UpdatePBUOrderSortList�������������ΪNULL");
		return;
	}
	
	pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
	if(pbuHead == NULL){
		ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ӧ��pbu������Ϣ");
		return;
	}
	
	if(IsBuyOrder(order)){
		if(pbuHead->pbuBuyOrderSortNo == -1){
			begin = 0;
			end = stock->lastBuyOrderSortOffset;
			//��Ҫ�²������ݣ�����Ҫ����stock��Ϣ�е�pbu���������λ��
			stock->lastBuyOrderSortOffset++;
		}
		else {
			pbuSortList = GetPBUBuySortByStockNo(stock->stockNo, pbuHead->pbuBuyOrderSortNo);
			pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
			if(pbuData == NULL){
				ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ӧ��pbu������Ϣ");
				return;
			}
			
			//��ǰ�����ǵ�һ��
			if(pbuHead->pbuBuyOrderSortNo == 0){
				//��ڶ����Ƚ�
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList + 1));
				if(pbuHeadCmp == NULL){
					//��ǰ�����б���ֻ��һ������������
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ӧ��pbu������Ϣ");
					return;
				}
				if(pbuData->orderBuyAmount < pbuDataCmp->orderBuyAmount){
					begin = 1;
					end = stock->lastBuyOrderSortOffset;
				}
				else{
					//�Ѿ��ǵ�һ�ˣ�����Ҫ����
					return;	
				}
			}
			//��ǰ���������һ��
			else if(pbuHead->pbuBuyOrderSortNo == stock->lastBuyOrderSortOffset){
				//�뵹���ڶ����Ƚ�
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList - 1));
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu��Ϣ");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu������Ϣ");
					return;
				}
				
				if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
					begin = 0;
					end = stock->lastBuyOrderSortOffset - 1;
				}
				else{
					//��Ȼ�ǵ�����һ������Ҫ����
					return;	
				}
			}
			//��ǰ�������ǵ�һҲ�������һ��
			else{
				//��ǰ��һ���Ƚ�
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList - 1));
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu��Ϣ");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu������Ϣ");
					return;
				}
				if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
					begin = 0;
					end = pbuHead->pbuBuyOrderSortNo - 1;
				}
				else if(pbuData->orderBuyAmount == pbuDataCmp->orderBuyAmount){
					//��ǰ��һ����ȣ���ô����Ҫ��������
					return;
				}
				else{
					//�����һ���Ƚ�
					pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList + 1));
					if(pbuHeadCmp == NULL){
						ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu��Ϣ");
						return;
					}
					pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
					if(pbuDataCmp == NULL){
						ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu������Ϣ");
						return;
					}
					if(pbuData->orderBuyAmount < pbuDataCmp->orderBuyAmount){
						begin = pbuHead->pbuBuyOrderSortNo + 1;
						end = stock->lastBuyOrderSortOffset;
					}
					else{
						//��ǰ��һ��С�����ұȺ���һ����(�������)�����Բ���������
						return;
					}
				}
			}
		}
		//��������
		//UpdatePBUBuyOrderSortList(stock, pbuNo, begin, end);
	}
	else if(IsSellOrder(order)){
		if(pbuHead->pbuSellOrderSortNo == -1){
			begin = 0;
			end = stock->lastSellOrderSortOffset;
			//��Ҫ�²������ݣ�����Ҫ����stock��Ϣ�е�pbu���������λ��
			stock->lastSellOrderSortOffset++;
		}
		else {
			pbuSortList = GetPBUSellSortByStockNo(stock->stockNo, pbuHead->pbuSellOrderSortNo);
			pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
			if(pbuData == NULL){
				ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ӧ��pbu������Ϣ");
				return;
			}	
			
			//��ǰ�����ǵ�һ��
			if(pbuHead->pbuSellOrderSortNo == 0){
				//��ڶ����Ƚ�
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList + 1));
				if(pbuHeadCmp == NULL){
					//��ǰ�����б���ֻ��һ������������
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ӧ��pbu������Ϣ");
					return;
				}
				if(pbuData->orderSellAmount < pbuDataCmp->orderSellAmount){
					begin = 1;
					end = stock->lastSellOrderSortOffset;
				}
				else{
					//�Ѿ��ǵ�һ�ˣ�����Ҫ����
					return;	
				}
			}
			//��ǰ���������һ��
			else if(pbuHead->pbuSellOrderSortNo == stock->lastSellOrderSortOffset){
				//�뵹���ڶ����Ƚ�
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList - 1));
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu��Ϣ");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu������Ϣ");
					return;
				}
				
				if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
					begin = 0;
					end = stock->lastSellOrderSortOffset - 1;
				}
				else{
					//��Ȼ�ǵ�����һ������Ҫ����
					return;	
				}
			}
			//��ǰ�������ǵ�һҲ�������һ��
			else{
				//��ǰ��һ���Ƚ�
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList - 1));
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu��Ϣ");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu������Ϣ");
					return;
				}
				if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
					begin = 0;
					end = pbuHead->pbuSellOrderSortNo - 1;
				}
				else if(pbuData->orderSellAmount == pbuDataCmp->orderSellAmount){
					//��ǰ��һ����ȣ���ô����Ҫ��������
					return;
				}
				else{
					//�����һ���Ƚ�
					pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *(pbuSortList + 1));
					if(pbuHeadCmp == NULL){
						ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu��Ϣ");
						return;
					}
					pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
					if(pbuDataCmp == NULL){
						ERROR(_moduleName, "pbuί������ʧ�ܣ�δ�ҵ���Ҫ�Ƚϵ�pbu������Ϣ");
						return;
					}
					if(pbuData->orderSellAmount < pbuDataCmp->orderSellAmount){
						begin = pbuHead->pbuSellOrderSortNo + 1;
						end = stock->lastSellOrderSortOffset;
					}
					else{
						//��ǰ��һ��С�����ұȺ���һ����(�������)�����Բ���������
						return;
					}
				}
			}
		}
		//��������
		//UpdatePBUSellOrderSortList(stock, pbuNo, begin, end);
	}
	else{
		ERROR(_moduleName, "pbuί������ʧ�ܣ��������ʹ���");
		return;
	}
}


/*****************************************************************
** ������:      UpdatePBUTradeSortList
** �� ��:       stock, pbuNo, trade
** �� ��:       ��
** ��������:    ά��pbu�ɽ�����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.18
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
UpdatePBUTradeSortList(STOCK_CS* stock, int pbuNo, TRADE_CS* trade){
	if(IsBuyTrade(trade)){
	//	UpdatePBUBuyTradeSortList(stock, pbuNo);
	}
	else if(IsSellTrade(trade)){
		//UpdatePBUSellTradeSortList(stock, pbuNo);
	}
}


/*****************************************************************
** ������:      UpdatePBUBuyTradeSortList
** �� ��:       stock, pbuNo
** �� ��:       ��
** ��������:    ά��pbu��ɽ�����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.19
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
/*static void 
UpdatePBUBuyTradeSortList(STOCK_CS* stock, int pbuNo){
	PBU_HEAD*	pbuHead = NULL;
	PBU_HEAD*	pbuHeadCmp = NULL;
	PBU_DATA*	pbuData = NULL;
	PBU_DATA*	pbuDataCmp = NULL;
	int			begin = -1;
	int			end = -1;
	int			i = 0;
	
	if(stock == NULL || pbuNo < 0){
		ERROR(_moduleName, "UpdatePBUBuyTradeSortList�����������");
		return;
	}
	
	pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
	if(pbuHead == NULL){
		ERROR(_moduleName, "UpdatePBUBuyTradeSortList����δ�ҵ���Ҫ�����pbu������Ϣ");
		return;
	}
	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
	if(pbuData == NULL){
		ERROR(_moduleName, "UpdatePBUBuyTradeSortList����pbu������Ϣ����");
		return;
	}
	
	
	for(i = PBU_TRADE_SORT_LENGTH - 1; i >= 0; i--){
		if(stock->pbuTradeBuySort[i] == -1){
			continue;
		}
		if(stock->pbuTradeBuySort[i] == pbuNo){
			end = i;
			continue;
		}
		
		pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, stock->pbuTradeBuySort[i]);
		if(pbuHeadCmp == NULL){
			ERROR(_moduleName, "UpdatePBUBuyTradeSortList����δ�ҵ���Ҫ�Ƚϵ�pbu������Ϣ");
			return;
		}
		pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
		if(pbuDataCmp == NULL){
			ERROR(_moduleName, "UpdatePBUBuyTradeSortList����δ�ҵ���Ҫ�Ƚϵ�pbu����");
			return;
		}
		
		if(pbuData->tradeBuyAmount <= pbuDataCmp->tradeBuyAmount){
			begin = i + 1;
			break;
		}
	}
	
	if(begin == PBU_TRADE_SORT_LENGTH){
		//Ҫ����ĳɽ�����С�����һ���ĳɽ���������˲���Ҫ��������
		return;
	}
	if(begin == -1){
		//���������ǿյģ�����Ҫ����ĳɽ�����Ӧ�����ڵ�һ��
		begin = 0;
	}
	if(end == -1){
		//Ҫ�����pbu�ɽ�û����ԭ��������������
		end = PBU_TRADE_SORT_LENGTH - 1;
	}
	
	if(begin == end){
		//����û�б仯��Ҫ����������pbu��Ȼ����ԭλ�����������������һλ
		stock->pbuTradeBuySort[begin] = pbuNo;
		return;
	}
	
	memmove(stock->pbuTradeBuySort + begin + 1, stock->pbuTradeBuySort + begin, sizeof(int) * (end - begin));
	stock->pbuTradeBuySort[begin] = pbuNo;
}*/


/*****************************************************************
** ������:      UpdatePBUSellTradeSortList
** �� ��:       stock, pbuNo
** �� ��:       ��
** ��������:    ά��pbu���ɽ�����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.19
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
/*static void 
UpdatePBUSellTradeSortList(STOCK_CS* stock, int pbuNo){
	PBU_HEAD*	pbuHead = NULL;
	PBU_HEAD*	pbuHeadCmp = NULL;
	PBU_DATA*	pbuData = NULL;
	PBU_DATA*	pbuDataCmp = NULL;
	int			begin = -1;
	int			end = -1;
	int			i = 0;
	
	if(stock == NULL || pbuNo < 0){
		ERROR(_moduleName, "UpdatePBUSellTradeSortList�����������");
		return;
	}
	
	pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
	if(pbuHead == NULL){
		ERROR(_moduleName, "UpdatePBUSellTradeSortList����δ�ҵ���Ҫ�����pbu������Ϣ");
		return;
	}
	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
	if(pbuData == NULL){
		ERROR(_moduleName, "UpdatePBUSellTradeSortList����pbu������Ϣ����");
		return;
	}
	
	
	for(i = PBU_TRADE_SORT_LENGTH - 1; i >= 0; i--){
		if(stock->pbuTradeSellSort[i] == -1){
			continue;
		}
		if(stock->pbuTradeSellSort[i] == pbuNo){
			end = i;
			continue;
		}
		
		pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, stock->pbuTradeSellSort[i]);
		if(pbuHeadCmp == NULL){
			ERROR(_moduleName, "UpdatePBUSellTradeSortList����δ�ҵ���Ҫ�Ƚϵ�pbu������Ϣ");
			return;
		}
		pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
		if(pbuDataCmp == NULL){
			ERROR(_moduleName, "UpdatePBUSellTradeSortList����δ�ҵ���Ҫ�Ƚϵ�pbu����");
			return;
		}
		
		if(pbuData->tradeSellAmount <= pbuDataCmp->tradeSellAmount){
			begin = i + 1;
			break;
		}
	}
	
	if(begin == PBU_TRADE_SORT_LENGTH){
		//Ҫ����ĳɽ�����С�����һ���ĳɽ���������˲���Ҫ��������
		return;
	}
	if(begin == -1){
		//���������ǿյģ�����Ҫ����ĳɽ�����Ӧ�����ڵ�һ��
		begin = 0;
	}
	if(end == -1){
		//Ҫ�����pbu�ɽ�û����ԭ��������������
		end = PBU_TRADE_SORT_LENGTH - 1;
	}
	
	if(begin == end){
		//����û�б仯��Ҫ����������pbu��Ȼ����ԭλ�����������������һλ
		stock->pbuTradeSellSort[begin] = pbuNo;
		return;
	}
	
	memmove(stock->pbuTradeSellSort + begin + 1, stock->pbuTradeSellSort + begin, sizeof(int) * (end - begin));
	stock->pbuTradeSellSort[begin] = pbuNo;
}*/



/*****************************************************************
** ������:      UpdatePBUBuyOrderSortList
** �� ��:       pbuNo:��Ҫ����������pbu
				sortBegin������ʼƫ�ƣ�sortEnd���ҽ���λ��ƫ��
** �� ��:       ��
** ��������:    pbu��ί����������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.18
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
/*static void 
UpdatePBUBuyOrderSortList(STOCK_CS* stock, int pbuNo, int sortBegin, int sortEnd){
	PBU_DATA*	pbuData = NULL;
	PBU_DATA*	pbuDataCmp = NULL;
	PBU_HEAD*	pbuHead = NULL;
	PBU_HEAD*	pbuHeadCmp = NULL;
	PBU_HEAD*	pbuHeadChange = NULL;
	int*		sortChange = NULL;
	int*		sortList = NULL;
	int*		sortInsert = NULL;
	int			begin = sortBegin;
	int			end = sortEnd;
	int			comp = -1;
	int			i = 0;
	
	pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
	
	sortList = GetPBUBuySortByStockNo(stock->stockNo, begin);
	if(sortEnd == -1){
		*sortList = pbuNo;
		pbuHead->pbuBuyOrderSortNo = begin;
		return;
	}
	pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
	pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
	
	//Ҫ�����pbuί���������ڵ�ǰ������λ��pbuί������
	if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
		memmove(sortList + 1, sortList, sizeof(int) * (end - begin + 1));
		*sortList = pbuNo;
		for(i = begin; i <= end + 1; i++){
			sortChange = GetPBUBuySortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuBuyOrderSortNo = i;
		}
		return;
	}
	
	sortList = GetPBUBuySortByStockNo(stock->stockNo, end);
	pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
	pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
	//Ҫ�����pbuί������С�ڵ�ǰ����ĩβ��pbuί������
	if(pbuData->orderBuyAmount < pbuDataCmp->orderBuyAmount){
		//������������0��ʼ��˵�����²�������ݣ�ֱ�Ӽӵ������б�β��
		if(begin == 0){
			*(sortList + 1) = pbuNo;
			pbuHead->pbuBuyOrderSortNo = end + 1;
			return;
		}
		memmove(sortList - (end - begin + 1), sortList - (end - begin), sizeof(int) * (end - begin + 1));
		*sortList = pbuNo;
		for(i = begin - 1; i <= end; i++){
			sortChange = GetPBUBuySortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuBuyOrderSortNo = i;
		}
		return;
	}
	
	//ʹ�ö��ַ�������Ҫ�����λ��
	while(end - begin > 1){
		comp = (end + begin) / 2;
		
		sortList = GetPBUBuySortByStockNo(stock->stockNo, comp);
		pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
		pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
		
		if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
			end = comp;
		}
		else if(pbuData->orderBuyAmount <= pbuDataCmp->orderBuyAmount){
			begin = comp;
		}
	}
	//��Ҫ�����λ����begin����end����Ҫ���ݲ��뷽�������Ҫ�ƶ�������
	//������������0��ʼ����ô����λ��Ϊend��end��������ݺ���
	if(sortBegin == 0){
		sortInsert = GetPBUBuySortByStockNo(stock->stockNo, end);
		memmove(sortInsert + 1, sortInsert, sizeof(int) * (sortEnd - end + 1));
		*sortInsert = pbuNo;
		for(i = end; i <= sortEnd; i++){
			sortChange = GetPBUBuySortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuBuyOrderSortNo = i;
		}
	}
	//����������䲻�Ǵ�0��ʼ����ô�����λ��Ϊbegin��beginǰ�������ǰ��
	else{
		sortInsert = GetPBUBuySortByStockNo(stock->stockNo, begin);
		memmove(sortInsert - (begin - sortBegin + 1), sortInsert - (begin - sortBegin), sizeof(int) * (begin - sortBegin + 1));
		*sortInsert = pbuNo;
		for(i = sortBegin - 1; i <= begin; i++){
			sortChange = GetPBUBuySortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuBuyOrderSortNo = i;
		}
	}
}*/


/*****************************************************************
** ������:      UpdatePBUSellOrderSortList
** �� ��:       pbuNo:��Ҫ����������pbu
				sortBegin������ʼƫ�ƣ�sortEnd���ҽ���λ��ƫ��
** �� ��:       ��
** ��������:    pbu��ί����������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.18
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
/*static void 
UpdatePBUSellOrderSortList(STOCK_CS* stock, int pbuNo, int sortBegin, int sortEnd){
	PBU_DATA*	pbuData = NULL;
	PBU_DATA*	pbuDataCmp = NULL;
	PBU_HEAD*	pbuHead = NULL;
	PBU_HEAD*	pbuHeadCmp = NULL;
	PBU_HEAD*	pbuHeadChange = NULL;
	int*		sortChange = NULL;
	int*		sortList = NULL;
	int*		sortInsert = NULL;
	int			begin = sortBegin;
	int			end = sortEnd;
	int			comp = -1;
	int			i = 0;
	
	pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
	
	sortList = GetPBUSellSortByStockNo(stock->stockNo, begin);
	if(sortEnd == -1){
		*sortList = pbuNo;
		pbuHead->pbuSellOrderSortNo = begin;
		return;
	}
	pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
	pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
	
	//Ҫ�����pbuί���������ڵ�ǰ������λ��pbuί������
	if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
		memmove(sortList + 1, sortList, sizeof(int) * (end - begin + 1));
		*sortList = pbuNo;
		for(i = begin; i <= end + 1; i++){
			sortChange = GetPBUSellSortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuSellOrderSortNo = i;
		}
		return;
	}
	
	sortList = GetPBUSellSortByStockNo(stock->stockNo, end);
	pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
	pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
	//Ҫ�����pbuί������С�ڵ�ǰ����ĩβ��pbuί������
	if(pbuData->orderSellAmount < pbuDataCmp->orderSellAmount){
		//������������0��ʼ��˵�����²�������ݣ�ֱ�Ӽӵ������б�β��
		if(begin == 0){
			*(sortList + 1) = pbuNo;
			pbuHead->pbuSellOrderSortNo = end + 1;
			return;
		}
		memmove(sortList - (end - begin + 1), sortList - (end - begin), sizeof(int) * (end - begin + 1));
		*sortList = pbuNo;
		for(i = begin - 1; i <= end; i++){
			sortChange = GetPBUSellSortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuSellOrderSortNo = i;
		}
		return;
	}
	
	//ʹ�ö��ַ�������Ҫ�����λ��
	while(end - begin > 1){
		comp = (end + begin) / 2;
		
		sortList = GetPBUSellSortByStockNo(stock->stockNo, comp);
		pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sortList);
		pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
		
		if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
			end = comp;
		}
		else if(pbuData->orderSellAmount <= pbuDataCmp->orderSellAmount){
			begin = comp;
		}
	}
	//��Ҫ�����λ����begin����end����Ҫ���ݲ��뷽�������Ҫ�ƶ�������
	//������������0��ʼ����ô����λ��Ϊend��end��������ݺ���
	if(sortBegin == 0){
		sortInsert = GetPBUSellSortByStockNo(stock->stockNo, end);
		memmove(sortInsert + 1, sortInsert, sizeof(int) * (sortEnd - end + 1));
		*sortInsert = pbuNo;
		for(i = end; i <= sortEnd; i++){
			sortChange = GetPBUSellSortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuSellOrderSortNo = i;
		}
	}
	//����������䲻�Ǵ�0��ʼ����ô�����λ��Ϊbegin��beginǰ�������ǰ��
	else{
		sortInsert = GetPBUSellSortByStockNo(stock->stockNo, begin);
		memmove(sortInsert - (begin - sortBegin + 1), sortInsert - (begin - sortBegin), sizeof(int) * (begin - sortBegin + 1));
		*sortInsert = pbuNo;
		for(i = sortBegin - 1; i <= begin; i++){
			sortChange = GetPBUSellSortByStockNo(stock->stockNo, i);
			pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
			pbuHeadChange->pbuSellOrderSortNo = i;
		}
	}
}*/


/*****************************************************************
** ������:      GetPBUData
** �� ��:       ��
** �� ��:       PBU_DATA*
** ��������:    ����PBU���ݵ���ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static PBU_DATA*	
GetPBUData(){
	#ifdef USE_PBU
		return (PBU_DATA*) GetShareMemory() + GetShareMemAllocationStats()->pbuDataOffset;
	#endif
	return NULL;
}


/*****************************************************************
** ������:      GetPBUTable
** �� ��:       ��
** �� ��:       PBU_HEAD*
** ��������:    ����PBU�����б����ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static PBU_HEAD*	
GetPBUTable(){
	#ifdef USE_PBU
		return (PBU_HEAD*) GetShareMemory() + GetShareMemAllocationStats()->pbuTableOffset;
	#endif
	return NULL;
}


/*****************************************************************
** ������:      GetPBUBuySort
** �� ��:       ��
** �� ��:       int*
** ��������:    ����PBU��ί����������ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static int*   
GetPBUBuySort(){
	#ifdef USE_PBU
		return (int*) GetShareMemory() + GetShareMemAllocationStats()->pbuBuySortOffset;
	#endif
	return NULL;
}


/*****************************************************************
** ������:      GetPBUSellSort
** �� ��:       ��
** �� ��:       int*
** ��������:    ����PBU��ί����������ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static int*   
GetPBUSellSort(){
	#ifdef USE_PBU
		return (int*) GetShareMemory() + GetShareMemAllocationStats()->pbuSellSortOffset;
	#endif
	return NULL;
}


/*****************************************************************
** ������:      CompFirm
** �� ��:       char**
** �� ��:       int
** ��������:    ���ַ���Ҫ�ıȽϺ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static int 
CompFirm(PBU_FIRM *p1, PBU_FIRM *p2){ 
	return strcmp(p1->pbuFirm, p2->pbuFirm);
}


/*****************************************************************
** ������:      PBUStatusChange
** �� ��:       stock
** �� ��:       ��
** ��������:    ����ʱ�α任����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2007.3.6
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void	
PBUStatusChange(STOCK_CS* stock){
	PBU_HEAD*		pbuHead = NULL;
	PBU_DATA*		pbuData = NULL;
	int				i = 0;
	
	if(stock == NULL){
		ERROR(_moduleName, "PBUStatusChange���������������ΪNULL");
		return;
	}
	
	for(i = 0; i < GetShareMemCurrentStats()->nextPBUFirm; i++){
		pbuHead = GetPBUTableByStockNo(stock->stockNo, i);
		if(pbuHead == NULL){
			ERROR(_moduleName, "PBUStatusChange��������pbu������Ϣʧ��");
			return;
		}
		if(pbuHead->pbuDataAdr == -1){
			continue;
		}
		pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
		
		//����ǰ����ʱ���ǿ��̼��Ͼ�����
		if(strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_OCALL, strlen(STOCK_TRADING_RESTRICTION_OCALL))==0){
			//ȥ��OA������ͳ��	
			pbuData->orderBuyAmount -= pbuData->orderOABuyAmount;
			pbuData->orderBuyMoney -= pbuData->orderOABuyMoney;
			pbuData->orderSellAmount -= pbuData->orderOASellAmount;
			pbuData->orderSellMoney -= pbuData->orderOASellMoney;
			//ȥ��AU������ͳ��
			pbuData->orderBuyAmount -= pbuData->orderAUBuyAmount;
			pbuData->orderBuyMoney -= pbuData->orderAUBuyMoney;
			pbuData->orderSellAmount -= pbuData->orderAUSellAmount;
			pbuData->orderSellMoney -= pbuData->orderAUSellMoney;
		}
		//����ǰ����ʱ�������м��Ͼ�����
		else if(strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_ICALL, strlen(STOCK_TRADING_RESTRICTION_ICALL))==0){
			//ȥ��AU������ͳ��
			pbuData->orderBuyAmount -= pbuData->orderAUBuyAmount;
			pbuData->orderBuyMoney -= pbuData->orderAUBuyMoney;
			pbuData->orderSellAmount -= pbuData->orderAUSellAmount;
			pbuData->orderSellMoney -= pbuData->orderAUSellMoney;
		}
		//����ǰ����ʱ���Ƕ�����ƽ��ʱ����
		else if(strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_IOBB, strlen(STOCK_TRADING_RESTRICTION_IOBB))==0 || strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IPOBB,strlen(STOCK_TRADING_RESTRICTION_IPOBB))==0 || strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OOBB,strlen(STOCK_TRADING_RESTRICTION_OOBB))==0 || strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OPOBB,strlen(STOCK_TRADING_RESTRICTION_OPOBB))==0){
			//ȥ��SU������ͳ��	
			pbuData->orderBuyAmount -= pbuData->orderSUBuyAmount;
			pbuData->orderBuyMoney -= pbuData->orderSUBuyMoney;
			pbuData->orderSellAmount -= pbuData->orderSUSellAmount;
			pbuData->orderSellMoney -= pbuData->orderSUSellMoney;
		}
		//����ǰ����ʱ���ǹ̶��۸���ʱ����
		else if(strncmp(stock->prePrcsStsValCod, STOCK_TRADING_RESTRICTION_FCALL, strlen(STOCK_TRADING_RESTRICTION_FCALL))==0){
			//ȥ��FP������ͳ��
			pbuData->orderBuyAmount -= pbuData->orderFPBuyAmount;
			pbuData->orderBuyMoney -= pbuData->orderFPBuyMoney;
			pbuData->orderSellAmount -= pbuData->orderFPSellAmount;
			pbuData->orderSellMoney -= pbuData->orderFPSellMoney;
		}
		
		//�½���ʱ���ǿ��̼��Ͼ�����
		if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OCALL,strlen(STOCK_TRADING_RESTRICTION_OCALL))==0){
			//����OA������ͳ��
			pbuData->orderBuyAmount += pbuData->orderOABuyAmount;
			pbuData->orderBuyMoney += pbuData->orderOABuyMoney;
			pbuData->orderSellAmount += pbuData->orderOASellAmount;
			pbuData->orderSellMoney += pbuData->orderOASellMoney;
			//����AU������ͳ��
			pbuData->orderBuyAmount += pbuData->orderAUBuyAmount;
			pbuData->orderBuyMoney += pbuData->orderAUBuyMoney;
			pbuData->orderSellAmount += pbuData->orderAUSellAmount;
			pbuData->orderSellMoney += pbuData->orderAUSellMoney;
		}
		//�½���ʱ�������м��Ͼ�����
		else if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_ICALL,strlen(STOCK_TRADING_RESTRICTION_ICALL))==0){
			//����AU������ͳ��
			pbuData->orderBuyAmount += pbuData->orderAUBuyAmount;
			pbuData->orderBuyMoney += pbuData->orderAUBuyMoney;
			pbuData->orderSellAmount += pbuData->orderAUSellAmount;
			pbuData->orderSellMoney += pbuData->orderAUSellMoney;
		}
		//�½���ʱ���Ƕ�����ƽ��ʱ����
		else if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IOBB,strlen(STOCK_TRADING_RESTRICTION_IOBB))==0 || \
			strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_IPOBB,strlen(STOCK_TRADING_RESTRICTION_IPOBB))==0 || \
			strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OOBB,strlen(STOCK_TRADING_RESTRICTION_OOBB))==0 || \
			strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_OPOBB,strlen(STOCK_TRADING_RESTRICTION_OPOBB))==0){
			//����SU������ͳ��
			pbuData->orderBuyAmount += pbuData->orderSUBuyAmount;
			pbuData->orderBuyMoney += pbuData->orderSUBuyMoney;
			pbuData->orderSellAmount += pbuData->orderSUSellAmount;
			pbuData->orderSellMoney += pbuData->orderSUSellMoney;
		}
		//�½���ʱ���Ƕ��̶��۸񾺼�ʱ����
		else if(strncmp(stock->prcsStsValCod,STOCK_TRADING_RESTRICTION_FCALL,strlen(STOCK_TRADING_RESTRICTION_FCALL))==0){
			//����FP������ͳ��
			pbuData->orderBuyAmount += pbuData->orderFPBuyAmount;
			pbuData->orderBuyMoney += pbuData->orderFPBuyMoney;
			pbuData->orderSellAmount += pbuData->orderFPSellAmount;
			pbuData->orderSellMoney += pbuData->orderFPSellMoney;
		}
		
	}
	return;
}


/*****************************************************************
** ������:      ResortPBUSortList
** �� ��:       stock
** �� ��:       ��
** ��������:    PBU������������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2007.3.6
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
ResortPBUSortList(STOCK_CS* stock){
	PBU_HEAD*		pbuHead = NULL;
	PBU_HEAD*		pbuHeadCmp = NULL;
	PBU_HEAD*		pbuHeadChange = NULL;
	PBU_DATA*		pbuData = NULL;
	PBU_DATA*		pbuDataCmp = NULL;
	int*			buySortList = NULL;
	int*			sellSortList = NULL;
	int*			sortInsert = NULL;
	int*			sortChange = NULL;
	int				begin = -1;
	int				end = -1;
	int				comp = -1;
	int				i = 0;
	
	if(stock == NULL){
		ERROR(_moduleName, "PBUStatusChange���������������ΪNULL");
		return;
	}
	
	//�����б����
	stock->lastBuyOrderSortOffset = -1;
	stock->lastSellOrderSortOffset = -1;
	//��������
	for(i = 0; i < GetShareMemCurrentStats()->nextPBUFirm; i++){
		pbuHead = GetPBUTableByStockNo(stock->stockNo, i);
		if(pbuHead == NULL){
			ERROR(_moduleName, "ResortPBUSortList ��������pbu������Ϣʧ��");
			return;
		}
		if(pbuHead->pbuDataAdr == -1){
			continue;
		}
		pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
		
		//����������
		//�����б��
		if(stock->lastBuyOrderSortOffset == -1){
			buySortList = GetPBUBuySortByStockNo(stock->stockNo, 0);
			*buySortList = i;
			stock->lastBuyOrderSortOffset++;
		}
		else{
			//���һ���Ƚ�
			buySortList = GetPBUBuySortByStockNo(stock->stockNo, 0);
			pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *buySortList);
			if(pbuHeadCmp == NULL){
				ERROR(_moduleName, "ResortPBUSortList ��������pbu������Ϣʧ��");
				return;
			}
			pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
			if(pbuDataCmp == NULL){
				ERROR(_moduleName, "ResortPBUSortList ��������pbu����ʧ��");
				return;
			}
			
			if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
				memmove(buySortList + 1, buySortList, sizeof(int)*(stock->lastBuyOrderSortOffset + 1));
				*buySortList = i;
				stock->lastBuyOrderSortOffset++;
			}
			else{
				//�����һ���Ƚ�
				buySortList = GetPBUBuySortByStockNo(stock->stockNo, stock->lastBuyOrderSortOffset);
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *buySortList);
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "ResortPBUSortList ��������pbu������Ϣʧ��");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "ResortPBUSortList ��������pbu����ʧ��");
					return;
				}
				if(pbuData->orderBuyAmount < pbuDataCmp->orderBuyAmount){
					*(buySortList + 1) = i;
					stock->lastBuyOrderSortOffset++;
				}
				else{
					//���ַ�����Ҫ�����λ��(end�ǽ�Ҫ�����λ�ã�end��������ݺ���)
					begin = 0;
					end = stock->lastBuyOrderSortOffset;
					while(end - begin > 1){
						comp = (end + begin) / 2;
					
						buySortList = GetPBUBuySortByStockNo(stock->stockNo, comp);
						pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *buySortList);
						if(pbuHeadCmp == NULL){
							ERROR(_moduleName, "ResortPBUSortList ��������pbu������Ϣʧ��");
							return;
						}
						pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
						
						if(pbuData->orderBuyAmount > pbuDataCmp->orderBuyAmount){
							end = comp;
						}
						else if(pbuData->orderBuyAmount <= pbuDataCmp->orderBuyAmount){
							begin = comp;
						}
					}
					
					sortInsert = GetPBUBuySortByStockNo(stock->stockNo, end);
					memmove(sortInsert + 1, sortInsert, sizeof(int) * (stock->lastBuyOrderSortOffset - end + 1));
					*sortInsert = i;
					stock->lastBuyOrderSortOffset++;
				}
			}
		}
		
		//����������
		//�����б��
		if(stock->lastSellOrderSortOffset == -1){
			sellSortList = GetPBUSellSortByStockNo(stock->stockNo, 0);
			*sellSortList = i;
			stock->lastSellOrderSortOffset++;
		}
		else{
			//���һ���Ƚ�
			sellSortList = GetPBUSellSortByStockNo(stock->stockNo, 0);
			pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sellSortList);
			if(pbuHeadCmp == NULL){
				ERROR(_moduleName, "ResortPBUSortList ��������pbu������Ϣʧ��");
				return;
			}
			pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
			if(pbuDataCmp == NULL){
				ERROR(_moduleName, "ResortPBUSortList ��������pbu����ʧ��");
				return;
			}
			
			if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
				memmove(sellSortList + 1, sellSortList, sizeof(int)*(stock->lastSellOrderSortOffset + 1));
				*sellSortList = i;
				stock->lastSellOrderSortOffset++;
			}
			else{
				//�����һ���Ƚ�
				sellSortList = GetPBUSellSortByStockNo(stock->stockNo, stock->lastSellOrderSortOffset);
				pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sellSortList);
				if(pbuHeadCmp == NULL){
					ERROR(_moduleName, "ResortPBUSortList ��������pbu������Ϣʧ��");
					return;
				}
				pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
				if(pbuDataCmp == NULL){
					ERROR(_moduleName, "ResortPBUSortList ��������pbu����ʧ��");
					return;
				}
				if(pbuData->orderSellAmount < pbuDataCmp->orderSellAmount){
					*(sellSortList + 1) = i;
					stock->lastSellOrderSortOffset++;
				}
				else{
					//���ַ�����Ҫ�����λ��(end�ǽ�Ҫ�����λ�ã�end��������ݺ���)
					begin = 0;
					end = stock->lastSellOrderSortOffset;
					while(end - begin > 1){
						comp = (end + begin) / 2;
					
						sellSortList = GetPBUSellSortByStockNo(stock->stockNo, comp);
						pbuHeadCmp = GetPBUTableByStockNo(stock->stockNo, *sellSortList);
						if(pbuHeadCmp == NULL){
							ERROR(_moduleName, "ResortPBUSortList ��������pbu������Ϣʧ��");
							return;
						}
						pbuDataCmp = GetPBUDataByIndex(pbuHeadCmp->pbuDataAdr);
						
						if(pbuData->orderSellAmount > pbuDataCmp->orderSellAmount){
							end = comp;
						}
						else if(pbuData->orderSellAmount <= pbuDataCmp->orderSellAmount){
							begin = comp;
						}
					}
					
					sortInsert = GetPBUSellSortByStockNo(stock->stockNo, end);
					memmove(sortInsert + 1, sortInsert, sizeof(int) * (stock->lastSellOrderSortOffset - end + 1));
					*sortInsert = i;
					stock->lastSellOrderSortOffset++;
				}
			}
		}
	}
	//����pbuHead�е�������Ϣ
	for(i = 0; i <= stock->lastBuyOrderSortOffset; i++){
		sortChange = GetPBUBuySortByStockNo(stock->stockNo, i);
		pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
		if(pbuHeadChange == NULL){
			ERROR(_moduleName, "ResortPBUSortList ��������pbu������Ϣʧ��");
			return;
		}
		pbuHeadChange->pbuBuyOrderSortNo = i;
	}
	for(i = 0; i <= stock->lastSellOrderSortOffset; i++){
		sortChange = GetPBUSellSortByStockNo(stock->stockNo, i);
		pbuHeadChange = GetPBUTableByStockNo(stock->stockNo, *sortChange);
		if(pbuHeadChange == NULL){
			ERROR(_moduleName, "ResortPBUSortList ��������pbu������Ϣʧ��");
			return;
		}
		pbuHeadChange->pbuSellOrderSortNo = i;
	}
}



/*****************************************************************
** ������:      PrintPBUList
** �� ��:       stock
** �� ��:       ��
** ��������:    ���pbu�����ͳɽ��������ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2007.3.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintPBUList(){
	char        buf[2048];
	int     	fp = 0;
    char    	filePath[256];
    STOCK_CS*	stock = NULL;
    PBU_FIRM*	pbuFirm = NULL;
    PBU_HEAD*	pbuHead = NULL;
    PBU_DATA*	pbuData = NULL;
    int*		pbuBuyList = NULL;
    int*		pbuSellList = NULL;
    int			i = 0;
    int			j = 0;

	if (!IsHomeValid()) {
        return;
    }
    
    sprintf(filePath, "%s/PbuOrderList%s.txt", GetHome(), GetTime());
    
	if ((fp = open(filePath, O_WRONLY | O_CREAT | O_APPEND,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
        printf("ERROR: �����ļ�[%s]ʧ��! \n", filePath);
        return;
    }
    
    for (i = 0; i < GetStockNumber(); i++) {
    	stock = GetStock(i);
    	if(stock == NULL){
    		continue;
    	}
        memset(buf, 0, 2048);
        
        for(j = 0; j < 5; j++){
        	pbuBuyList = GetPBUBuySortByStockNo(stock->stockNo, j);
        	pbuHead = GetPBUTableByStockNo(stock->stockNo, *pbuBuyList);
        	if(pbuHead == NULL){
        		continue;
        	}
        	pbuFirm = GetPBUFirmByPbuNo(*pbuBuyList);
        	if(pbuFirm == NULL){
        		continue;
        	}
        	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
        	if(pbuData == NULL){
        		continue;
        	}
        	
        	sprintf(buf, "--buyOrderList=[%8d]--isinCod=[%12s]--pbuFirm=[%8s]--pbuBuyOrderAmount=[%16lld]--\n",
        		 j,
        		 stock->isinCod,
        		 pbuFirm->pbuFirm,
        		 pbuData->orderBuyAmount);
        	write(fp, buf, strlen(buf));
        }
        
        memset(buf, 0, 2048);
        for(j = 0; j < 5; j++){
        	pbuSellList = GetPBUSellSortByStockNo(stock->stockNo, j);
        	pbuHead = GetPBUTableByStockNo(stock->stockNo, *pbuSellList);
        	if(pbuHead == NULL){
        		continue;
        	}
        	pbuFirm = GetPBUFirmByPbuNo(*pbuSellList);
        	if(pbuFirm == NULL){
        		continue;
        	}
        	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
        	if(pbuData == NULL){
        		continue;
        	}
        	
        	sprintf(buf, "--sellOrderList=[%8d]--isinCod=[%12s]--pbuFirm=[%8s]--pbuBuyOrderAmount=[%16lld]--\n",
        		 j,
        		 stock->isinCod,
        		 pbuFirm->pbuFirm,
        		 pbuData->orderSellAmount);
        	write(fp, buf, strlen(buf));
        }
        
        memset(buf, 0, 2048);
        for(j = 0; j < PBU_TRADE_SORT_LENGTH; j++){
        	pbuHead = GetPBUTableByStockNo(stock->stockNo, stock->pbuTradeBuySort[j]);
        	if(pbuHead == NULL){
        		continue;
        	}
        	pbuFirm = GetPBUFirmByPbuNo(stock->pbuTradeBuySort[j]);
        	if(pbuFirm == NULL){
        		continue;
        	}
        	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
        	if(pbuData == NULL){
        		continue;
        	}
        	sprintf(buf, "--buyTradeList=[%8d]--isinCod=[%12s]--pbuFirm=[%8s]--tradeBuyAmount=[%16lld]--\n",
        		 j,
        		 stock->isinCod,
        		 pbuFirm->pbuFirm,
        		 pbuData->tradeBuyAmount);
        	write(fp, buf, strlen(buf));
        }
        
        memset(buf, 0, 2048);
        for(j = 0; j < PBU_TRADE_SORT_LENGTH; j++){
        	pbuHead = GetPBUTableByStockNo(stock->stockNo, stock->pbuTradeSellSort[j]);
        	if(pbuHead == NULL){
        		continue;
        	}
        	pbuFirm = GetPBUFirmByPbuNo(stock->pbuTradeSellSort[j]);
        	if(pbuFirm == NULL){
        		continue;
        	}
        	pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
        	if(pbuData == NULL){
        		continue;
        	}
        	sprintf(buf, "--sellTradeList=[%8d]--isinCod=[%12s]--pbuFirm=[%8s]--tradeSellAmount=[%16lld]--\n",
        		 j,
        		 stock->isinCod,
        		 pbuFirm->pbuFirm,
        		 pbuData->tradeSellAmount);
        	write(fp, buf, strlen(buf));
        }
    }
    close(fp);
}
