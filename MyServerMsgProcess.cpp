#include "MyServerMsgProcess.h"
#include "./base/MySockTaskManager.h"

namespace MyNameSpace
{
	void MyServerMsgProcess::run()
	{
		while (!isFini())
		{
			MySockTaskManager::getInstance().doProcessMsg();
			usleep(2*1000*1000);	
		}
	}
}
