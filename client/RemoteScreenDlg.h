#pragma once


// RemoteScreenDlg 对话框

class RemoteScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RemoteScreenDlg)

public:
	RemoteScreenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RemoteScreenDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	void showImage();
	CImage *image;
	afx_msg void OnPaint();
	afx_msg void OnClose();
};
