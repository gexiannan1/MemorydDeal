#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<libxml/xmlmemory.h>
#include<libxml/parser.h>
#include <iostream>
#include<libxml/tree.h>
#include <iostream>
#include "zXMLParser.h"
#include <vector>
using namespace std;
#define  STOCK_ISIN_CODE_LENGTH 13
typedef struct {
  unsigned long long  orderNo;                                    /* 订单号 */
  unsigned long long  orderNoOld;  /* 旧订单号 */
    unsigned long long   orderNoOld1;   /* 旧订单号 */
    int  orderProcessNo;   /* 订单序列号 */
    int   orderProcessNoOld;  /* 旧订单序列号 */
    int    orderProcessNoOld1;   /* 旧订单序列号 */
    int               orderTime;                                  /* 订单接收时间 */
  char   isinCod[STOCK_ISIN_CODE_LENGTH];   /* 股票国际证券标识码*/
 //   char       pbuId[PBU_FIRM_LENGTH];             /* pbu号 */
    char       orderBSType;                              /* 买卖标志 - BUY 或 SELL */
    char      orderType;                        /* 订单类型 */
    long long     orderPrice;                              /* 价格 */
    long long     orderQty;                                /* 数量 */
    char   trdResTypCod[3];  /* 交易限制类型 */
    char    trnTypId; /* 事务类型 */
    char   orderStatus;  /* 订单状态 */
    long long    remPeakQty;  /* 剩余揭示数量 */
    long long   peakSizeQty;   /* 最大揭示数量 */
    char  ordrResCod;   /* 订单限制类型 */

    long long balance;                                    /* 余额（扣除已成交和已撤单后的剩余未处理数量，最终应为0） */
  char    status;                                     /* 状态 - OPEN, MATCHED; OPEN 的才参与撮合 */

  int   stockNo;                              /* 股票记录号，stock表入口 */
    int      nextStock;                                  /* 同股票下一笔委托 */
  int      priceLevel;                                 /* 该委托价位在价位列表中的位置 */
  int       prevPrice;                                  /* 同价位上一笔委托 */
  int        nextPrice;                                  /* 同价位下一笔委托 */
} ORDER_CS;

int main()
{
  std::cout <<"gexiannan" << std::endl;
  zXMLParser xml;
  std::vector<ORDER_CS *> _v;
  if (!xml.initFile("../conf/order.xml"))
  {
    std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): open order xml error\n "<<std::endl;
    return false;
  }
  xmlNodePtr root = xml.getRootNode("order");
  if(!root) return false;
  xmlNodePtr itemNode = xml.getChildNode(root,"orderrow");
  if(!itemNode) return false;

  while(itemNode)
  {
    ORDER_CS *order= new ORDER_CS();
    if(!order)
    {
      //malloc order error
      return false;
    }
    xml.getNodePropNum(itemNode,"orderNo",&order->orderNo,sizeof(order->orderNo));
    xml.getNodePropNum(itemNode,"orderBSType",&order->orderBSType,sizeof(order->orderBSType));
    xml.getNodePropNum(itemNode,"orderPrice",&order->orderPrice,sizeof(order->orderPrice));
    xml.getNodePropNum(itemNode,"isinCod",&order->isinCod,sizeof(order->isinCod));

    xml.getNodePropNum(itemNode,"orderQty",&order->orderQty,sizeof(order->orderQty));
    _v.push_back(order);
    itemNode = xml.getNextNode(itemNode,"orderrow");
  }

}
