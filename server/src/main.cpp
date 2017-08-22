#include "Commons.h"
#include "ChatServer.h"
#include <muduo/net/InetAddress.h>
#include <unistd.h>
#include <muduo/base/Logging.h>

void func(){
	cout<<CurrentThread::name<<" is running"<<endl;
}

void outputFunc(const char* msg, int len){

}

void flushFunc(){

}

int main(){
	Logger::setOutput(outputFunc);
	Logger::setFlush(flushFunc);
	net::EventLoop loop;
	net::InetAddress addr(9000);
	ChatServer server(&loop,addr);
	server.start();
	loop.loop();
	return 0;
}