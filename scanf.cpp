#include<stdio.h>
#include <string.h>
#define STOCK_ISIN_CODE_LENGTH 15
typedef struct {
  int    setId;   /* 证券所属set号 */
  char   isinCod[STOCK_ISIN_CODE_LENGTH]; /* 股票国际证券标识码 */
} STOCK_INFO;

bool stockmeminit()
{
  char str[256];
  int i = 1;
  STOCK_INFO stcokinfo ;
  memset(str,0x0,sizeof(str));
  FILE *fin = fopen("./conf/stock.txt","r");
  while (fgets(str, 100, fin) != NULL ) {
    printf("%d %s\n", i++, str);
    bzero((char*) &stcokinfo,sizeof(STOCK_INFO));
    sscanf(str, "%s", &stcokinfo.isinCod);
  }
return true;
}
int main()
{
  bool flag = stockmeminit();
  if(flag)
  {}
  return 0;
}
