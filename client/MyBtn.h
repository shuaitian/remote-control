#pragma once
//#include "ChatClientDlg.h"
class CChatClientDlg;
class MySock;
class MyBtn : public CButton
{
public:
	MyBtn();
	~MyBtn();
	void onMessage(CChatClientDlg* dlg);
private:
	void initSocket(MySock *sock);
};

