#ifndef _UTILS_H_
#define _UTILS_H_
#include <vector>
#include <string>
#include <iostream>
#include <muduo/net/TcpConnection.h>
#include <map>
#include <queue>
using namespace muduo;
using namespace std;

class UserDomain;
class Utils
{
public:
	static void splitString(vector<muduo::string> *res,const char* data,const char *pattern);
	static const char* getUsernameFromConn(net::TcpConnectionPtr conn);
	static UserDomain* getUserDomainFromConn(net::TcpConnectionPtr conn);
	static map<std::string,net::TcpConnectionPtr> *onlineList;
	static net::TcpConnectionPtr findConnByUsername(const char* username);
	static map<muduo::string,queue<muduo::string> > *screenFrame;
};

#endif