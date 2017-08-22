#pragma once
#include "afxwin.h"


// AddFriendDlg 对话框

class AddFriendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AddFriendDlg)

public:
	AddFriendDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AddFriendDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit _friendname;
	afx_msg void OnBnClickedButton1();
	void onAddFriendReqSend();
	void onFriendNotOnline();
	void onIsFriend();
};
