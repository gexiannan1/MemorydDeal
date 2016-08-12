#ifndef __MY_CLIENT_TASK_MANAGER_H_
#define __MY_CLIENT_TASK_MANAGER_H_

#include "../base/MySingleton.h"
#include "../base/MyLock.h"
#include "MySockClientTask.h"
#include <map>
#include <vector>
#include "../mem/model/order.h"

namespace MyNameSpace
{
  class MyClientTaskManager : public MySingleton<MyClientTaskManager>
  {
    private:
      friend class MySingleton<MyClientTaskManager>;
      MyClientTaskManager(bool sendflag = false) : m_sendflag(sendflag)
      {

      }
      ~MyClientTaskManager()
      {

      }
    public:
      void addTask(MySockClientTask *task);
      void removeTask(MySockClientTask *task);
      bool doProcessMsg();	
      void testSend();
      MySockClientTask *getTaskById(int);
    public:
      bool m_sendflag;
    private:
      typedef std::map<int, MySockClientTask*> Container;
      typedef std::map<int, MySockClientTask*>::iterator Container_IT;
      Container mTasks;
      MyLock mLock;
  };
  class MyOrderData : public MySingleton<MyOrderData>
  {
    private:
      friend class MySingleton<MyOrderData>;
      MyOrderData()
      {

      }
      ~MyOrderData()
      {

      }
    public:
      bool initXml();
    public:
      typedef std::vector<ORDER_CS *> ORDER;
      typedef std::vector<ORDER_CS *>::iterator ORDER_IT;
      static ORDER  _v;
  };

}
#endif
