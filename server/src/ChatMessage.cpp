#include "ChatMessage.h"
#include "Utils.h"
#include <vector>
using namespace std;

ChatMessage::ChatMessage(net::TcpConnectionPtr conn,const char* data):
	from(),
	to(),
	msg()
{
	std::vector<muduo::string> v;
	Utils::splitString(&v,data,":");
	if(v.size() != 2){
		return;
	}
	to = v[0];
	msg = v[1];
	from = Utils::getUsernameFromConn(conn);
}

muduo::string ChatMessage::toString(){
	muduo::string res;
	res += "from:";
	res += from;
	res += ",";
	res += "to:";
	res += to;
	res += ",";
	res += "msg";
	res += msg;
	return res;
}