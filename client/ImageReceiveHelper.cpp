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
	WSADATA wsaData;    // ��ṹ�����ڽ���Wjndows Socket�Ľṹ��Ϣ��  
	int err;

	wVersionRequested = MAKEWORD(1, 1);   // ����1.1�汾��WinSock��  

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return false;          // ����ֵΪ���ʱ���Ǳ�ʾ�ɹ�����WSAStartup  
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		// ���������ֽ��ǲ���1�����ֽ��ǲ���1��ȷ���Ƿ������������1.1�汾  
		// ����Ļ�������WSACleanup()�����Ϣ����������  
		WSACleanup();
		return false;
	}
	return true;
}

char* ImageReceiveHelper::receiveImage(int port,int size) {
	sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(Utils::IP);      // ���ػ�·��ַ��127.0.0.1;   
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
	WSACleanup();   // ��ֹ���׽��ֿ��ʹ��  

	return data;
}