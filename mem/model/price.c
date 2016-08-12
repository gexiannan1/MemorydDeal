/******************************************************************
** �ļ���:      price.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.9.11
** �� ��:       ��λ��Ϣ����
** �޶���¼��
** �޶�����     �޶���   SIR    �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
** 2005.12.12   Song Fang       �޸�UpdateBidByWithdraw()��UpdateOfferByWithdraw()������
**                              ��Ӧ�ġ�������ܡ��е��������Ϊ[����������ϵͳ]->[32]��
** 2013.09.27   LIBO     00811  ����λ����ѭ��ʹ��ʱ�ж���������
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
 * �ڲ���������
 */
static int  NextBid();                              /* ������һ���õĿ���Bidλ�� */
static int  NextOffer();                            /* ������һ���õĿ���Offerλ�� */
static void InitBid(BID_CS*);                       /* ��ʼ��ָ��λ�õ�BID_CS */
static void InitOffer(OFFER_CS*);                   /* ��ʼ��ָ��λ�õ�OFFER_CS */

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
** ������:      GetBidList
** �� ��:       ��
** �� ��:       BID_CS[]
** ��������:    ����ί���λ�б����ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BID_CS*
GetBidList() {
    return (BID_CS*) GetShareMemory() + GetShareMemAllocationStats()->bidOffset;
}


/*****************************************************************
** ������:      GetOfferList
** �� ��:       ��
** �� ��:       OFFER_CS[]
** ��������:    ����ί����λ�б����ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
OFFER_CS*
GetOfferList() {
    return (OFFER_CS*) GetShareMemory() + GetShareMemAllocationStats()->offerOffset;
}


/*****************************************************************
** ������:      GetStockBidList
** �� ��:       stockNo - ��Ʊ�б���ں�
** �� ��:       BID_CS[]
** ��������:    ����ָ����Ʊ��ί���λ�б�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BID_CS*
GetStockBidList(int stockNo) {
    return (BID_CS*) GetShareMemory() + GetShareMemAllocationStats()->bidOffset
            + GetStock(stockNo)->highestBid;
}


/*****************************************************************
** ������:      GetStockOfferList
** �� ��:       stockNo - ��Ʊ�б���ں�
** �� ��:       OFFER_CS[]
** ��������:    ����ָ����Ʊ��ί����λ�б�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
OFFER_CS*
GetStockOfferList(int stockNo) {
    return (OFFER_CS*) GetShareMemory() + GetShareMemAllocationStats()->offerOffset
            + GetStock(stockNo)->lowestOffer;
}


/*****************************************************************
** ������:      GetBid
** �� ��:       ��
** �� ��:       BID_CS*
** ��������:    ����ָ��λ�õ�BID_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BID_CS*
GetBid(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->bidMax) {
        return NULL;
    }
    return GetBidList() + index;
}


/*****************************************************************
** ������:      GetOffer
** �� ��:       ��
** �� ��:       OFFER_CS*
** ��������:    ����ָ��λ�õ�OFFER_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
OFFER_CS*
GetOffer(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->offerMax) {
        return NULL;
    }
    return GetOfferList() + index;
}


/*****************************************************************
** ������:      GetBidNumber
** �� ��:       ��
** �� ��:       Bid�����������Ѷ����Ŀ���λ�ã�
** ��������:    ����Bid�����������Ѷ����Ŀ���λ�ã�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetBidNumber() {
    return GetShareMemCurrentStats()->nextBid;
}


/*****************************************************************
** ������:      GetOfferNumber
** �� ��:       ��
** �� ��:       Offer�����������Ѷ����Ŀ���λ�ã�
** ��������:    ����Offer�����������Ѷ����Ŀ���λ�ã�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetOfferNumber() {
    return GetShareMemCurrentStats()->nextOffer;
}


/*****************************************************************
** ������:      UpdateBidByOrder
** �� ��:       ��
** �� ��:       ��
** ��������:    ί�мӹ�����ʱί���λ����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:
** �� ��:
** �汾:        0.1
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
             * ���¼�λ
             */
         UpdateBidNodeByOrder(stock, order, bidIndex);
            return;
        }

        if (bid->price < order->orderPrice) {
            /*
             * �����¼�λ
             */
            InsertBidNodeByOrder(stock, order, bidIndex);
            return;
        }

        bidIndex = bid->next;
    }
    /*
     * �����¼�λ
     */
    InsertLastBidNodeByOrder(stock, order);
}


/*****************************************************************
** ������:      UpdateOfferByOrder
** �� ��:       ��
** �� ��:       ��
** ��������:    ί�мӹ�����ʱί����λ����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:
** �� ��:
** �汾:        0.1
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
             * ���¼�λ
             */
            UpdateOfferNodeByOrder(stock, order, offerIndex);
            return;
        }

        if (offer->price > order->orderPrice) {
            /*
             * �����¼�λ
             */
            InsertOfferNodeByOrder(stock, order, offerIndex);
            return;
        }

        offerIndex = offer->next;
    }

    /*
     * �����¼�λ
     */
    InsertLastOfferNodeByOrder(stock, order);
}


/*****************************************************************
** ������:      UpdateBidByWithdraw
** �� ��:       ��
** �� ��:       ��
** ��������:    �ɽ��������ӹ�����ʱ��ί���λ����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:		Li Bo
** �� ��:		2006.9.27
** �汾:        0.1
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
	//���½���ʱ��ͳ��
	BidStatDel(bid, order, amount);
	
    //���¼�λ��Ϣ
    if(IsTradingPhase(order)){
    	bid->orderAmount -= amount;
    }

    //�������ʣ������Ϊ0�������Ǳ�ɽ������ʾ����Ϊ0���Ӽ�λ������ɾ��
 /*   if ((order->balance - amount == 0) || (IsIceOrder(order) && (order->remPeakQty - amount == 0))){
        if(IsTradingPhase(order)){
	    	bid->orderNumber --;
	    }

        //����ͬ��λί���б�
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
	
    //ɾ����λ�ڵ�
    if (orderNum == 0) {
        stock->bidNumber --;

        //���¼�λ�б�
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

        //���ü�λ��Ϣ
        InitBid(bid);
    }
}


/*****************************************************************
** ������:      UpdateOfferByWithdraw
** �� ��:       ��
** �� ��:       ��
** ��������:    �ɽ��������ӹ�����ʱ��ί����λ����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:
** �� ��:
** �汾:        0.1
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
	
    //���¼�λ��Ϣ
    if(IsTradingPhase(order)){
    	offer->orderAmount -= amount;
    }
    if ((order->balance - amount == 0) || (IsIceOrder(order) && (order->remPeakQty - amount == 0))) {
        if(IsTradingPhase(order)){
	    	offer->orderNumber --;
	    }

        //����ͬ��λί���б�
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
	
    //ɾ����λ�ڵ�
    if (orderNum == 0) {
        stock->offerNumber --;
        
        //���¼�λ�б�
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
        
        //���ü�λ��Ϣ
        InitOffer(offer);
    }
}


/*****************************************************************
** ������:      IsHaveOpponent
** �� ��:       order
** �� ��:       TRUE:���� FALSE:δ���� 
** ��������:    �ж϶����Ƿ񵹹�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.26
** �� ��:
** �� ��:
** �汾:        0.1
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

	//�ж�֤ȯ�Ľ���ʱ���Ƿ���Ҫ��⵹��
	
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
	
	//ERROR("�۸񵹹�","��ǰ����ʱ��[%s],��ƱISINCode=[%s],orderNo=[%lld]--orderPrice=[%lld]--trdResTypCod=[%s]--orderBSType=[%c],���ֶ���orderNo=[%lld]--orderPrice=[%lld]--trdResTypCod=[%s]--orderBSType=[%c]--balance=[%lld]",\
		stock->prcsStsValCod,stock->isinCod, order->orderNo, order->orderPrice, order->trdResTypCod, order->orderBSType, pOrder->orderNo, pOrder->orderPrice, pOrder->trdResTypCod, pOrder->orderBSType, pOrder->balance \
		);
	return TRUE;
}


/*****************************************************************
** ������:      BidStatAdd
** �� ��:       bid:Ҫ���µ�bid�ڵ㣬order:Ҫ���µĶ�����Ϣ
** �� ��:       ��
** ��������:    ������Ϣ�����ӽ���ʱ��ͳ����Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.27
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
BidStatAdd(BID_CS* bid, ORDER_CS* order){
	int i = 0;
	
	if(bid == NULL || order == NULL){
		return;
	}
	
	//ȷ������ʱ�α��
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
		//��������ʱ�β���Ҫͳ��
		return;
	}
	
	//�Ѷ�����Ϣ��ӵ�����ʱ��ͳ����Ϣ
	bid->tradePhaseOrderNum[i]++;
	bid->tradePhaseOrderAmount[i] += order->balance;
}


/*****************************************************************
** ������:      BidStatAdd
** �� ��:       offer:Ҫ���µ�bid�ڵ㣬order:Ҫ���µĶ�����Ϣ
** �� ��:       ��
** ��������:    ������Ϣ�����ӽ���ʱ��ͳ����Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.27
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
OfferStatAdd(OFFER_CS* offer, ORDER_CS* order){
	int i = 0;
	
	if(offer == NULL || order == NULL){
		return;
	}
	
	//ȷ������ʱ�α��
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
	
	//�Ѷ�����Ϣ��ӵ�����ʱ��ͳ����Ϣ
	offer->tradePhaseOrderNum[i]++;
	offer->tradePhaseOrderAmount[i] += order->balance;
}


/*****************************************************************
** ������:      BidStatDel
** �� ��:       bid:Ҫ���µ�bid�ڵ㣬order:Ҫ���µĶ�����Ϣ, amount��Ҫ���µ�����
** �� ��:       ��
** ��������:    ������Ϣ�м��ٽ���ʱ��ͳ����Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.27
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
BidStatDel(BID_CS* bid, ORDER_CS* order, long long amount){
	int i = 0;
	
	if(bid == NULL || order == NULL || amount < 0){
		return;
	}
	
	//ȷ������ʱ�α��
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
		//��������ʱ�β���Ҫͳ��
		return;
	}
	
	//�Ѷ�����Ϣ��ӵ�����ʱ��ͳ����Ϣ
	bid->tradePhaseOrderAmount[i] -= amount;
	if(order->balance - amount == 0){
		bid->tradePhaseOrderNum[i]--;
	}
}


/*****************************************************************
** ������:      OfferStatDel
** �� ��:       offer:Ҫ���µ� offer �ڵ㣬order:Ҫ���µĶ�����Ϣ, amount��Ҫ���µ�����
** �� ��:       ��
** ��������:    ������Ϣ�м��ٽ���ʱ��ͳ����Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.27
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void 
OfferStatDel(OFFER_CS* offer, ORDER_CS* order, long long amount){
	int i = 0;
	
	if(offer == NULL || order == NULL || amount < 0){
		return;
	}
	
	//ȷ������ʱ�α��
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
	
	//�Ѷ�����Ϣ��ӵ�����ʱ��ͳ����Ϣ
	offer->tradePhaseOrderAmount[i] -= amount;
	if(order->balance - amount == 0){
		offer->tradePhaseOrderNum[i]--;
	}
}


/*****************************************************************
** ������:      UpdateBidNodeByOrder
** �� ��:       ��
** �� ��:       ��
** ��������:    ����ί���λ�ڵ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
UpdateBidNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int bidIndex) {
    BID_CS      *bid        = NULL;
    ORDER_CS	*pOrder		= NULL;
    int         lastOrder   = -1;

    bid = GetBid(bidIndex);
	
	order->priceLevel = bidIndex;

	//Ҫ����Ķ������Ǳ��ۣ����߼۸�ʱ������
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
** ������:      InsertBidNodeByOrder
** �� ��:       ��
** �� ��:       ��
** ��������:    ������ί���λ�ڵ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:
** �� ��:
** �汾:        0.1
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
         * �����λ�б�ͷ�ڵ�
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
** ������:      InsertLastBidNodeByOrder
** �� ��:       ��
** �� ��:       ��
** ��������:    ����ĩί���λ�ڵ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      AddNewBidNode
** �� ��:       ��
** �� ��:       new Bid index
** ��������:    ����ί���λ�ڵ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:
** �� ��:
** �汾:        0.1
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
	    	TRACE(_moduleName, "��ɽ����(����ί��ڵ�)����ʼ����۸�ͳ����Ϣ");
	    	bid->orderAmount = order->remPeakQty;
	    	TRACE(_moduleName, "��ɽ����(����ί��ڵ�)������۸�ͳ����Ϣ���� ��ί������[%lld],��ɽ������ʾ����[%lld]",bid->orderAmount,order->remPeakQty);
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
     * ����Stock��Ϣ
     */
    stock->bidNumber ++;

    return bidIndex;
}


/*****************************************************************
** ������:      UpdateOfferNodeByOrder
** �� ��:       ��
** �� ��:       ��
** ��������:    ����ί����λ�ڵ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
UpdateOfferNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int offerIndex) {
    OFFER_CS    *offer      = NULL;
    ORDER_CS	*pOrder		= NULL;
    int         lastOrder   = -1;

    offer = GetOffer(offerIndex);

    order->priceLevel = offerIndex;

	//Ҫ����Ķ������Ǳ��ۣ����߼۸�ʱ������
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
** ������:      InsertOfferNodeByOrder
** �� ��:       ��
** �� ��:       ��
** ��������:    ������ί����λ�ڵ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:
** �� ��:
** �汾:        0.1
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
         * �����λ�б�ͷ�ڵ�
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
** ������:      InsertLastOfferNodeByOrder
** �� ��:       ��
** �� ��:       ��
** ��������:    ����ĩί����λ�ڵ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      AddNewOfferNode
** �� ��:       ��
** �� ��:       new Offer index
** ��������:    ����ί����λ�ڵ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.5
** �� ��:
** �� ��:
** �汾:        0.1
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
	    	TRACE(_moduleName, "��ɽ����(����ί���ڵ�)����ʼ����۸�ͳ����Ϣ");
	    	offer->orderAmount = order->remPeakQty;
	    	TRACE(_moduleName, "��ɽ����(����ί���ڵ�)������۸�ͳ����Ϣ���� ��ί������[%lld],��ɽ������ʾ����[%lld]",offer->orderAmount,order->remPeakQty);
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
     * ����Stock��Ϣ
     */
    stock->offerNumber ++;

    return offerIndex;
}


/*****************************************************************
** ������:      NextBid
** �� ��:       ��
** �� ��:       ��һ���õĿ���Bid��λ��
** ��������:    ������һ���õĿ���Bidλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static int
NextBid() {
  /* �޸�Ϊ�ǵݹ��㷨 */
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
			FATAL("Price.NextBid()", "ί���λ��������Ԥ��������ί���λ������ Current Number: %d", GetShareMemCurrentStats()->nextBid);
			sleep(1);
		}
	}
	return -1;
}


/*****************************************************************
** ������:      NextOffer
** �� ��:       ��
** �� ��:       ��һ���õĿ���Offer��λ��
** ��������:    ������һ���õĿ���Offerλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static int
NextOffer() {
  /* �޸�Ϊ�ǵݹ��㷨 */
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
			FATAL("Price.NextBid()", "ί���λ��������Ԥ��������ί���λ������ Current Number: %d", GetShareMemCurrentStats()->nextOffer);
			sleep(1);
		}
	}
	return -1;
}


/*****************************************************************
** ������:      InitBid
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ʼ��ָ��λ�õ�BID_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
InitBid(BID_CS *target) {
    bzero(target, sizeof(BID_CS));
    target->firstOrder = target->lastOrder = \
            target->prev = target->next = NULL_INDEX;
}


/*****************************************************************
** ������:      InitOffer
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ʼ��ָ��λ�õ�OFFER_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
InitOffer(OFFER_CS *target) {
    bzero(target, sizeof(OFFER_CS));
    target->firstOrder = target->lastOrder = \
            target->prev = target->next = NULL_INDEX;
}


/*****************************************************************
** ������:      GetBidOrderTotalNum
** �� ��:       bid
** �� ��:       ��
** ��������:    ���ָ���򷽼۸�ڵ��ί���ܱ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.10.17
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      GetOfferOrderTotalNum
** �� ��:       offer
** �� ��:       ��
** ��������:    ���ָ�������۸�ڵ��ί���ܱ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.10.17
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      PrintBidInfoToFile
** �� ��:       fp �ļ�������
** �� ��:       ��
** ��������:    ��ӡί���λ��Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       weihy
** �� ��:       2005.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintBidInfoToFile(int fp) {
    BID_CS      *priceLevel = NULL;
    char        buf[1024];
    int         number = 0;
    int         i = 0;

    number = GetBidNumber();

    sprintf(buf, "#****ί���λ��Ϣ****\n#ί���λ��: %d\n\n", number);
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
** ������:      PrintBidInfoToFile
** �� ��:       fp �ļ�������
** �� ��:       ��
** ��������:    ��ӡί����λ��Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       weihy
** �� ��:       2005.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintOfferInfoToFile(int fp) {
    OFFER_CS    *priceLevel = NULL;
    char        buf[1024];
    int         number = 0;
    int         i = 0;

    number = GetOfferNumber();

    sprintf(buf, "#****ί����λ��Ϣ****\n#ί����λ��: %d\n\n", number);
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
** ������:      PrintStockSnapshotToFile
** �� ��:       ��
** �� ��:       TRUE FALSE
** ��������:    ��ӡ֤ȯ������Ϣ���ļ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.10.13
** �� ��:
** �� ��:
** �汾:        0.1
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
        printf("ERROR: �����ļ�[%s]ʧ��! \n", filePath);
        return FALSE;
    }
    
    for(i = 0; i < GetStockNumber(); i++){
    	stock = GetStock(i);
    	bid = GetBid(stock->highestBid);
    	while(bid != NULL){
    		memset(buf, 0, 512);
    		sprintf(buf, "֤ȯ����[%s]�������[%lld]��������������\n", stock->stockId, bid->price);
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
    		sprintf(buf, "֤ȯ����[%s]��������[%lld]��������������\n", stock->stockId, offer->price);
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
