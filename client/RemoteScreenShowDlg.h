#pragma once


// RemoteScreenShowDlg �Ի���

class RemoteScreenShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RemoteScreenShowDlg)

public:
	RemoteScreenShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RemoteScreenShowDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
