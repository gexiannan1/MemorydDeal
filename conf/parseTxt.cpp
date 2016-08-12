#include <stdio.h>
#include <string.h>
#include "../mem/model/stock.h"

int main()
{
  int n;
  char str[256];
  int i = 1;
  STOCK_INFO stcokinfo ;
  memset(str,0x0,sizeof(str));
  FILE *fin = fopen("./stock.txt","r");
   while (fgets(str, 100, fin) != NULL ) {
             printf("%d %s\n", i++, str);
             bzero((char*) &stcokinfo,sizeof(STOCK_INFO));
             sscanf(str, "stockNo: %s\t", &stcokinfo.isinCod);
             //AddStock(&stcokinfo);
                 }
  return 0;

}

