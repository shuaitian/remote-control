#pragma once


// RemoteScreenShowDlg 对话框

class RemoteScreenShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RemoteScreenShowDlg)

public:
	RemoteScreenShowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RemoteScreenShowDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
