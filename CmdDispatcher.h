
#ifndef __CMDDISPATCHER_H_
#define __CMDDISPATCHER_H_

#include <functional>
#include <memory>
#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
//#include "OrderMessage.h"

namespace MyNameSpace
{
  using CallBackFunT = std::function< bool(const Cmd::order *, uint32_t)>;
	class Dispatcher
	{
		public:
            Dispatcher(){}
			Dispatcher(const std::string & name) : mName(name){}
			~Dispatcher(){}
			const std::string & getName() const { return mName;}
			void regCallback(uint32_t cmdId, CallBackFunT fun)
			{
          //    funTable.insert(std::make_pair<uint32_t,  std::function<bool (const Cmd::order *, uint32_t) > > (cmdId, fun));// 好奇怪 为什么这样不行
                funTable[cmdId] = fun;
			}

			/*bool dispatcher(const char *cmd, uint32_t cmdLen, int taskId)
			{
				CallBackFunT fun = funTable[cmd->mCmdId];
				if (fun)
				{
					return fun(cmd, cmdLen, taskId);
				}
				else
				{
					std::cerr<<"cmdId:"<<cmd->mCmdId<<"not regeister callback"<<" taskId: "<<taskId<<std::endl;
				}
				return false;
			}*/
        public:
             bool DealOrderMsg(const Cmd::order *pOrder, uint32_t );

		private:
            std::map<uint32_t, std::function< bool(const Cmd::order *, uint32_t) > > funTable;
			std::string mName;
	};

}
	
#endif
