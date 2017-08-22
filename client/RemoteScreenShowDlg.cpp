// RemoteScreenShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "RemoteScreenShowDlg.h"
#include "afxdialogex.h"


// RemoteScreenShowDlg 对话框

IMPLEMENT_DYNAMIC(RemoteScreenShowDlg, CDialogEx)

RemoteScreenShowDlg::RemoteScreenShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG8, pParent)
{

}

RemoteScreenShowDlg::~RemoteScreenShowDlg()
{
}

void RemoteScreenShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RemoteScreenShowDlg, CDialogEx)
END_MESSAGE_MAP()


// RemoteScreenShowDlg 消息处理程序
