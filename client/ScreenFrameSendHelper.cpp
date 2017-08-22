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
	addrSrv.sin_addr.S_un.S_addr = inet_addr(Utils::IP);      // ���ػ�·��ַ��127.0.0.1;   
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(9003);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
}

bool ScreenFrameSendHelper::init() {
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
