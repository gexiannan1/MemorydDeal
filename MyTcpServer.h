#ifndef _MY_TCP_SERVER_H_
#define  _MY_TCP_SERVER_H_

#include <sys/socket.h>

namespace MyNameSpace
{
	class MyTcpServer
	{
		public:
			MyTcpServer() : mEpfd(-1), mSock(-1), mListenCount(1024)
			{
			}
			~MyTcpServer();
			bool bindPort(int port);
			int acceptCallBack();
			bool setNonBlock(bool b);
		private:
			int mEpfd;
			int mSock;
			int mListenCount;
	};
}

#endif
