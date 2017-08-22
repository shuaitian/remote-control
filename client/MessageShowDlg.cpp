// MessageShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "MessageShowDlg.h"
#include "afxdialogex.h"


// MessageShowDlg 对话框

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


// MessageShowDlg 消息处理程序

void MessageShowDlg::updateInfomation(const char* info) {

	GetDlgItem(IDC_EDIT1)->SetWindowText(CString(info));
}
