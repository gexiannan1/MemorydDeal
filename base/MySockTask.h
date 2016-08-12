#ifndef _MYSOCK_TASK_H_
#define _MYSOCK_TASK_H_
#include "MyQueue.h"
#include "MySocket.h"
#include "../MatchingOrder.h"

namespace MyNameSpace
{
	class MySockTask : public MyQueue
	{
		public:
			MySockTask(int sock, int id);
			virtual ~MySockTask() {}
			int getMsg();
			int rcvBuffer();
			int sendDataWithBuffer(const char *buf, uint32_t len);
			int addEpollEvent(int epfd, epoll_event & ev);
			int delEpollEvent(int epfd, epoll_event & ev);
			int syncSendBuf();
			int getId()
			{
				return mId;
			}
		private:
			MySocket mSock;
			int mId;
            Order doOrder;
	};
}

#endif
