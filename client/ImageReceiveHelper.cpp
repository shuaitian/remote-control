#include "stdafx.h"
#include "ImageReceiveHelper.h"
#include "Utils.h"
#include <string>


ImageReceiveHelper::ImageReceiveHelper()
{
	if (!init()) {
		return;
	}
	
}


ImageReceiveHelper::~ImageReceiveHelper()
{
}

bool ImageReceiveHelper::init() {
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

char* ImageReceiveHelper::receiveImage(int port,int size) {
	sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(Utils::IP);      // 本地回路地址是127.0.0.1;   
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	//recv(sockClient, recvBuf, 100, 0);
	int len = 0;
	int recvCount = 0;
	char *buffer = new char[size];
	char *data = new char[size];

	while (true) {
		len = recv(sockClient,buffer,size-recvCount,0);
		memcpy(data + recvCount, buffer, len);
		recvCount += len;
		if (recvCount == size) {
			break;
		}
	}

	//AfxMessageBox("ok");

//	char info[128] = { 0 };
//	sprintf_s(info,"%d bytes receive",recvCount);
//	AfxMessageBox(info);
//	FILE *pFile = fopen("C:\\image.jpg", "wb");
//	fwrite(data, size, 1, pFile);
//	fclose(pFile);
	//SetFileAttributes("image.bmp", FILE_ATTRIBUTE_HIDDEN);
	closesocket(sockClient);
	WSACleanup();   // 终止对套接字库的使用  

	return data;
}