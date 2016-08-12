#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define NELEMS(arr) (sizeof(arr) / sizeof(arr[0]))
 //void *bsearch(const void *key, const void *base, size_t *nelem, size_t width, int(*fcmp)(const void *, const void*))
int numarray[] = {123, 145, 512, 627, 800, 933};

int numeric(const int *p1, const int *p2)
{
     return(*(int*)p1 - *(int*)p2);
}

int lookup(int key)
{
  int *itemptr;

  itemptr = (int *)bsearch(&key, numarray, NELEMS(numarray),
      sizeof(int), (int(*)(const void *,const void *))numeric);
  return (itemptr != NULL);
}

const std::string& GetBuffToSend()
{
 std::string RetStr;
 char str[] = "gexiannnan";
 return RetStr = str;
}

int main(void)
{
 const std::string& str =  GetBuffToSend();
  std::cout <<"str"<< str << std::endl;
  if (lookup(522))
    printf("512 is in the table  %d  .\n", lookup(522));
  else
    printf("512 isn't in the table.\n");
  return 0;
}



