#include "stdafx.h"
#include "ScreenFrameSendHelper.h"
#include "Utils.h"
#include "SocketInstance.h"


ScreenFrameSendHelper::ScreenFrameSendHelper()
{
	if (!init()) {
		return;
	}
	sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(Utils::IP);      // 本地回路地址是127.0.0.1;   
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(9003);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
}

bool ScreenFrameSendHelper::init() {
	WORD wVersionRequested;
	WSADATA wsaData;    // 这结构是用于接收Wjndows Socket的结构信息的  
	int err;

	wVersionRequested = MAKEWORD(1, 1);   // 请求1.1版本的WinSock库  

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return false;          // 返回值为零的时候是表示成功申请WSAStartup  
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		// 检查这个低字节是不是1，高字节是不是1以确定是否我们所请求的1.1版本  
		// 否则的话，调用WSACleanup()清除信息，结束函数  
		WSACleanup();
		return false;
	}
	return true;
}

void ScreenFrameSendHelper::senData(const char* data, uint32_t size) {
	uint32_t dataSize = htonl(size);
	//SocketInstance::getInstance()->Send(&dataSize,sizeof(dataSize));
	//SocketInstance::getInstance()->Send(data, size);
	send(sockClient,(const char*)&dataSize,sizeof(dataSize),0);
	send(sockClient,(const char*)data,size,0);
}

ScreenFrameSendHelper::~ScreenFrameSendHelper()
{
}
