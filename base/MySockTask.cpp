#include "MySockTask.h"
#include "MySocket.h"
#include <sys/epoll.h>
#include <arpa/inet.h>
#include "../mem/model/order.h"
#include "../mem/model/model_te.h"
#include "../mem/model/model.h"

namespace MyNameSpace
{

  MySockTask::MySockTask(int sock, int id): mSock(sock), mId(id)
  {
  }

    int MySockTask::getMsg()
    {

      int msgCount = 0;
      while(true)
      {
        int len = 0;
        std::vector<char> msg;
        len = mSock.readBuffer(msg);
        if (len <= 0)
        {
          if (len < 0)
          {
            msgCount = -1;
          }
          break;
        }
        ++msgCount;
        std::string str(&msg[0], len);
        //     std::cerr<<__FUNCTION__<<"("<<__LINE__<<") size :"<<len<<"str :"<<str<<std::endl
        ORDER_CS *pOrder = new ORDER_CS();

        memcpy(pOrder,str.c_str(),sizeof(ORDER_CS));
   //     printf("another client orderNo %llu     orderBSType %c orderPrice%lld   isinCod %s    \n",     pOrder->orderNo  ,pOrder->orderBSType, pOrder->orderPrice,pOrder->isinCod );

        PushOrder(pOrder);
        //   AttachOrder(pOrder);
        if(IsHaveOpponent(pOrder))
        {
        doOrder.MakeOrder(pOrder);//TODO 撮合交易 生成订单 
          printf("orderNo %llu   orderPrice%lld   isinCod %s  orderBSType %d   orderQty %d  \n",     pOrder->orderNo  , pOrder->orderPrice,pOrder->isinCod ,pOrder->orderBSType  , pOrder->orderQty);

          int len = sizeof(ORDER_CS);
          char strsend[len + 1];
          memset(strsend,0x0,sizeof(strsend));
          memcpy(strsend, pOrder ,len);

         this->sendDataWithBuffer(strsend,sizeof(ORDER_CS));
        }
        else
        {
          //TODO 不能撮合成交 发回应 丢价位对列
          printf("push order into price queue \n");
          if(pOrder->orderBSType == 1) 
          {
            UpdateBidByOrder(pOrder);
          }
          if(pOrder->orderBSType == 2)
          {
            UpdateOfferByOrder(pOrder);
          }
        }

        pushMsg(len, &msg[0]);
        delete pOrder;
      }
      return msgCount;
    }

    int MySockTask::rcvBuffer()
    {
      return mSock.rcvBuffer();
    }
    int MySockTask::addEpollEvent(int epfd, epoll_event & ev)
    {
      return mSock.addEpollEvent(epfd, ev);
    }
    int MySockTask::delEpollEvent(int epfd, epoll_event & ev)
    {
      return mSock.delEpollEvent(epfd, ev);
    }

    int MySockTask::syncSendBuf()
    {
      return mSock.syncSendBuf();
    }

    int MySockTask::sendDataWithBuffer(const char *buf, uint32_t len)
    {
      return mSock.sendDataWithBuffer(buf, len);
    }
}
