CXXFLAGS=-Wall  -std=c++11 -g -Wparentheses
.PHONY : all
all : test 
test : test.o MySocket.o MySockTaskManager.o MySockTask.o MyThread.o MySockTaskPool.o MyTask.o MyTcpClient.o MySockClientTask.o MyClientTask.o MySockClientTaskPool.o  MyClient.o libmemmory.so  MatchingOrder.o 
	g++ $(CXXFLAGS) -o test  test.o MySocket.o MySockTask.o MyThread.o MySockTaskManager.o MySockTaskPool.o  MyTask.o MyTcpClient.o MySockClientTask.o MyClientTask.o MySockClientTaskPool.o -lpthread -L./ -lmemmory MyClient.o  MatchingOrder.o MyClientTaskManager.cpp libxml.a -lxml2
test.o: test.cpp
	g++ -c -g test.cpp
MySocket.o : ../base/MySocket.cpp ../base/MySocket.h ../base/MyLock.h
	g++ $(CXXFLAGS) -c ../base/MySocket.cpp 

MySockTaskManager.o : ../base/MySockTaskManager.h ../base/MySockTaskManager.cpp
	g++ $(CXXFLAGS) -c ../base/MySockTaskManager.cpp

MySockTask.o : ../base/MySockTask.cpp ../base/MySockTask.h ../base/MyQueue.h ../base/MySocket.h ../mem/model/model_te.h ../mem/model/order.h
	g++ $(CXXFLAGS) -c ../base/MySockTask.cpp -L./ -lmemmory

MyThread.o : ../base/MyThread.cpp ../base/MyThread.h ../base/MyLock.h
	g++ $(CXXFLAGS) -c ../base/MyThread.cpp -lpthread

MySockTaskPool.o : ../base/MySockTaskPool.cpp ../base/MySockTaskPool.h ../base/MySockTaskManager.h
	g++ $(CXXFLAGS) -c ../base/MySockTaskPool.cpp

MyTask.o : ../base/MyTask.cpp ../base/MyTask.h
	g++ $(CXXFLAGS) -c ../base/MyTask.cpp

MyTcpClient.o : MyTcpClient.cpp MyTcpClient.h
	g++ $(CXXFLAGS) -c MyTcpClient.cpp

MySockClientTask.o : MySockClientTask.cpp MySockClientTask.h
	g++ $(CXXFLAGS) -c MySockClientTask.cpp

MyClientTask.o : MyClientTask.cpp MyClientTask.h
	g++ $(CXXFLAGS) -c MyClientTask.cpp

MySockClientTaskPool.o : MySockClientTaskPool.cpp MySockClientTaskPool.h
	g++ $(CXXFLAGS) -c MySockClientTaskPool.cpp

MyClient.o : MyClient.cpp MyClient.h
	g++ $(CXXFLAGS) -c MyClient.cpp
MatchingOrder.o : ../MatchingOrder.cpp ../MatchingOrder.h
	g++ $(CXXFLAGS) -c ../MatchingOrder.cpp
libmemmory.so : ../mem/memory.h ../mem/memory.c ../mem/util/log/log.h ../mem/util/log/log.c ../mem/util/log/log_level.h ../mem/util/log/log_mode.h  ../mem/util/log/log_level.c ../mem/util/log/log_mode.c  ../mem/env/env.h ../mem/env/env.c ../mem/util/env_base/env_base.h  ../mem/util/env_base/env_base.c ../mem/util/file/common_file.h ../mem/util/file/common_file.c ../mem/util/time/common_time.h ../mem/util/time/common_time.c ../mem/util/string/common_str.h ../mem/util/string/common_str.c ../mem/util/macro/macro.h ../mem/util/macro/macro.c  ../mem/util/log/log_file.h ../mem/util/log/log_file.c ../mem/util/log/log_impl_console.h ../mem/util/log/log_impl_console.c ../mem/util/log/log_config.h ../mem/util/log/log_config.c  ../mem/util/ini/get_ini.h ../mem/util/ini/get_ini.c ../mem/model/order.h ../mem/model/model_te.h ../mem/model/model.h ../mem/model/stock.h ../mem/model/price.h ../mem/model/industry.h ../mem/model/category.h
	g++ $(CPPFLAGS) -fPIC -shared -o $@ ../mem/memory.c ../mem/util/log/log.c ../mem/util/log/log_level.c ../mem/util/log/log_mode.c  ../mem/env/env.c ../mem/util/env_base/env_base.c ../mem/util/file/common_file.c ../mem/util/time/common_time.c ../mem/util/string/common_str.c ../mem/util/macro/macro.c ../mem/util/log/log_file.c ../mem/util/log/log_impl_console.c ../mem/util/log/log_config.c ../mem/util/ini/get_ini.c ../mem/model/order.c ../mem/model/stock.c ../mem/model/price.c ../mem/model/industry.c ../mem/model/category.c
clean:
	rm -rf test
	rm -rf *.o
