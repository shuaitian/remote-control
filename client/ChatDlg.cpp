// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include <map>
#include "UserDomain.h"
#include "SocketInstance.h"
#include "RemoteScreenDlg.h"
#include "Utils.h"
using namespace std;


// ChatDlg 对话框

IMPLEMENT_DYNAMIC(ChatDlg, CDialogEx)

ChatDlg::ChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent),
	username()
{
	Utils::remoteScreendlg->Create(IDD_DIALOG8, NULL);
}

ChatDlg::~ChatDlg()
{
}

BOOL ChatDlg::OnInitDialog() {
	CDialogEx();
	//AfxMessageBox(username.c_str());
	map<string, UserDomain>::iterator iter;
	iter = Utils::friendMap->find(username);
	if (iter != Utils::friendMap->end()) {
		UserDomain user = iter->second;
		if (user.dlg != NULL) {
			return false;
		}
	}
	return true;
}

void ChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_sendText);
	DDX_Control(pDX, IDC_EDIT1, m_recvText);
	//DDX_Control(pDX, IDC_EDIT5, m_test);
}


BEGIN_MESSAGE_MAP(ChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ChatDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ChatDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &ChatDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &ChatDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &ChatDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// ChatDlg 消息处理程序

void ChatDlg::updateName() {
	char buff[128] = { 0 };
	sprintf_s(buff, "与 %s 聊天", username.c_str());
	SetWindowText(buff);
}


void ChatDlg::OnBnClickedButton1()
{

	CString textSend;
	GetDlgItem(IDC_EDIT2)->GetWindowText(textSend);
	if (strcmp(textSend.GetString(),"") == 0) { return; }
	char buffer[1024] = { 0 };	
	sprintf_s(buffer, "17:%s:%s", username.c_str(), textSend.GetString());
	Utils::sendMsg(buffer);
	addRecvMessage(string(textSend.GetString()),Utils::curUser);
	GetDlgItem(IDC_EDIT2)->SetWindowText("");
	CEdit*p = (CEdit*)GetDlgItem(IDC_EDIT1);	//定义一个指向编辑框的句柄的指针
	p->LineScroll(p->GetLineCount());	//滚动条置底
}

void ChatDlg::addRecvMessage(string msg,string user) {
	CString recvText;
	GetDlgItem(IDC_EDIT1)->GetWindowText(recvText);
	std::string result(recvText.GetString());
	if (strcmp(recvText.GetString(), "") != 0) {
		result += "\r\n";
	}
	result += user;
	result += ":\r\n";
	result += "--->";
	result += msg;
	//	m_recvText.SetWindowText(result.c_str());
	GetDlgItem(IDC_EDIT1)->SetWindowText(result.c_str());
}

void ChatDlg::updateMessage() {
	map<string, vector<string>>::iterator unreadMsgIter;
	unreadMsgIter = Utils::unreadMsg->find(username);
	if (unreadMsgIter == Utils::unreadMsg->end()) {
		//return;
	}
	else {
		vector<string>::iterator msgIter;
		for (msgIter = (unreadMsgIter->second).begin(); msgIter != (unreadMsgIter->second).end(); ++msgIter) {
			addRecvMessage((*msgIter),username);
		}
		(unreadMsgIter->second).clear();
	}
	CEdit*p = (CEdit*)GetDlgItem(IDC_EDIT1);	//定义一个指向编辑框的句柄的指针
	p->LineScroll(p->GetLineCount());	//滚动条置底
}


void ChatDlg::OnBnClickedButton2()
{
	CString textSend;
	GetDlgItem(IDC_EDIT2)->GetWindowText(textSend);
	char buffer[1024] = { 0 };
	sprintf_s(buffer,"19:%s:%s",username.c_str(),textSend.GetString());
	Utils::sendMsg(buffer);
}


void ChatDlg::OnBnClickedButton3()
{
	/*
	uint32_t size;
	Utils::screenShot(&size);
	CImage image;
	image.Load("temp.bmp");
	image.Save("send.jpg");
	FILE *pFile = fopen("send.jpg", "rb");
	char *pBuf;
	fseek(pFile, 0, SEEK_END);
	int len = ftell(pFile);
	pBuf = new char[len + 1+3];
	memset(pBuf,0,len+4 + username.size());
	sprintf_s(pBuf, 4 + username.size() + 1, "22:%s:", username.c_str());
	rewind(pFile);   //指针复位  
	fread(pBuf+4+username.size(), 1, len, pFile);
	//pBuf[len + 3] = 0;
	fclose(pFile);
	uint32_t sendSize = htonl(len + 4 + username.size());
	//uint32_t sendSize = htonl(30);
	SocketInstance::getInstance()->Send(&sendSize, sizeof(sendSize));
	SocketInstance::getInstance()->Send(pBuf, len  + 4+username.size());
	//Utils::sendMsg(pBuf);
	*/
	char buffer[128] = { 0 };
	sprintf_s(buffer,"23:%s",username.c_str());
	Utils::sendMsg(buffer);

}


void ChatDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码

	CString textSend;
	GetDlgItem(IDC_EDIT2)->GetWindowText(textSend);
	char buffer[128] = { 0 };
	sprintf_s(buffer,"27:%s:%s",username.c_str(),textSend.GetString());
	Utils::sendMsg(buffer);
}

void ChatDlg::OnOK()
{
	//throw std::logic_error("The method or operation is not implemented.");
	OnBnClickedButton1();
}
bool remoteControl = false;
DWORD WINAPI threadFunc(LPVOID param) {
	while (remoteControl) {
		Sleep(500);
		// TODO: 在此添加控件通知处理程序代码
		//if (!Utils::_remotescreenSend) {
		char buffer[128] = { 0 };
		sprintf_s(buffer, "23:%s", param);
		Utils::sendMsg(buffer);
		Utils::_remotescreenSend = true;
		//}
	}

	return NULL;
}

void ChatDlg::OnBnClickedButton4()
{
	AfxMessageBox("暂未实现");
	return;
	/*
	if(!remoteControl){
		remoteControl = true;
		DWORD dwThreadID = 0;
		HANDLE handleFirst = CreateThread(NULL, 0, threadFunc,(LPVOID)username.c_str(), 0, &dwThreadID);
	}
	else {
		remoteControl = false;
	}
	/*
	uint32_t size;
	char *pBmp = Utils::screenShot(&size);
	CImage imageBmp;
	Utils::ptrToImage(pBmp, size, &imageBmp);
	CImage imageJpg;
	Utils::BmpToJpg(&imageBmp, &imageJpg);
	imageJpg.Save("test1.jpg");
	CImage smallJpg;
	Utils::CreateStretchImage(&imageJpg, &smallJpg, 480, 840);
	smallJpg.Save("small.jpg");
	*/
	if (!Utils::remoteScreendlg->IsWindowVisible()) {
		Utils::remoteScreendlg->ShowWindow(SW_SHOW);
	}
	char buffer[128] = { 0 };
	sprintf_s(buffer, "29:%s",username.c_str());
	Utils::sendMsg(buffer);
//	Utils::
}
