#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include <muduo/net/TcpConnection.h>
#include <stdint.h>
#include <string>
class Message
{
public:
	int32_t code;
	muduo::string data;
};

#endif