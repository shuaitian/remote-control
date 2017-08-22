#include "stdafx.h"
#include "MyBtn.h"
#include "ChatClientDlg.h"
#include "MySock.h"
#include "Message.h"
#include "Utils.h"


MyBtn::MyBtn()
{
}

MyBtn::~MyBtn()
{
}

void MyBtn::onMessage(CChatClientDlg* dlg) {
	CString username;
	dlg->_username.GetWindowText(username);
	CString password;
	dlg->_password.GetWindowText(password);
	if (strcmp(username.GetString(), "") == 0) {
		MessageBox("用户名不能为空");
		return;
	}
	if (strcmp(password.GetString(), "") == 0) {
		MessageBox("密码不能为空");
		return;
	}
	//
	Message msg;
	uint32_t code = 1;
	char buffer[1024] = { 0 };
	sprintf_s(buffer,"%d:%s:%s",code,username.GetString(),password.GetString());

	if (dlg->_mysock == NULL) {
		MessageBox("sock为空");
		return;
	}
	Utils::sendMsg(buffer);
}

void MyBtn::initSocket(MySock *mysock) {
	mysock = new MySock();
	BOOL bFlag = mysock->Create();
	if (!bFlag) {
		MessageBox("Socket Error!");
		mysock->Close();
		//PostQuitMessage(0);
		return;
	}
	bFlag = mysock->Connect("172.20.77.5", 9000);

}


