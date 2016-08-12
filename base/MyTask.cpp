#include "MyTask.h"

namespace MyNameSpace
{
	bool MyTask::cmdParse(const char *msg, int len)
	{
		std::cout<<"len: "<<len<<"msg: "<<msg<<std::endl;
		return true;
	}
}
