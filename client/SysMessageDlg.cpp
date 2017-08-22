// SysMessageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "SysMessageDlg.h"
#include "afxdialogex.h"
#include "Utils.h"


// SysMessageDlg 对话框

IMPLEMENT_DYNAMIC(SysMessageDlg, CDialogEx)

SysMessageDlg::SysMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

SysMessageDlg::~SysMessageDlg()
{
}

void SysMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, _msg);
	DDX_Control(pDX, IDC_BUTTON1, _accbtn);
	DDX_Control(pDX, IDC_BUTTON2, _refbtn);
}


BEGIN_MESSAGE_MAP(SysMessageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &SysMessageDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &SysMessageDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// SysMessageDlg 消息处理程序


void SysMessageDlg::OnBnClickedButton1()
{
	char buffer[128] = { 0 };
	sprintf_s(buffer, "7:%s", _username.c_str());
	//AfxMessageBox(buffer);
	Utils::sendMsg(buffer);
	ShowWindow(SW_HIDE);
}


void SysMessageDlg::OnBnClickedButton2()
{
	char buffer[128] = { 0 };
	sprintf_s(buffer, "9:%s", _username.c_str());
	//AfxMessageBox(buffer);
	Utils::sendMsg(buffer);
	ShowWindow(SW_HIDE);
}
