// AddFriendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "AddFriendDlg.h"
#include "afxdialogex.h"
#include "SocketInstance.h"
#include "Utils.h"
#include "boost/bind.hpp"


// AddFriendDlg 对话框

IMPLEMENT_DYNAMIC(AddFriendDlg, CDialogEx)

AddFriendDlg::AddFriendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

BOOL AddFriendDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();
	SocketInstance::getInstance()->setAddFriendReqSendlineCallback(boost::bind(&AddFriendDlg::onAddFriendReqSend, this));
	SocketInstance::getInstance()->setFriendNotOnlineCallback(boost::bind(&AddFriendDlg::onFriendNotOnline, this));
	SocketInstance::getInstance()->setIsFriendCallback(boost::bind(&AddFriendDlg::onIsFriend,this));
	return TRUE;
}

AddFriendDlg::~AddFriendDlg()
{
}

void AddFriendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, _friendname);
}


BEGIN_MESSAGE_MAP(AddFriendDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &AddFriendDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// AddFriendDlg 消息处理程序


void AddFriendDlg::OnBnClickedButton1()
{
	
	CString friendname;
	_friendname.GetWindowText(friendname);
	char buffer[1024] = { 0 };
	sprintf_s(buffer, "3:%s", friendname.GetString());
	int32_t size = strlen(buffer);
	//AfxMessageBox(buffer);
	//SocketInstance::getInstance()->Send(&size,sizeof(size));
	//SocketInstance::getInstance()->Send(buffer, strlen(buffer));
	Utils::sendMsg(buffer);
	
}

void AddFriendDlg::onAddFriendReqSend() {
	AfxMessageBox("好友请求已经发送");
	this->ShowWindow(SW_HIDE);
}

void AddFriendDlg::onFriendNotOnline() {
	AfxMessageBox("该用户不在线");
}

void AddFriendDlg::onIsFriend() {
	AfxMessageBox("已经是好友");
}
