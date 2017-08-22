#ifndef _CHATMESSAGE_H_
#define _CHATMESSAGE_H_
#include <string>
#include <iostream>
#include <muduo/net/TcpConnection.h>
using namespace std;

class ChatMessage
{
public:
	ChatMessage(muduo::net::TcpConnectionPtr conn,const char* data);
	muduo::string from;
	muduo::string to;
	muduo::string msg;
	muduo::string toString();
};

#endif