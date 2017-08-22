#pragma once
#include "afxwin.h"
#include <string>

// SysMessageDlg 对话框

class SysMessageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SysMessageDlg)

public:
	SysMessageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SysMessageDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit _msg;
	std::string _username;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CButton _accbtn;
	CButton _refbtn;
};
