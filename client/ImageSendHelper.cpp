#include "stdafx.h"
#include "ImageSendHelper.h"
#include "Utils.h"
#include <stdint.h>


ImageSendHelper::ImageSendHelper()
{
	if (!init()) {
		return;
	}
	sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(Utils::IP);      // ���ػ�·��ַ��127.0.0.1;   
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(9002);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
}

bool ImageSendHelper::init() {
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

void ImageSendHelper::sendData(const char* data, int size) {
	uint32_t sizes = htonl(size);
	send(sockClient,(const char*)&sizes,sizeof(sizes),0);
	send(sockClient,data, size,0);
}


ImageSendHelper::~ImageSendHelper()
{
}
