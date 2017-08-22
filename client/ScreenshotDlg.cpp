// ScreenshotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ScreenshotDlg.h"
#include "afxdialogex.h"
#include "Utils.h"
#include "ImageReceiveHelper.h"


// ScreenshotDlg 对话框

IMPLEMENT_DYNAMIC(ScreenshotDlg, CDialogEx)

ScreenshotDlg::ScreenshotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG7, pParent)
{
	_image = NULL;
}

ScreenshotDlg::~ScreenshotDlg()
{
}

BOOL ScreenshotDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();
	//showImage();
	//UpdateWindow();
	this->CenterWindow();
	CRect   temprect(0, 0, 1400, 900);
	m_pic.SetWindowPos(NULL, 0, 0, temprect.Width(), temprect.Height(), SWP_NOZORDER | SWP_NOMOVE);
	SetWindowPos(NULL, 0, 0, temprect.Width(), temprect.Height(), SWP_NOZORDER | SWP_NOMOVE);
	return TRUE;
}

void ScreenshotDlg::showImage() {
	int cx, cy;
	CRect   rect;
	//根据路径载入图片  
	//imageTemp.Load("C:\\image.jpg");
	

	CImage image;
	Utils::CreateStretchImage(_image, &image, 900,1400);
//	DeleteFile("image.jpg");
	//获取图片的宽 高度  
	cx = image.GetWidth();
	cy = image.GetHeight();
	char info[128] = { 0 };
	//sprintf_s(info,"cx:%d,cy:%d",cx,cy);
	//AfxMessageBox(info);
	//获取Picture Control控件的大小  
	GetDlgItem(IDC_SCSHOOT_STATIC)->GetWindowRect(&rect);
	//将客户区选中到控件表示的矩形区域内  
	ScreenToClient(&rect);
	//窗口移动到控件表示的区域  
	GetDlgItem(IDC_SCSHOOT_STATIC)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_SCSHOOT_STATIC);//获取控件句柄  
	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小  

	CDC *pDc = NULL;
	pDc = pWnd->GetDC();//获取picture的DC  

	image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内  

	ReleaseDC(pDc);
}

void ScreenshotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCSHOOT_STATIC, m_pic);
}


BEGIN_MESSAGE_MAP(ScreenshotDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ScreenshotDlg::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// ScreenshotDlg 消息处理程序


void ScreenshotDlg::OnBnClickedButton1()
{
	showImage();
	// TODO: 在此添加控件通知处理程序代码
}


void ScreenshotDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	showImage();
	Utils::_remotescreenSend = false;
}

extern bool remoteControl;
void ScreenshotDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	remoteControl = false;
	CDialogEx::OnClose();
}
