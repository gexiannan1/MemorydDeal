#include "MyServer.h"
#include "./base/MySockTaskManager.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "./mem/env/env.h"
#include "./mem/memory.h"
#include "./mem/util/log/log_config.h"

#include "./mem/model/order.h"
#include "./mem/model/model_te.h"
#include "./mem/model/model.h"
#include "./mem/model/stock.h"
#include <sys/types.h>
#include <sys/shm.h>

#include <signal.h>
#include <unistd.h>

//static const char _moduleName[] = "heha ";
bool stockmeminit()
{
  int n;
  char str[256];
  int i = 1;
  STOCK_INFO stcokinfo ;
  memset(str,0x0,sizeof(str));
  FILE *fin = fopen("./conf/stock.txt","r");
  while (fgets(str, 100, fin) != NULL ) {
    printf("%d %s\n", i++, str);
    bzero((char*) &stcokinfo,sizeof(STOCK_INFO));
    sscanf(str, "%s", &stcokinfo.isinCod);
    int ret = AddStock(&stcokinfo);
    if(ret == -1 )
    {
      std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): stock init fail"<<std::endl;
      return false;
    }
  }
  return true;

}
void LookShareMemoryEveryTime(int sig)
{
   MyNameSpace::MyShareMemoryData::getInstance().ReadOrderShareMemory();
   alarm(2);
}

int main()
{
  //just for test
/*[share]
  mem.set_id_map.max=1000
  mem.set.max=10000
  mem.set_size.max=5000
  mem.category.max=100
  mem.stock.max=10000
  mem.order.max=35000000
  mem.trade.max=50000000
  mem.withdraw.max=10000000
  mem.bid.max=500000
  mem.offer.max=500000
  mem.industry.max=50
  mem.stock_index.max=1000*/
  MEM_CONFIG_ALL do_test ;
  MyNameSpace::MyServer ser;
 do_test.shareConfig.setIdMapMax =10;
 do_test.shareConfig.setMax = 100 ;
 do_test.shareConfig.setSizeMax = 50 ;
 do_test.shareConfig.categoryMax = 1;
 do_test.shareConfig.stockMax = 100 ;
 do_test.shareConfig.orderMax = 35000;

 do_test.shareConfig.tradeMax = 5000;
 do_test.shareConfig.withdrawMax = 1000 ;
 do_test.shareConfig.bidMax = 500;
 do_test.shareConfig.offerMax = 500;
 do_test.shareConfig.industryMax = 5 ;
 do_test.shareConfig.stockIndexMax = 10;
 InitShareMemory(&do_test);
 stockmeminit();
// ReadOrderShareMemory();
// PrintEvaluateMemAllocationStats(&PrintEvaluateMemAllocationStats);
//INFO(_moduleName, "获得共享内存Id失败! info   adkfdkdkaldjkafljlhdalkfdalog  %s\n", "hello world ");
 PrintMemoryStats() ;

 InitRootLogConfigExceptive("log.conf","log_root","log.main");

 if (!ser.init(50113))
  {
    std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): server init fail"<<std::endl;
    exit(1);
  }
signal(SIGALRM, LookShareMemoryEveryTime); 

  alarm(2);


  ser.mainLoop();

  //MyNameSpace::MySockTaskManager::getInstance().doProcessMsg();
  ser.fini();
  return 0;
}
