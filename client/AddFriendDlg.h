#pragma once
#include "afxwin.h"


// AddFriendDlg �Ի���

class AddFriendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AddFriendDlg)

public:
	AddFriendDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AddFriendDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit _friendname;
	afx_msg void OnBnClickedButton1();
	void onAddFriendReqSend();
	void onFriendNotOnline();
	void onIsFriend();
};
