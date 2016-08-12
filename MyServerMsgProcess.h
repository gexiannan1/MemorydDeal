#ifndef _MY_SERVER_MSG_PROCESS_H_
#define _MY_SERVER_MSG_PROCESS_H_

#include "./base/MySingleton.h"
#include "./base/MyThread.h"

namespace MyNameSpace
{
	class MyServer;
	class MyServerMsgProcess : public MyThread, public MySingleton<MyServerMsgProcess>
	{
		private:
			friend class MySingleton<MyServerMsgProcess>;
			MyServerMsgProcess()
			{

			}
			~MyServerMsgProcess()
			{

			}
		public:
			void run();
	};
}

#endif
