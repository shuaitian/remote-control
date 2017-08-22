#pragma once
#include <string>
#include "afxwin.h"


// ChatDlg �Ի���

class ChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ChatDlg)

public:
	ChatDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ChatDlg();
	virtual BOOL OnInitDialog();


// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	std::string username;
	void updateName();
	CEdit m_sendText;
	afx_msg void OnBnClickedButton1();
	CEdit m_recvText;
	CEdit m_test;
	void updateMessage();
	void addRecvMessage(std::string msg,std::string user);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton6();

	virtual void OnOK() override;

	afx_msg void OnBnClickedButton4();
	bool remotescreenBegin = false;
};
