CPPFLAGS = -Wall -std=c++11 -g -Wparentheses -Wsequence-point
.PHONY : all
all:gexn
gexn: server.o  MySocket.o MySockTaskManager.o  MyThread.o MySockTaskPool.o MyTcpServer.o MyBaseServer.o MySockTask.o MyServer.o MyServerMsgProcess.o MyServerTask.o MatchingOrder.o libmemmory.so 
	g++ $(CPPFLAGS) -o gexn server.o  MySocket.o MySockTaskManager.o  MyThread.o MySockTaskPool.o MyTcpServer.o  MyBaseServer.o MyServer.o MySockTask.o MyServerMsgProcess.o MyServerTask.o MatchingOrder.o -lpthread -L./ -lmemmory
MySocket.o : ./base/MySocket.cpp ./base/MySocket.h ./base/MyLock.h
	g++ $(CPPFLAGS) -c  ./base/MySocket.cpp
MySockTaskManager.o : ./base/MySockTaskManager.h ./base/MySockTaskManager.cpp
	g++ $(CPPFLAGS) -c  ./base/MySockTaskManager.cpp
MySockTask.o : ./base/MySockTask.cpp ./base/MySockTask.h ./base/MyQueue.h ./base/MySocket.h
	g++ $(CPPFLAGS) -c  ./base/MySockTask.cpp
MyThread.o : ./base/MyThread.cpp ./base/MyThread.h ./base/MyLock.h
	g++ $(CPPFLAGS) -c  ./base/MyThread.cpp -lpthread
MySockTaskPool.o : ./base/MySockTaskPool.cpp ./base/MySockTaskPool.h ./base/MySockTaskManager.h
	g++ $(CPPFLAGS) -c  ./base/MySockTaskPool.cpp
MyTcpServer.o : MyTcpServer.cpp MyTcpServer.h
	g++ $(CPPFLAGS) -c MyTcpServer.cpp
MyBaseServer.o : MyBaseServer.cpp MyBaseServer.h
	g++ $(CPPFLAGS) -c MyBaseServer.cpp
MyServer.o : MyServer.cpp MyServer.h ./base/CmdDispatcher.h
	g++ $(CPPFLAGS) -c MyServer.cpp
server.o : server.cpp MyServer.h ./base/MySockTaskManager.h
	g++ $(CPPFLAGS) -c server.cpp
MyServerMsgProcess.o : MyServerMsgProcess.cpp
	g++  $(CPPFLAGS) -c MyServerMsgProcess.cpp
MyServerTask.o : ./base/MyTask.cpp ./base/MyTask.h MyServerTask.h MyServerTask.cpp
	g++ $(CPPFLAGS) -c MyServerTask.cpp
MatchingOrder.o : ./MatchingOrder.cpp ./MatchingOrder.h
	g++ $(CPPFLAGS) -c MatchingOrder.cpp
libmemmory.so : ./mem/memory.h ./mem/memory.c ./mem/util/log/log.h ./mem/util/log/log.c ./mem/util/log/log_level.h ./mem/util/log/log_mode.h  ./mem/util/log/log_level.c ./mem/util/log/log_mode.c  ./mem/env/env.h ./mem/env/env.c ./mem/util/env_base/env_base.h  ./mem/util/env_base/env_base.c ./mem/util/file/common_file.h ./mem/util/file/common_file.c ./mem/util/time/common_time.h ./mem/util/time/common_time.c ./mem/util/string/common_str.h ./mem/util/string/common_str.c ./mem/util/macro/macro.h ./mem/util/macro/macro.c  ./mem/util/log/log_file.h ./mem/util/log/log_file.c ./mem/util/log/log_impl_console.h ./mem/util/log/log_impl_console.c ./mem/util/log/log_config.h ./mem/util/log/log_config.c  ./mem/util/ini/get_ini.h ./mem/util/ini/get_ini.c ./mem/model/order.h ./mem/model/model_te.h ./mem/model/model.h ./mem/model/stock.h ./mem/model/price.h ./mem/model/industry.h ./mem/model/category.h
	g++ $(CPPFLAGS) -fPIC -shared -o $@ ./mem/memory.c ./mem/util/log/log.c ./mem/util/log/log_level.c ./mem/util/log/log_mode.c  ./mem/env/env.c ./mem/util/env_base/env_base.c ./mem/util/file/common_file.c ./mem/util/time/common_time.c ./mem/util/string/common_str.c ./mem/util/macro/macro.c ./mem/util/log/log_file.c ./mem/util/log/log_impl_console.c ./mem/util/log/log_config.c ./mem/util/ini/get_ini.c ./mem/model/order.c ./mem/model/stock.c ./mem/model/price.c ./mem/model/industry.c ./mem/model/category.c

clean:
	rm -rf gexn
	rm -rf *.o
	rm -rf *.so
