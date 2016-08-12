#include "MyClient.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<libxml/xmlmemory.h>
#include<libxml/parser.h>
#include <iostream>
#include "zXMLParser.h"
#include<libxml/tree.h>
#include <iostream>
#include <vector>
#include "../mem/model/order.h"
#include "MyClientTaskManager.h"
using namespace std;

int main()
{
  MyNameSpace::MyOrderData::getInstance().initXml();
  MyNameSpace::MyClient client;
  std::multimap<int, int> ipPortPair;
  ipPortPair.insert(std::pair<int, int>(INADDR_ANY, 50003));
  if (!client.init(ipPortPair))
  {
    std::cerr<<__FUNCTION__<<"("<<__LINE__<<"): client init fail"<<std::endl;
    exit(1);
  }
 client.mainLoop();
}
