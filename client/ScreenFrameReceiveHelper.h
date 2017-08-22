#pragma once
#include <string>
class ScreenFrameReceiveHelper
{
public:
	ScreenFrameReceiveHelper();
	~ScreenFrameReceiveHelper();
	void receiveData();
	SOCKET clientSocket;
	bool isRunning;
	std::string targetName;
	char* readByte(int size);
};

