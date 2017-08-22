
// ChatClientDlg.h : ͷ�ļ�
//

#pragma once
#include "MyBtn.h"
#include "afxwin.h"
#include "ChatPanel.h"
#include "RegistDlg.h"

class MyBtn;
class MySock;
// CChatClientDlg �Ի���
class CChatClientDlg : public CDialogEx
{
// ����
public:
	CChatClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
