#ifndef _MYSOCK_TASK_POOL_H
#define _MYSOCK_TASK_POOL_H

#include "MyThread.h"
#include "MySockTask.h"
namespace MyNameSpace
{
	class MyRecycleThread;
	class MySockTaskPool
	{
		public:
			MySockTaskPool(int ioCount, int maxConnPerIo);
			~MySockTaskPool();
			bool addTask(MySockTask * task);
			int getMaxCoonPerIo()
			{
				return mMaxConnPerIo;
			}
			void addRecycleThread(MySockTask *task);
			bool init();
			void fini();
		private:
			int mIoCount;
			int mMaxConnPerIo;
			MyThreadPool mIoThreadPool;
			MyRecycleThread *mRecycleThread;
	};
}
#endif
