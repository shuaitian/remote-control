#pragma once


// MessageShowDlg �Ի���

class MessageShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MessageShowDlg)

public:
	MessageShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MessageShowDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void updateInfomation(const char* info);
};
