#ifndef _MY_SERVER_H
#define _MY_SERVER_H

#include "MyBaseServer.h"
#include "./base/MySockTaskPool.h"

namespace MyNameSpace
{
	class MyServer : public MyBaseServer
	{
		public:
		MyServer()  : mTaskPool(4, 32), mUniqueId(0)
			{

			}
			 virtual bool init(int port);
             virtual bool reload();
			 virtual bool newTask(int sock);
            void fini();
			void initCallBack();
		private:
			MySockTaskPool mTaskPool;
			uint32_t mUniqueId;
	};
}

#endif
