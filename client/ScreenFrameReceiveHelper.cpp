#include "stdafx.h"
#include "ScreenFrameReceiveHelper.h"
#include "Utils.h"


ScreenFrameReceiveHelper::ScreenFrameReceiveHelper():targetName()	
{
	WORD wVersionRequested;
	WSADATA wsaData;    // 这结构是用于接收Wjndows Socket的结构信息的  
	int err;

	wVersionRequested = MAKEWORD(1, 1);   // 请求1.1版本的WinSock库  

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return ;          // 返回值为零的时候是表示成功申请WSAStartup  
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		// 检查这个低字节是不是1，高字节是不是1以确定是否我们所请求的1.1版本  
		// 否则的话，调用WSACleanup()清除信息，结束函数  
		WSACleanup();
		return ;
	}
	isRunning = true;

	return ;
}

void ScreenFrameReceiveHelper::receiveData() {
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	//AfxMessageBox(targetName.c_str());
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(Utils::IP);      // 本地回路地址是127.0.0.1;   
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(9004);
	connect(clientSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	char buffer[256] = { 0 };
	sprintf_s(buffer,"%s:%s", Utils::curUser.c_str(), targetName.c_str());
	send(clientSocket, buffer, strlen(buffer), 0);
	char recvBuff[1024] = { 0 };
//	AfxMessageBox("in");
	while (true) {
		//Sleep(1);
		int dataSize = 0;
		int len = recv(clientSocket, (char*)&dataSize,sizeof(dataSize),0);
		char info[128] = { 0 };
		sprintf_s(info, "%d", dataSize);
//		AfxMessageBox(info);
	//	AfxMessageBox(info);
		if (len == 0) {
			break;
		}

		char* jpgPtr = readByte(dataSize);
		CImage *image = new CImage();
		Utils::ptrToImage(jpgPtr,dataSize,image);
		if (Utils::remoteScreendlg->image != NULL) {
			delete Utils::remoteScreendlg->image;
		}
		Utils::remoteScreendlg->image = image;
		Utils::remoteScreendlg->Invalidate(false);
		delete jpgPtr;
	}
}

char* ScreenFrameReceiveHelper::readByte(int size) {
	char info[128] = { 0 };
//	sprintf_s(info, "%d", size);
//	AfxMessageBox(info);
	int recvCount = 0;
	char *ret = new char[size];
	while (recvCount < size) {
		int len = recv(clientSocket,ret+recvCount,size-recvCount,0);
		recvCount += len;
	}
//	char info[128] = { 0 };
	sprintf_s(info, "%d", recvCount);
	//AfxMessageBox(info);
	return ret;
}


ScreenFrameReceiveHelper::~ScreenFrameReceiveHelper()
{
}
