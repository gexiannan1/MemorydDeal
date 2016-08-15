#include "MyClientTaskManager.h"
#include <string.h>
#include "../mem/model/model_te.h"
#include "../mem/model/order.h"
#include "../mem/model/model_te.h"
#include "../mem/model/model.h"
#include<stdio.h>
#include<stdlib.h>
#include<libxml/xmlmemory.h>
#include<libxml/parser.h>
#include <iostream>
#include "zXMLParser.h"
#include<libxml/tree.h>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/shm.h>
 #include <sys/ipc.h>
#include "../mem/shm_define.h"
using namespace std;

namespace MyNameSpace
{
    MyOrderData::ORDER MyOrderData::_v;
    void MyClientTaskManager::addTask(MySockClientTask *task)
    {
      MyScopeLock lock(mLock);
      mTasks[task->getId()] = task;
    }

    void MyClientTaskManager::removeTask(MySockClientTask *task)
    {
      MyScopeLock lock(mLock);
      mTasks.erase(task->getId());
    }

    bool MyClientTaskManager::doProcessMsg()
    {
      MyScopeLock lock(mLock);
      for (Container_IT it = mTasks.begin(); it != mTasks.end(); ++it)
      {
        it->second->processMsg();
      }
      return true;
    }

    MySockClientTask * MyClientTaskManager::getTaskById(int id)
    {
      Container_IT it;
      it = mTasks.find(id);
      if (mTasks.end() == it)
      {
        return NULL;
      }
      return it->second;
    }

    void MyClientTaskManager::testSend()
    {
      printf("the  size of total number : %lu\n", MyOrderData::_v.size());
     if(!m_sendflag)
      {
        size_t count  = 0 ;
        for( MyOrderData::ORDER_IT iter = MyOrderData::_v.begin() ; iter != MyOrderData::_v.end() ;++iter   )
        {
          char *tmp = new char[sizeof(ORDER_CS)];
          memcpy(tmp , (*iter) ,sizeof(ORDER_CS) );
          MyScopeLock lock(mLock);
         for (Container_IT it = mTasks.begin(); it != mTasks.end(); ++it)
          {
           if (it->second->isConn())
            {
              it->second->sendDataWithBuffer(tmp, sizeof(ORDER_CS));
              printf("---------------------------------------------提交订单  订单号:%llu   合约名:%s ------------------------------------------\n ",(*iter)->orderNo ,(*iter)->isinCod);
              count++;
              break;
            }
          }
          delete tmp;
        }
        if(count == (MyOrderData::_v.size()) )
        {
          m_sendflag = true;
        }
      }
      usleep(2 *1000* 1000);
    }

    bool MyOrderData::initXml()
    {
      zXMLParser xml;
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
        xml.getNodePropStr(itemNode,"isinCod",&order->isinCod,sizeof(order->isinCod));

        xml.getNodePropNum(itemNode,"orderQty",&order->orderQty,sizeof(order->orderQty));
        _v.push_back(order);
        itemNode = xml.getNextNode(itemNode,"orderrow");
      }
      return true;


    }

