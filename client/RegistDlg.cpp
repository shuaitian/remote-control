// RegistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "RegistDlg.h"
#include "afxdialogex.h"
#include "Message.h"
#include "SocketInstance.h"
#include <string>
#include <boost/bind.hpp>


// RegistDlg 对话框

IMPLEMENT_DYNAMIC(RegistDlg, CDialogEx)

RegistDlg::RegistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

RegistDlg::~RegistDlg()
{
}

BOOL RegistDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();
	SocketInstance::getInstance()->setRegistSuccessCallback(boost::bind(&RegistDlg::onRegistSuccess,this));
	SocketInstance::getInstance()->setRegistFailueCallback(boost::bind(&RegistDlg::onRegistFailue, this,_1));

	return TRUE;
}

void RegistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, _username);
	DDX_Control(pDX, IDC_EDIT2, _password);
	DDX_Control(pDX, IDC_EDIT3, _passwordconfirm);
	DDX_Control(pDX, IDC_EDIT4, _mail);
}


BEGIN_MESSAGE_MAP(RegistDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &RegistDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// RegistDlg 消息处理程序


void RegistDlg::OnBnClickedButton1()
{
	CString username;
	_username.GetWindowText(username);
	CString password;
	_password.GetWindowText(password);
	CString passwordconfirm;
	_passwordconfirm.GetWindowText(passwordconfirm);
	CString mail;
	_mail.GetWindowText(mail);

	if (!validate(username, "用户名不能为空")) { return; } 
	if (!validate(password, "密码不能为空")) { return; }
	if (!validate(passwordconfirm, "确认密码不能为空")) { return; } 
	if (!validate(mail, "邮箱不能为空")) { return; }
	/*
	Message msg;
	memset(&msg, 0, sizeof(msg.data));
	msg.code = htonl(0);
	sprintf_s(msg.data, "%d%s:%s:%s",msg.code, username.GetString(), password.GetString(), mail.GetString());
	char buffer[1024] = { 0 };
	sprintf_s(buffer, "%d%s");*/
	char head[8] = { 128 };
	char buffer[1024] = { 0 };
	uint32_t code = 0;
	char*p = (char*)&code;
//	head[4] = p[0]; head[5] = p[1]; head[6] = p[2]; head[7] = p[3];
	char* const index = &buffer[8];
	sprintf_s(buffer,"%d:%s:%s:%s",code, username.GetString(), password.GetString(), mail.GetString());
	uint32_t size = htonl(strlen(buffer));
	char temp[128] = { 0 };

	p = (char*)&size;

//	head[0] = p[0]; head[1] = p[1]; head[2] = p[2]; head[3] = p[3];
	char res[1024] = { 0 };
	strcat_s(res, head); strcat_s(res,buffer);
	SocketInstance::getInstance()->Send((const void*)&size, sizeof(size));
	SocketInstance::getInstance()->Send((const void*)buffer, strlen(buffer));
}

bool RegistDlg::validate(CString data,const char* info) {
	if (strcmp(data.GetString(), "") == 0) {
		MessageBox(info);
		return false;
	}
	return true;
}

void RegistDlg::onRegistFailue(int code) {
	if (code == 3) {
		AfxMessageBox("注册失败，用户名已经存在");
	}

}

void RegistDlg::onRegistSuccess() {
	AfxMessageBox("恭喜，注册成功");
	ShowWindow(SW_HIDE);
}
