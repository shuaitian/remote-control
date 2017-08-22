#ifndef _USERDOMAIN_H_
#define _USERDOMAIN_H_
#include <string>
#include <vector>
#include <iostream>
#include <muduo/net/TcpConnection.h>
#include <muduo/base/Mutex.h>

using namespace std;
using std::string;

class UserDomain
{
public:
	UserDomain(const char* data);
	UserDomain();
	const char* getUsername();
	const char* getPassword();
	const char* getMail();
	const char* getOnline();
	const char* getLevel();
	muduo::string& getImagedata(){
		return _imageData;
	}

	void setUsername(const char* username);
	void setPassword(const char* passowrd);
	void setMail(const char* mail);
	void setOnline(const char* online);
	void setLevel(const char* level);
	void setImageData(muduo::string data){
		_imageData = data;
	}
	muduo::string toString();
	muduo::string remoteScreenUsername;
	void remoteScreenTimer();
	void setThreadIsRunning(bool para)
	{
		threadisRunning = para; 
	}

	void onScreenFrameToConnection(const muduo::net::TcpConnectionPtr &conn);
	void* sendDataThreadFunc(void *);
private:
	muduo::string _username;
	muduo::string _password;
	muduo::string _mail;
	muduo::string _online;
	muduo::string _level;
	muduo::string _imageData;
	void threadFunc();
public:
	bool threadisRunning;
	bool isSenderRecv;
//	MutexLock mutex;
	
};

#endif