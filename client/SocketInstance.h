#pragma once
#include "MySock.h"
#include <string>
class SocketInstance
{
public:
	~SocketInstance();
	static MySock* getInstance();
	static std::string curuser;
private:
	SocketInstance();
	static MySock *_instance;
	static void init();
};

