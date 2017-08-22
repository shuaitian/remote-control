#include "stdafx.h"
#include "SocketInstance.h"
#include "Utils.h"
#pragma comment(lib,"ws2_32.lib")

MySock* SocketInstance::_instance = NULL;

std::string SocketInstance::curuser = "";

SocketInstance::SocketInstance()
{

}


SocketInstance::~SocketInstance()
{
}

void SocketInstance::init() {
	_instance = new MySock();
	BOOL bFlag = _instance->Create();
	if (!bFlag) {
		//MessageBox("Socket Error!");
		_instance->Close();
		return;
	}
	//115.28.92.172
	bFlag = _instance->Connect(Utils::IP, 9000);
}

MySock* SocketInstance::getInstance() {
	if (_instance == NULL) {
		init();
	}
	return _instance;
}
