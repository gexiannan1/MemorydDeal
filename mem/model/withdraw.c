/******************************************************************
** �ļ���:      withdraw.c
** �� ��:       0.1
** ������:      wugb
** �� ��:       2006.9.11
** �� ��:       Withdraw��Ϣ����
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/withdraw.c,v 1.1 2012/03/29 01:19:55 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:55 $
 */


#include	<stdlib.h> 
#include	<stdio.h>
#include    <string.h>
#include    <fcntl.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include	<unistd.h>

#include    "withdraw.h"
#include    "model.h"
#include    "order.h"
#include    "stock.h"
#include	"price.h"
#include    "../memory.h"
#include    "../env/env.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"
#include    "../util/time/common_time.h"
#include    "../util/log/log.h"

/*
 * ģ������
 */
static const char   _moduleName[] = "��������";

static void UpdateWithdrawSortList(int *sortList, WITHDRAW_CS *withdraw, ORDER_CS *order);
/*****************************************************************
** ������:      GetWithdrawList
** �� ��:       ��
** �� ��:       WITHDRAW_CS[]
** ��������:    ����Withdraw�������ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
WITHDRAW_CS*
GetWithdrawList() {
    return (WITHDRAW_CS*) GetShareMemory() + GetShareMemAllocationStats()->withdrawOffset;
}


/*****************************************************************
** ������:      GetWithdraw
** �� ��:       ��
** �� ��:       WITHDRAW_CS*
** ��������:    ����ָ��λ�õ�WITHDRAW_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
WITHDRAW_CS*
GetWithdraw(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->withdrawMax) {
        return NULL;
    }
    return GetWithdrawList() + index;
}


/*****************************************************************
** ������:      GetWithdrawNumber
** �� ��:       ��
** �� ��:       Withdraw����
** ��������:    ����Withdraw����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetWithdrawNumber() {
    return GetShareMemCurrentStats()->nextWithdraw;
}


/*****************************************************************
** ������:      GetLastWithdrawTime
** �� ��:       ��
** �� ��:       ����ʱ�� (hhmmss)
** ��������:    �������һ�ʳ���ʱ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetLastWithdrawTime() {
    int index = GetShareMemCurrentStats()->nextWithdraw;
    if (index > 0) {
        return GetWithdraw(index - 1)->orderTime;
    }
    return 0;
}


/*****************************************************************
** ������:      AddWithdraw
** �� ��:       ��
** �� ��:       ������Withdraw��λ��
** ��������:    ��Withdraw����������Withdraw��Ŀ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
AddWithdraw(ORDER_CHANGE_TE *teWithdraw) {
    WITHDRAW_CS*	target = NULL;
    ORDER_CS*		order = NULL;
    int         	index   = GetShareMemCurrentStats()->nextWithdraw;
	
    if (index >= GetShareMemConfigStats()->withdrawMax) {
        FATAL("Withdraw.AddWithdraw()", "������������Ԥ�������󳷵������� Current Number: %d"
                , index);
        return -1;
    }

    target = GetWithdraw(index);
    
    target->orderNo				= teWithdraw->orderNo;
    target->orderNoOld			= teWithdraw->orderNoOld;
    target->orderNoOld1			= teWithdraw->orderNoOld1;
    target->orderProcessNo	  	= teWithdraw->orderProcessNo;
    target->orderProcessNoOld	= teWithdraw->orderProcessNoOld;
    target->orderProcessNoOld1	= teWithdraw->orderProcessNoOld1;
    target->orderTime			= teWithdraw->orderTime;
	target->ordrBuyCod			= teWithdraw->orderBSType;
	target->ordrTypCod			= teWithdraw->orderType;
	target->price				= teWithdraw->orderPrice / ONE_HUNDRAD;
	target->qty					= teWithdraw->orderQty;
	target->remPeakQty			= teWithdraw->remPeakQty;
	target->orderStatus			= teWithdraw->orderStatus;
	target->peakSizeQty			= teWithdraw->peakSizeQty;
	target->trnTypId			= teWithdraw->trnTypId;
	memcpy(target->isinCod, teWithdraw->isinCod, STOCK_ISIN_CODE_LENGTH);
	memcpy(target->pbuId, teWithdraw->pbuId, PBU_FIRM_LENGTH);

    target->nextStock = NULL_INDEX;
    if(teWithdraw->orderProcessNo<0)
	{
		ERROR(_moduleName, "��������ʧ��! teWithdraw->orderProcessNo[%d]",teWithdraw->orderProcessNo);
	}
    order = GetOrder(target->orderProcessNo);
    if(order == NULL){
    	GetShareMemCurrentStats()->nextWithdraw++;
    	return -1;
    }
    target->stockNo = order->stockNo;
	target->withdrawProcessNo = index;
	
    GetShareMemCurrentStats()->nextWithdraw++;
    
    return index;
}


/*****************************************************************
** ������:      GetWorkedWithdrawNumber
** �� ��:       ��
** �� ��:       �Ѵ�����ĳ�������
** ��������:    ���ص�ǰ�Ѵ�����ĳ�����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetWorkedWithdrawNumber() {
    return GetMemTradeStats()->workedWithdrawNumber;
}


/*****************************************************************
** ������:      SetWorkedWithdrawNumber
** �� ��:       ��ǰ�Ѵ�����ĳ�����
** �� ��:       ��
** ��������:    ���õ�ǰ�Ѵ�����ĳ�����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetWorkedWithdrawNumber(int workedNumber) {
    GetMemTradeStats()->workedWithdrawNumber = workedNumber;
}


/*****************************************************************
** ������:      IncreaseWorkedWithdrawNumber
** �� ��:       ��
** �� ��:       ��
** ��������:    ������ǰ�Ѵ�����ĳ�����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
IncreaseWorkedWithdrawNumber() {
    GetMemTradeStats()->workedWithdrawNumber++;
}


/*****************************************************************
** ������:      CancelOrder
** �� ��:       WITHDRAW_CS *
** �� ��:       BOOL
** ��������:    ���ݼӹ�-ȡ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL 
CancelOrder(WITHDRAW_CS* csWithdraw){
	#ifdef USE_PBU
		int			pbuNo = -1;
		PBU_HEAD*	pbuHead = NULL;
		PBU_DATA*	pbuData = NULL;
	#endif
	STOCK_CS	*stock		= NULL;
	ORDER_CS    *order		= NULL;
	
	stock = GetStock(csWithdraw->stockNo);
	if(stock == NULL){
		ERROR(_moduleName, "��ȡ��Ʊ��Ϣʧ��");
		return FALSE;
	}
	
	//�ж��Ƿ�Ϊ���۶���
	if(csWithdraw->ordrTypCod == 'Q')
	{
		order = GetOrder(csWithdraw->orderProcessNoOld);
	}else{
		order = GetOrder(csWithdraw->orderProcessNo);
	}
		
	//�Ӷ�������ɾ������
	if(order == NULL){
		ERROR(_moduleName, "��ȡ������Ϣʧ��");
		return FALSE;
	}
	
	if(!DetachOrder(order)){
    	ERROR(_moduleName, "�Ѷ����Ӷ�������ɾ��ʧ��! λ��order->orderNo[%d]",order->orderNo);
    	return FALSE;
    }
    StockStatDel(order, order->balance);
    
    #ifdef USE_PBU
		pbuNo = GetPBUNoByFirm(order->pbuId);
		if(pbuNo < 0){
			ERROR(_moduleName, "����pbuί����Ϣʧ�ܣ�δ�ҵ���Ӧ��pbu����");
			return;
		}
		
		pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
		if(pbuHead == NULL){
			ERROR(_moduleName, "����pbuί����Ϣʧ�ܣ�δ�ҵ���Ӧ��pbu������");
			return;
		}
		
		pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
		if(pbuData == NULL){
			ERROR(_moduleName, "����pbuί����Ϣʧ�ܣ�δ�ҵ���Ӧ��pbu����");
			return;
		}
		
		//pbuͳ��
		PBUOrderStatDel(stock, pbuData, order, order->balance);
		//ά��pbuί������
		UpdatePBUOrderSortList(stock, pbuNo, order);
	#endif
    
    //Ϊ�˷�ֹ���Ͷ�����ʣ������Ϊ0��ȡ��������ԭ�ж�����ʣ��������������Ϊ0����Ϊ���ö���״̬Ϊ����״̬
    //order->balance = 0;
    order->status = ORDER_STATUS_WITHDRAWN;
    
    TRACE(_moduleName, "[STOCK] stockNo=[%-4d]--orderBuyNumber=[%-4d]--orderBuyAmount=[%-16lld]--orderSellNumber=[%-4d]--orderSellAmount=[%-16lld]",
			stock->stockNo,
			stock->orderBuyNumber,
			stock->orderBuyAmount,
			stock->orderSellNumber,
			stock->orderSellAmount
         );
    
    return TRUE;
}

/*****************************************************************
** ������:      ModifyOrder
** �� ��:       WITHDRAW_CS *, ORDER_CS *
** �� ��:       BOOL
** ��������:    ���ݼӹ�-�޸Ķ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL 
ModifyOrder(WITHDRAW_CS * csWithdraw){
	#ifdef USE_PBU
		int			pbuNo = -1;
		PBU_HEAD*	pbuHead = NULL;
		PBU_DATA*	pbuData = NULL;
	#endif
	STOCK_CS	*stock                  = NULL;
	ORDER_CS    *order                  = NULL;
	BID_CS		*bid					= NULL;
	OFFER_CS	*offer					= NULL;
	
	//���ݶ�������Ų��Ҷ�Ӧ�Ķ���
	order = GetOrder(csWithdraw->orderProcessNo);
	stock = GetStock(csWithdraw->stockNo);
	if(stock == NULL){
		ERROR(_moduleName, "��ȡ��Ʊ��Ϣʧ��");
		return FALSE;
	}
	if(order == NULL){
		ERROR(_moduleName, "��ȡ������Ϣʧ��");
		return FALSE;
	}
	
	#ifdef USE_PBU
		pbuNo = GetPBUNoByFirm(order->pbuId);
		if(pbuNo < 0){
			ERROR(_moduleName, "����pbuί����Ϣʧ�ܣ�δ�ҵ���Ӧ��pbu����");
			return;
		}
		
		pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
		if(pbuHead == NULL){
			ERROR(_moduleName, "����pbuί����Ϣʧ�ܣ�δ�ҵ���Ӧ��pbu������");
			return;
		}
		
		pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
		if(pbuData == NULL){
			ERROR(_moduleName, "����pbuί����Ϣʧ�ܣ�δ�ҵ���Ӧ��pbu����");
			return;
		}
	#endif
	
	//�ж��Ƿ�Ϊ����,YΪ���ᣬNΪ����
	if(csWithdraw->orderStatus == 'N'){
		if(order->balance != csWithdraw->qty){
			//�޸Ķ�����������֤ȯ��Ϣ��ۼ�ί������Ϊ��ԭί������-�޸�������
			if(!IsIceOrder(order)){
				if(IsBuyWithdraw(csWithdraw)){
					if((bid = GetBid(order->priceLevel)) == NULL){
						ERROR(_moduleName,"��ȡ���λ����ʧ��");
						return FALSE;
					}
					//��ͳ����Ϣ�м�ȥδ�޸�ǰ�Ķ�����Ϣ
					if(IsTradingPhase(order)){
						UpdateStockDel(stock, order);
						bid->orderAmount -= order->balance;
					}
					StockStatDel(order, order->balance);
					BidStatDel(bid, order, order->balance);
					
					#ifdef USE_PBU
						//pbuͳ��
						PBUOrderStatDel(stock, pbuData, order, order->balance);
					#endif
					
					//���³����ۼ�����
					stock->withdrawBuyAmount += (order->balance - csWithdraw->qty);
					stock->withdrawBuyMoney += (stock->lotSize * (order->balance - csWithdraw->qty) / TEN_THOUSAND * order->orderPrice);
					//�޸Ķ�������
					order->orderQty += (csWithdraw->qty - order->balance);
					order->balance = csWithdraw->qty;
					//�����޸ĺ�Ķ�����ͳ����Ϣ��
					if(IsTradingPhase(order)){
						UpdateStockAdd(stock, order);
						bid->orderAmount += order->balance;
					}
					StockStatAdd(order);
					BidStatAdd(bid, order);
					
					#ifdef USE_PBU
						//pbuͳ��
						PBUOrderStatAdd(stock, pbuData, order);
						//ά��pbuί������
						UpdatePBUOrderSortList(stock, pbuNo, order);
					#endif
				}
				else if(IsSellWithdraw(csWithdraw)){
					if((offer = GetOffer(order->priceLevel)) == NULL){
						ERROR(_moduleName,"��ȡ����λ����ʧ��");
						return FALSE;
					}
					//��ͳ����Ϣ�м�ȥδ�޸�ǰ�Ķ�����Ϣ
					if(IsTradingPhase(order)){
						UpdateStockDel(stock, order);
						offer->orderAmount -= order->balance;
					}
					StockStatDel(order, order->balance);
					OfferStatDel(offer, order, order->balance);
					
					#ifdef USE_PBU
						//pbuͳ��
						PBUOrderStatDel(stock, pbuData, order, order->balance);
					#endif
					
					//���³����ۼ�����
					stock->withdrawSellAmount += (order->orderQty - csWithdraw->qty);
					stock->withdrawSellMoney += (stock->lotSize * (order->balance - csWithdraw->qty) / TEN_THOUSAND * order->orderPrice);
					//�޸Ķ�������
					order->orderQty += (csWithdraw->qty - order->balance);
					order->balance = csWithdraw->qty;
					//�����޸ĺ�Ķ�����ͳ����Ϣ��
					if(IsTradingPhase(order)){
						UpdateStockAdd(stock, order);
						offer->orderAmount += order->balance;
					}
					StockStatAdd(order);
					OfferStatAdd(offer, order);
					
					#ifdef USE_PBU
						//pbuͳ��
						PBUOrderStatAdd(stock, pbuData, order);
						//ά��pbuί������
						UpdatePBUOrderSortList(stock, pbuNo, order);
					#endif
				}
			}
			else{
				//����Ǳ�ɽ����������Ҫ�޸��κ�ͳ����Ϣ����Ϊ��ɽ�����Ľ�ʾ���������޸�
				TRACE(_moduleName, "����������ɽ������");
	    		order->orderQty += (csWithdraw->qty - order->balance);
				order->balance = csWithdraw->qty;
			}
		}

		if(strcmp(csWithdraw->trdResTypCod, order->trdResTypCod) != 0){
			//�ж�ԭ���������Ʒ����ʱ����
			if(IsTradingPhase(order)){
				//ɾ��Order������������ͳ����Ϣ
				StockStatDel(order, order->balance);
			  	
			  	#ifdef USE_PBU
					//pbuͳ��
					PBUOrderStatDel(stock, pbuData, order, order->balance);
				#endif
			  	
			  	//�޸Ķ�������ʱ������
				strcpy(order->trdResTypCod, csWithdraw->trdResTypCod);
				
				//����Order������������ͳ����Ϣ
				StockStatAdd(order);
				
				#ifdef USE_PBU
					//pbuͳ��
					PBUOrderStatAdd(stock, pbuData, order);
					//ά��pbuί������
					UpdatePBUOrderSortList(stock, pbuNo, order);
				#endif
				
				//�޸ĺ󶩵������Բμӵ�ǰ����ʱ����
				if(!IsTradingPhase(order)) {
					//����֤ȯͳ����Ϣ�����ٶ��������Ͷ�������
					UpdateStockDel(stock, order);
					//���¼�λ���еĽ���ʱ��ͳ����Ϣ
					if(IsBuyOrder(order)){
						if((bid = GetBid(order->priceLevel)) == NULL){
							ERROR(_moduleName,"��ȡ���λ����ʧ��");
							return FALSE;
						}
						BidStatDel(bid, order, order->balance);
					}
					else if(IsSellOrder(order)){
						if((offer = GetOffer(order->priceLevel)) == NULL){
							ERROR(_moduleName,"��ȡ����λ����ʧ��");
							return FALSE;
						}
						OfferStatDel(offer, order, order->balance);
					}
				}
				
			}
			else{
				//ɾ��Order������������ͳ����Ϣ
				StockStatDel(order, order->balance);
			  	
			  	#ifdef USE_PBU
					//pbuͳ��
					PBUOrderStatDel(stock, pbuData, order, order->balance);
				#endif
			  	
			  	//�޸Ķ�������ʱ������
				strcpy(order->trdResTypCod, csWithdraw->trdResTypCod);
				
				//����Order������������ͳ����Ϣ
				StockStatAdd(order);
			    
			    #ifdef USE_PBU
					//pbuͳ��
					PBUOrderStatAdd(stock, pbuData, order);
					//ά��pbuί������
					UpdatePBUOrderSortList(stock, pbuNo, order);
				#endif
			    
			    //�޸ĺ󶩵������Բμӵ�ǰ����ʱ����
				if(IsTradingPhase(order)) {
					//����֤ȯͳ����Ϣ�����Ӷ��������Ͷ�������
					UpdateStockAdd(stock, order);
					//���¼�λ���еĽ���ʱ��ͳ����Ϣ
					if(IsBuyOrder(order)){
						if((bid = GetBid(order->priceLevel)) == NULL){
							ERROR(_moduleName,"��ȡ���λ����ʧ��");
							return FALSE;
						}
						BidStatAdd(bid, order);
					}
					else if(IsSellOrder(order)){
						if((offer = GetOffer(order->priceLevel)) == NULL){
							ERROR(_moduleName,"��ȡ����λ����ʧ��");
							return FALSE;
						}
						OfferStatAdd(offer, order);
					}
				}
			}
		}
		
		//�ж��Ƿ�Ϊ��ɽ��������߽�ʾ������ԭ������ͬ�޸Ķ�������߽�ʾ����
		if(IsIceOrder(order)){
			if(csWithdraw->peakSizeQty != order->peakSizeQty){
				TRACE(_moduleName, "���������޸ı�ɽ������߽�ʾ������ԭ��߽�ʾ����[%lld]������߽�ʾ����[%lld]",order->peakSizeQty,csWithdraw->peakSizeQty);
				order->peakSizeQty = csWithdraw->peakSizeQty;
			}
		}
	}
	else if(csWithdraw->orderStatus=='Y'){
		//�Ӷ�������ɾ������
		TRACE(_moduleName, "���ᶩ������ʼorderNo=[%d]",order->orderNo);
		if(!DetachOrder(order)){
    		ERROR(_moduleName, "�Ѷ����Ӷ�������ɾ��ʧ��! λ��order->orderNo[%d]",order->orderNo);
    		return FALSE;
    	}
    	StockStatDel(order, order->balance);
    	
    	#ifdef USE_PBU
			//pbuͳ��
			PBUOrderStatDel(stock, pbuData, order, order->balance);
			//ά��pbuί������
			UpdatePBUOrderSortList(stock, pbuNo, order);
		#endif
    	
    	TRACE(_moduleName, "���ᶩ���������orderNo=[%d]",order->orderNo);
	}
	
	PrintOrderCS(order);
	///////////////////////
	PrintStock(stock);
	///////////////////////
	return TRUE;
}
/*****************************************************************
** ������:      DeleteOrder
** �� ��:       WITHDRAW_CS *
** �� ��:       BOOL
** ��������:    ���ݼӹ�-ɾ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL 
DeleteOrder(WITHDRAW_CS * csWithdraw){
	#ifdef USE_PBU
		int			pbuNo = -1;
		PBU_HEAD*	pbuHead = NULL;
		PBU_DATA*	pbuData = NULL;
	#endif
	STOCK_CS	*stock                  = NULL;
	ORDER_CS    *order                  = NULL;
	
	//���ݶ�������Ų��Ҷ�Ӧ�Ķ���
	order = GetOrder(csWithdraw->orderProcessNo);
	stock = GetStock(csWithdraw->stockNo);
    if(stock == NULL){
		ERROR(_moduleName, "��ȡ��Ʊ��Ϣʧ��");
		return FALSE;
	}
	if(order == NULL){
		ERROR(_moduleName, "��ȡ������Ϣʧ��");
		return FALSE;
	}
    
    //����֤ȯ��Ϣ�еĳ�������
    UpdateStockSort(csWithdraw);
    
    //���³�������
    if(stock->categoryNo == CATEGORY_ID_ASHARE)
    {
    	if(IsBuyWithdraw(csWithdraw)){
    		UpdateWithdrawSortList(GetAShareWithdrawBuySortList()->withdrawNo,csWithdraw,order);
    	}
	 	else if(IsSellWithdraw(csWithdraw)){
	 		UpdateWithdrawSortList(GetAShareWithdrawSellSortList()->withdrawNo,csWithdraw,order);
	 	}
    }
    
    if(stock->categoryNo == CATEGORY_ID_BSHARE)
    {
    	if(IsBuyWithdraw(csWithdraw)){
    		UpdateWithdrawSortList(GetBShareWithdrawBuySortList()->withdrawNo,csWithdraw,order);
    	}
	 	else if(IsSellWithdraw(csWithdraw)){
	 		UpdateWithdrawSortList(GetBShareWithdrawSellSortList()->withdrawNo,csWithdraw,order);
	 	}
    }

	//��֤ȯ��Ϣ��ۼ�ί������Ϊ��ԭί������-�޸������������ӳ�����������������
	if(IsBuyWithdraw(csWithdraw)){
		stock->withdrawBuyNumber++;
		if(IsIceOrder(order)){
			TRACE(_moduleName, "��ʼ�����ɽ����������ԭ���볷������[%lld]",stock->withdrawBuyAmount);
			stock->withdrawBuyAmount += csWithdraw->remPeakQty;
			stock->withdrawBuyMoney += (stock->lotSize * csWithdraw->remPeakQty / TEN_THOUSAND * order->orderPrice);
			TRACE(_moduleName, "�����ɽ�����������������ӳ���ͳ�������������볷������[%lld]����ɽ������ʾ����[%lld]",stock->withdrawBuyAmount,csWithdraw->remPeakQty);
		}
		else{
			stock->withdrawBuyAmount += csWithdraw->qty;
			stock->withdrawBuyMoney += (stock->lotSize * csWithdraw->qty / TEN_THOUSAND * order->orderPrice);
		}
	}
	else if(IsSellWithdraw(csWithdraw)){
		stock->withdrawSellNumber++;
		if(IsIceOrder(order)){
			TRACE(_moduleName, "��ʼ�����ɽ����������ԭ������������[%lld]",stock->withdrawSellAmount);
			stock->withdrawSellAmount += csWithdraw->remPeakQty;
			stock->withdrawSellMoney += (stock->lotSize * csWithdraw->remPeakQty / TEN_THOUSAND * order->orderPrice);
			TRACE(_moduleName, "�����ɽ�����������������ӳ���ͳ����������������������[%lld]����ɽ������ʾ����[%lld]",stock->withdrawSellAmount,csWithdraw->remPeakQty);
		}
		else{
			stock->withdrawSellAmount += csWithdraw->qty;
			stock->withdrawSellMoney += (stock->lotSize * csWithdraw->qty / TEN_THOUSAND * order->orderPrice);
		}
	}
	
	//�Ӷ�������ɾ������
	if(!DetachOrder(order)){
    	ERROR(_moduleName, "�Ѷ����Ӷ�������ɾ��ʧ��! λ��order->orderNo[%d]",order->orderNo);
    	return FALSE;
    }
    //�ӽ���ʱ��ͳ���м�ȥ��������
    StockStatDel(order, order->balance);
	
	#ifdef USE_PBU
		pbuNo = GetPBUNoByFirm(order->pbuId);
		if(pbuNo < 0){
			ERROR(_moduleName, "����pbuί����Ϣʧ�ܣ�δ�ҵ���Ӧ��pbu����");
			return;
		}
		
		pbuHead = GetPBUTableByStockNo(stock->stockNo, pbuNo);
		if(pbuHead == NULL){
			ERROR(_moduleName, "����pbuί����Ϣʧ�ܣ�δ�ҵ���Ӧ��pbu������");
			return;
		}
		
		pbuData = GetPBUDataByIndex(pbuHead->pbuDataAdr);
		if(pbuData == NULL){
			ERROR(_moduleName, "����pbuί����Ϣʧ�ܣ�δ�ҵ���Ӧ��pbu����");
			return;
		}
		
		//pbuͳ��
		PBUOrderStatDel(stock, pbuData, order, order->balance);
		//ά��pbuί������
		UpdatePBUOrderSortList(stock, pbuNo, order);
	#endif
	
	//Ϊ�˷�ֹ���Ͷ�����ʣ������Ϊ0���Ӷ���������ɾ��������ԭ�ж�����ʣ��������������Ϊ0����Ϊ���ö���״̬Ϊ����״̬
    //order->balance = 0;
    order->status = ORDER_STATUS_WITHDRAWN;
	
	///////////////////////
	PrintStock(stock);
	///////////////////////
	return TRUE;
}

/*****************************************************************
** ������:      UpdateStockSort
** �� ��:       WITHDRAW_CS *, ORDER_CS *, STOCK_CS	*
** �� ��:       ��
** ��������:    ����Stock��������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void UpdateStockSort(WITHDRAW_CS * withdraw){
    int         lastWithdraw    = -1;
    STOCK_CS	*stock                  = NULL;
    
    stock = GetStock(withdraw->stockNo);
    if(stock == NULL){
		ERROR(_moduleName, "��ȡ��Ʊ��Ϣʧ��");
		return;
	}
    //����stock��withdraw�еĳ���ָ��
    if (stock->firstWithdraw < 0) {
        stock->firstWithdraw = stock->lastWithdraw = withdraw->withdrawProcessNo;
    } else {
        lastWithdraw = stock->lastWithdraw;
        stock->lastWithdraw = withdraw->withdrawProcessNo;
        GetWithdraw(lastWithdraw)->nextStock = withdraw->withdrawProcessNo;
    }
}

/*****************************************************************
** ������:      IsBuyWithdraw
** �� ��:       WITHDRAW_CS*
** �� ��:       BOOL
** ��������:    ����ָ����ί���Ƿ�������ί��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL 
IsBuyWithdraw(WITHDRAW_CS * csWithdraw) {
    return csWithdraw->ordrBuyCod == ORDER_TYPE_BUY;
}


/*****************************************************************
** ������:      IsSellWithdraw
** �� ��:       WITHDRAW_CS*
** �� ��:       BOOL
** ��������:    ����ָ����ί���Ƿ�������ί��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL 
IsSellWithdraw(WITHDRAW_CS * csWithdraw) {
    return csWithdraw->ordrBuyCod == ORDER_TYPE_SELL;
}

/*****************************************************************
** ������:      UpdateWithdrawSortList
** �� ��:
** �� ��:       ��
** ��������:    ���µ���ί������/������������
**              ������ί����������10��ί��/ί��������
** ȫ�ֱ���:    ��
** ����ģ��:    DeleteOrder
** �� ��:       Baoyi Li
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
UpdateWithdrawSortList(int *sortList, WITHDRAW_CS *withdraw, ORDER_CS *order) {
    int         i               = 0;
    int         begin           = -1;
    int         end             = -1;
    WITHDRAW_CS *tmpWithdraw    = NULL;
    ORDER_CS    *tmpOrder       = NULL;
    
    //���������ִ����������0����ô��ʶ����ĳ������ܲμӵ��ʳ�������
    if(order->orderQty - order->balance > 0){
    	return;
    }
	
    for (i = 0; i < WITHDRAW_SORT_LENGTH; i++) {
        if (sortList[i] < 0) {
            if (begin < 0) {
                sortList[i] = withdraw->withdrawProcessNo;
                return;
            } else {
                end = i;
                break;
            }
        }

        tmpWithdraw = GetWithdraw(sortList[i]);
        tmpOrder 	= GetOrder(tmpWithdraw->orderProcessNo);
		
        if (begin < 0 && order->orderQty > tmpOrder->orderQty) {
            begin = i;
        }

        if (withdraw->orderNo == tmpWithdraw->orderNo) {
            end = i;
            break;
        }
    }

    if (begin < 0) {
        return;
    }

    if (end < 0) {
        end = WITHDRAW_SORT_LENGTH - 1;
    }

    for (i = end; i > begin; i--) {
        sortList[i] = sortList[i - 1];
    }

    sortList[begin] = withdraw->withdrawProcessNo;
}


/*****************************************************************
** ������:      PrintWithdrawToFile
** �� ��:       
** �� ��:       ��
** ��������:    ��ӡ������Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:		Li Bo
** �� ��:		2006.11.1
** �汾:        0.1
****************************************************************/
BOOL
PrintWithdrawToFile(){
    char			buf[2048];
	int				fp = 0;
    char			filePath[256];
    char			stockType = 0;
    WITHDRAW_CS* 	withdraw = NULL;
    STOCK_CS*		stock = NULL;
    int				i = 0;

	if (!IsHomeValid()) {
        return FALSE;
    }
    
    sprintf(filePath, "%s/WithdrawSnapshot%s.txt", GetHome(), GetTime());
    
	if ((fp = open(filePath, O_WRONLY | O_CREAT | O_APPEND,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
        printf("ERROR: �����ļ�[%s]ʧ��! \n", filePath);
        return FALSE;
    }
    
    for (i = 0; i < GetWithdrawNumber(); i++) {
    	withdraw = GetWithdraw(i);
    	memset(buf, 0, 2048);
        stock = GetStock(withdraw->stockNo);
        if(stock == NULL){
        	continue;
        }
        if(IsAShare(stock)){
        	stockType = 'A';
        }
        else if(IsBShare(stock)){
        	stockType = 'B';
        }
        else{
        	continue;
        }
        sprintf(buf,"%12s|%15lld|%c|%15d|%c|%15lld|\n",
        	withdraw->isinCod,
        	withdraw->orderNo,
        	stockType,
        	withdraw->orderTime,
        	withdraw->ordrBuyCod,
        	withdraw->qty
        	);
        write(fp, buf, strlen(buf));
    }
    close(fp);
    return TRUE;
}



/*****************************************************************
** ������:      PrintWithdrawToFile
** �� ��:       �ļ�������
** �� ��:       ��
** ��������:    ��ӡ������Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.11.30
** �� ��:		
** �� ��:		
** �汾:        0.1
****************************************************************/
void
PrintWithdrawInfoToFile(int fp){
	char			buf[2048];
    WITHDRAW_CS* 	withdraw = NULL;
    int         	number  = 0;
    int         	i       = 0;

    number = GetWithdrawNumber();

    sprintf(buf, "#****������Ϣ****\n#��������: %d\n\n", number);
    write(fp, buf, strlen(buf));

    for (i = 0; i < number; i++) {
        withdraw = GetWithdraw(i);
		
		memset(buf, 0, 2048);

        sprintf(buf, "orderNo=[%16llu]--orderNoOld=[%16llu]--orderNoOld1=[%16llu]--orderProcessNo=[%9d]--orderProcessNoOld=[%9d]--orderProcessNoOld1=[%9d]--orderTime=[%9d]--isinCod=[%12s]--pbuId=[%8s]--ordrBuyCod=[%c]--ordrTypCod=[%c]--price=[%16lld]--qty=[%16lld]--trdResTypCod=[%3s]--remPeakQty=[%16lld]--orderStatus=[%c]--peakSizeQty=[%16lld]--trnTypId=[%c]--withdrawProcessNo=[%9d]--\n",
        	withdraw->orderNo,
        	withdraw->orderNoOld,
        	withdraw->orderNoOld1,
        	withdraw->orderProcessNo,
        	withdraw->orderProcessNoOld,
        	withdraw->orderProcessNoOld1,
        	withdraw->orderTime,
        	withdraw->isinCod,
        	withdraw->pbuId,
        	withdraw->ordrBuyCod,
        	withdraw->ordrTypCod,
        	withdraw->price,
        	withdraw->qty,
        	withdraw->trdResTypCod,
        	withdraw->remPeakQty,
        	withdraw->orderStatus,
        	withdraw->peakSizeQty,
        	withdraw->trnTypId,
        	withdraw->withdrawProcessNo);

        write(fp, buf, strlen(buf));
    }
}
