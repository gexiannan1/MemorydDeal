#ifndef _MY_TASK_H_
#define _MY_TASK_H_
#include "MySockTask.h"

namespace MyNameSpace
{
	class MyTask : public MySockTask
	{
		public:
			virtual bool cmdParse(const char *msg, int len);
			MyTask(int sock, int id) : MySockTask(sock, id)
			{

			}
	};
}
#endif
