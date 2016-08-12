#include "MatchingOrder.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>


Order::Order()
{

}

Order::~Order()
{
}

void Order::MakeOrder(ORDER_CS* order)
{

  if(order == NULL)
    return ;
  STOCK_CS*  stock =  GetStock(order->stockNo);
  if(stock == NULL )
    return ;
  if(IsBuyOrder(order))
  {
  int offerIndex =  -1;
    int SetBuyNumber = 0 ; //记录成交次数
    offerIndex = stock->lowestOffer;
    while(offerIndex > 0)
    {
      OFFER_CS *offer = GetOffer(offerIndex);
      if(offer)
      {
        if(order->orderPrice > offer->price  && offer->orderNumber != 0 )//订单数量 也就是orderNumber
        {
          // TODO 撮合
        int dealNum =  (order->orderQty  < offer->orderAmount) ? order->orderQty : offer->orderAmount  ;
        offer->orderAmount -= dealNum; 

        order->orderQty -= dealNum;//扣除数量
        order->balance -=  order->orderQty * order->orderPrice ;
          SetBuyNumber++;
          if(offer->orderAmount <= 0 )
          {
            DeleteFromSellQueue(order, order->balance);
          }
          if(order->balance <= 0   || order->orderQty )
          {
            order->status =ORDER_STATUS_CALL_PRICE_OVER;
            break;
          }
        }
      }
      offerIndex = offer->next;
    }
    if(SetBuyNumber == 0)
      {
        AddBidNodeByOrder(stock,order,stock->lowestOffer);//丢买方队列
      }

  }
 
  if(IsSellOrder(order))//mai
  {
    int bidIndex = -1;
    int SetSellNumber = 0;
    bidIndex = stock->highestBid;
    while(bidIndex    > 0 )
    {
      BID_CS*  bid = GetBid(bidIndex) ;
      if(bid)
      {
        if(order->orderPrice  < bid->price  && bid->orderNumber != 0 )//订单数量 也就是orderNumber
        {
          // TODO 撮合
          int dealNum =  (order->orderQty  < bid->orderAmount) ? order->orderQty : bid->orderAmount  ;
          bid->orderAmount -= dealNum; 

          order->orderQty -= dealNum;//扣除数量
          order->balance -=  order->orderQty * order->orderPrice ;
          SetSellNumber++;
          if(bid->orderAmount <= 0 )
          {
           // DeleteFromBuyQueue(order, order->balance);
          }
          if(order->balance <= 0   || order->orderQty )
          {
            order->status =ORDER_STATUS_CALL_PRICE_OVER;
            break;
          }
        }
      }
      if(SetSellNumber == 0)
      {
        AddBidNodeByOrder(stock,order,stock->lowestOffer);//丢卖方队列
      }
      bidIndex = bid->next;
    }

    }
}

bool Order::IsBuyOrder(ORDER_CS* order)
{
return order->orderBSType == 1;

 // return order->orderBSType == ORDER_TYPE_BUY;
}

bool Order::IsSellOrder(ORDER_CS *order)
{
  return order->orderBSType == 2;
}

void Order::AddBidNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int nextBidIndex)
{
  int     bidIndex    = -1;
  BID_CS  *newBid     = NULL;
  BID_CS  *nextBid    = NULL;
  BID_CS  *prevBid    = NULL;

  bidIndex = AddNewBuyNode(stock, order);

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
int  Order::AddNewBuyNode(STOCK_CS *stock, ORDER_CS *order)
{
  int     bidIndex    = -1;
  int     orderIndex  = -1;
  BID_CS  *bid        = NULL;

  orderIndex = order->orderProcessNo;
  bidIndex = NextBid();
  bid = GetBid(bidIndex);

  InitBid(bid);

  bid->price = order->orderPrice;
  /*if(IsTradingPhase(order)){
    bid->orderNumber = 1;
   if(IsIceOrder(order)){
      TRACE(_moduleName, "冰山订单(增加委买节点)，开始处理价格统计信息");
      bid->orderAmount = order->remPeakQty;
      TRACE(_moduleName, "冰山订单(增加委买节点)，处理价格统计信息结束 现委托数量[%lld],冰山订单揭示数量[%lld]",bid->orderAmount,order->remPeakQty);
    }
    else{
      bid->orderAmount = order->balance;
    }
  }*/

  bid->orderAmount = order->balance;
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

int Order::NextBid()
{
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
      //FATAL("Price.NextBid()", "委买价位数量超出预定义的最大委买价位数量！ Current Number: %d", GetShareMemCurrentStats()->nextBid);
      sleep(1);
    }
  }
  return -1;
}
 void Order::InitBid(BID_CS *target) {
    bzero(target, sizeof(BID_CS));
    target->firstOrder = target->lastOrder = \
            target->prev = target->next = NULL_INDEX;
}
 void Order::AddOfferNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int nextOfferIndex)
{
  int         offerIndex  = -1;
  OFFER_CS    *newOffer   = NULL;
  OFFER_CS    *nextOffer  = NULL;
  OFFER_CS    *prevOffer  = NULL;

  offerIndex = AddNewOfferNode(stock, order);

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
int Order::AddNewOfferNode(STOCK_CS *stock, ORDER_CS *order)
{
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
  offer->firstOrder = orderIndex;
  offer->lastOrder = orderIndex;

  order->priceLevel = offerIndex;
  order->prevPrice = order->nextPrice = NULL_INDEX;

  OfferStatAdd(offer, order);
  /*
   * 更新Stock信息
   */
  stock->offerNumber ++;

  return offerIndex;

}

int Order::NextOffer()
{
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
    //  FATAL("Price.NextBid()", "委买价位数量超出预定义的最大委买价位数量！ Current Number: %d", GetShareMemCurrentStats()->nextOffer);
      sleep(1);
    }
  }
  return -1;

}
 void Order::InitOffer(OFFER_CS *target)
{
   bzero(target, sizeof(OFFER_CS));
   target->firstOrder = target->lastOrder = \
   target->prev = target->next = NULL_INDEX;
}

std::string Order::GetBuffToSend(ORDER_CS *order)
{
  std::string RetStr;
  std::cout << "send the order response" << std::endl;
  int len = sizeof(ORDER_CS);
  char str[len + 1 ];
  memset(str,0x0,sizeof(str));
  memcpy(str,order,len);
  return RetStr=str;//测试了下 返回的是左值
}


void Order::DeleteFromSellQueue(ORDER_CS *order,long long amount)
{
  STOCK_CS    *stock      = NULL;
  OFFER_CS    *offer      = NULL;
  int               orderNum = 0;
  int         offerIndex  = -1;

  if (! order || ! (stock = GetStock(order->stockNo))) {
    return;
  }

  offerIndex = order->priceLevel;
  if (! (offer = GetOffer(offerIndex))) {
    return;
  }


  offer->orderAmount -= amount;
  offer->orderNumber --;

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

  orderNum = GetSellOrderTotalNum(offer, stock->prcsStsValCod);

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

int  Order::GetSellOrderTotalNum(OFFER_CS*,char*)
{
  //TODO
    int number = 1;
    return number;
}
void Order::DeleteFromBuyQueue(ORDER_CS* order,long long amount)
{
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
 // BidStatDel(bid, order, amount);

  //更新价位信息
  if(IsTradingPhase(order)){
    bid->orderAmount -= amount;
  }


  orderNum = GetBuyOrderTotalNum(bid, stock->prcsStsValCod);

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

int Order::GetBuyOrderTotalNum(BID_CS* ,char *)
{
  //TOFO
  int number = 1;
  return number;
}
