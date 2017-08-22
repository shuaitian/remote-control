#pragma once
#include "afxwin.h"


// ScreenshotDlg 对话框

class ScreenshotDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ScreenshotDlg)

public:
	ScreenshotDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ScreenshotDlg();
	virtual BOOL OnInitDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_pic;
	void showImage();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
	int imageSize;
	CImage *_image;
	afx_msg void OnClose();
};
