#pragma once
#include "afxwin.h"


// RegistDlg 对话框

class RegistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RegistDlg)

public:
	RegistDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RegistDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit _username;
	CEdit _password;
	CEdit _passwordconfirm;
	CEdit _mail;
private:
	bool validate(CString data,const char *info);	
	void onRegistSuccess();
	void onRegistFailue(int code);
};
