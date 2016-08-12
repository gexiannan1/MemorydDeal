#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "./env/env.h"
#include "./util/log/log_config.h"
static const char _moduleName[] = "heha ";
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
  InitRootLogConfigExceptive("log.conf","log_root","log.main");
 MEM_CONFIG_ALL do_test ;
 do_test.shareConfig.setIdMapMax =1000;
 do_test.shareConfig.setMax = 10000 ;
 do_test.shareConfig.setSizeMax = 5000 ;
 do_test.shareConfig.categoryMax = 100;
 do_test.shareConfig.stockMax = 10000 ;
 do_test.shareConfig.orderMax = 35000000;
 do_test.shareConfig.tradeMax = 50000000 ;
 do_test.shareConfig.withdrawMax = 10000000 ;
 do_test.shareConfig.bidMax = 500000 ;
 do_test.shareConfig.offerMax = 500000;
 do_test.shareConfig.industryMax = 50 ;
 do_test.shareConfig.stockIndexMax = 1000 ;
InitShareMemory(&do_test);
INFO(_moduleName, "获得共享内存Id失败! info   adkfdkdkaldjkafljlhdalkfdalog  %s\n", "hello world ");

  return 0;
}
