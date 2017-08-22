
// ChatClientDlg.h : 头文件
//

#pragma once
#include "MyBtn.h"
#include "afxwin.h"
#include "ChatPanel.h"
#include "RegistDlg.h"

class MyBtn;
class MySock;
// CChatClientDlg 对话框
class CChatClientDlg : public CDialogEx
{
// 构造
public:
	CChatClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
	void initSocket();
public:
	CEdit _username;
	CEdit _password;
	MyBtn _submit;
	afx_msg void OnBnClickedButton1();
	MySock *_mysock;
	afx_msg void OnBnClickedButton2();
	void onLoginSuccess();
	void onLoginFailue();
	void onAddFriendRefuse(const char* usename);

	virtual void OnOK() override;

	ChatPanel *_panel;
	RegistDlg *_regist;
};
