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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
//STOCK_CS*   GetStockByIsinCod(char *isinCod);

int main()
{
  int     memId = 0;
 static void * _shareMemoryAddress = SHM_FAILED;
  if ((memId = shmget((key_t )0x0101675a, 0, 0666)) < 0) {
    printf("get  share memory id  failed \n");
    return 0;
  }
  if ((_shareMemoryAddress = shmat(memId, NULL, 0)) == SHM_FAILED) {
    printf("connect share memory failed \n");
    return 0;
  }
  system("ipcs -m");

  char *isincod = "ffffffffffff";
  printf("memId   %d _shareMemoryAddress %0x \n",   memId  , _shareMemoryAddress);
  STOCK_CS *stock = GetStockByIsinCod(isincod);
  if(!stock)
  {
    std::cout  << "error :not found this stock" << std::cout ;
  }
  ORDER_CS *order = NULL;
  order = GetOrder(stock->firstOrderSell);
 // order = 0x3e6e0000;
  if(!order)
  {
    std::cout  << "error :not found this order" << std::cout ;
  }
  if(order->priceLevel ==  -1 )
  {
    std::cout  << "not found buy order" << std::cout ;

  }
  int index =  order->nextStock;
  while(index >=  0)
  {
    order = GetOrder(index);
    if(order)
    {

      printf(" orderNo: %llu  orderPrice %ld orderQty %ld \n",order->orderNo,order->orderPrice , order->orderQty);
    }

    index = order->nextStock ;
  }
  return 0;
}


