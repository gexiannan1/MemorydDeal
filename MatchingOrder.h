#include "./mem/model/order.h"
#include "./mem/model/model_te.h"
#include "./mem/model/stock.h"
#include "./mem/model/price.h"
#include "./mem/model/model.h"
#include "./mem/memory.h"
#include <iostream>
#include "./base/MySingleton.h"

class Order
{
  public:
    Order();
    ~Order();
  public:
   void  MakeOrder(ORDER_CS* order);
    bool IsBuyOrder(ORDER_CS* order);
    bool IsSellOrder(ORDER_CS *order);
    void AddBidNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int nextBidIndex);
    int  AddNewBuyNode(STOCK_CS *stock, ORDER_CS *order);
    int  NextBid(); 
    void InitBid(BID_CS *target);
    void AddOfferNodeByOrder(STOCK_CS *stock, ORDER_CS *order, int nextOfferIndex);
    int AddNewOfferNode(STOCK_CS *stock, ORDER_CS *order);//相当于InsertBidNodeByOrder
    int  NextOffer();
   void InitOffer(OFFER_CS *target);
   std::string GetBuffToSend(ORDER_CS *);
   void DeleteFromSellQueue(ORDER_CS *order,long long amount);//UpdateBidByWithdraw(pOrderCS, pOrderCS->balance);
   int  GetSellOrderTotalNum(OFFER_CS*,char*);
   void  DeleteFromBuyQueue(ORDER_CS* order,long long amount);
   int GetBuyOrderTotalNum(BID_CS*, char*);
};

namespace MyNameSpace
{
  class MyShareMemoryData : public MySingleton<MyShareMemoryData>
  {
    private:
      friend class MySingleton<MyShareMemoryData>;
      MyShareMemoryData()
      {

      }
      ~MyShareMemoryData()
      {

      }
    public:
        void ReadOrderShareMemory();
  };
}

