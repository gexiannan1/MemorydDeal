#ifndef __MY_TCP_CLIENT_H_
#define __MY_TCP_CLIENT_H_

namespace MyNameSpace
{
	class MyTcpClient
	{
		public:
			MyTcpClient(int ip, int port, int reconn) : 
				mIp(ip), mPort(port), mSock(-1), mReconn(reconn)
			{
			}
			~MyTcpClient();
			int conn();
			int close();
		private:
			int mIp;
			int mPort;
			int mSock;
			bool mReconn;
	};
}

#endif
