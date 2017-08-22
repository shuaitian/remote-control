#pragma once


// MessageShowDlg 对话框

class MessageShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MessageShowDlg)

public:
	MessageShowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MessageShowDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void updateInfomation(const char* info);
};
