#include "stdafx.h"
#include "ScreenFrameReceiveHelper.h"
#include "Utils.h"


ScreenFrameReceiveHelper::ScreenFrameReceiveHelper():targetName()	
{
	WORD wVersionRequested;
	WSADATA wsaData;    // ��ṹ�����ڽ���Wjndows Socket�Ľṹ��Ϣ��  
	int err;

	wVersionRequested = MAKEWORD(1, 1);   // ����1.1�汾��WinSock��  

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return ;          // ����ֵΪ���ʱ���Ǳ�ʾ�ɹ�����WSAStartup  
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		// ���������ֽ��ǲ���1�����ֽ��ǲ���1��ȷ���Ƿ������������1.1�汾  
		// ����Ļ�������WSACleanup()�����Ϣ����������  
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
	addrSrv.sin_addr.S_un.S_addr = inet_addr(Utils::IP);      // ���ػ�·��ַ��127.0.0.1;   
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
