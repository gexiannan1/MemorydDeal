#ifndef _MY_BASE_SERVER_H_
#define  _MY_BASE_SERVER_H_

#include "MyTcpServer.h"
#include <vector>
#include "./base/CmdDispatcher.h"
namespace MyNameSpace
{
	class MyBaseServer
	{
		public:
			MyBaseServer(): mComplete(false), mInnerDispatcher("InnerDispatcher"), mOutterDispatcher("OutterDispatcher")
			{
			}
		bool reload();	//for hup signal
		 bool init(int port);
         virtual ~MyBaseServer(){}
	virtual bool newTask(int sock) = 0;
	void fini()
		{
			mComplete = true;
		}
		void mainLoop();
		protected:
//void regInnerCallBack(uint32_t cmdId, CallBackFunT fun);	//内部消息分发器，也就是服务器之间的消息
//void regOutterCallBack(uint32_t cmdId, CallBackFunT fun); //外部消息分发器，也就是服务端客户端之间的消息
		private:
int serverProcess();
	bool isFini()
	{
      return mComplete;
	}
		public:
			typedef std::vector<MyBaseServer *> Container;
			typedef std::vector<MyBaseServer *>::iterator Container_IT;
			static Container mServerContainer;
		private:
	bool mComplete;
	MyTcpServer mTcpServer;
		protected:
	Dispatcher mInnerDispatcher;
    Dispatcher mOutterDispatcher;
	};
}
#endif
