// MessageShowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient.h"
#include "MessageShowDlg.h"
#include "afxdialogex.h"


// MessageShowDlg �Ի���

IMPLEMENT_DYNAMIC(MessageShowDlg, CDialogEx)

MessageShowDlg::MessageShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{

}

MessageShowDlg::~MessageShowDlg()
{
}

void MessageShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MessageShowDlg, CDialogEx)
END_MESSAGE_MAP()


// MessageShowDlg ��Ϣ�������

void MessageShowDlg::updateInfomation(const char* info) {

	GetDlgItem(IDC_EDIT1)->SetWindowText(CString(info));
}
