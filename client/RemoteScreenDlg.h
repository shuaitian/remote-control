#pragma once


// RemoteScreenDlg �Ի���

class RemoteScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RemoteScreenDlg)

public:
	RemoteScreenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RemoteScreenDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	void showImage();
	CImage *image;
	afx_msg void OnPaint();
	afx_msg void OnClose();
};
