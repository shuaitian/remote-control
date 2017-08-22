// RemoteScreenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "RemoteScreenDlg.h"
#include "afxdialogex.h"
#include "Utils.h"


// RemoteScreenDlg 对话框

IMPLEMENT_DYNAMIC(RemoteScreenDlg, CDialogEx)

RemoteScreenDlg::RemoteScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG8, pParent)
{
	image = NULL;
}

RemoteScreenDlg::~RemoteScreenDlg()
{
}

void RemoteScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RemoteScreenDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &RemoteScreenDlg::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// RemoteScreenDlg 消息处理程序


void RemoteScreenDlg::OnBnClickedButton1()
{
	Utils::isSendFrame = false;
	char buffer[128] = { 0 };
	sprintf_s(buffer,"30:stop remotesceen");
	Utils::sendMsg(buffer);

}

void RemoteScreenDlg::showImage() {
	if (image == NULL) {
		return;
	}
	int cx, cy;
	CRect   rect;
	//根据路径载入图片  
	//imageTemp.Load("C:\\image.jpg");

	//	DeleteFile("image.jpg");
	//获取图片的宽 高度  
	cx = image->GetWidth();
	cy = image->GetHeight();
	char info[128] = { 0 };
	//sprintf_s(info,"cx:%d,cy:%d",cx,cy);
	//AfxMessageBox(info);
	//获取Picture Control控件的大小  
	GetDlgItem(IDC_REMOTESC_STATIC)->GetWindowRect(&rect);
	//将客户区选中到控件表示的矩形区域内  
	ScreenToClient(&rect);
	//窗口移动到控件表示的区域  
	GetDlgItem(IDC_REMOTESC_STATIC)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_REMOTESC_STATIC);//获取控件句柄  
	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小  

	CDC *pDc = NULL;
	pDc = pWnd->GetDC();//获取picture的DC  

	image->Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内  

	ReleaseDC(pDc);
}

void RemoteScreenDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	showImage();
}


void RemoteScreenDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	char buffer[128] = { 0 };
	sprintf_s(buffer, "30:stop remotesceen");
	Utils::sendMsg(buffer);
	CDialogEx::OnClose();
}
