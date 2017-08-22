// ScreenshotDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ScreenshotDlg.h"
#include "afxdialogex.h"
#include "Utils.h"
#include "ImageReceiveHelper.h"


// ScreenshotDlg �Ի���

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
	//����·������ͼƬ  
	//imageTemp.Load("C:\\image.jpg");
	

	CImage image;
	Utils::CreateStretchImage(_image, &image, 900,1400);
//	DeleteFile("image.jpg");
	//��ȡͼƬ�Ŀ� �߶�  
	cx = image.GetWidth();
	cy = image.GetHeight();
	char info[128] = { 0 };
	//sprintf_s(info,"cx:%d,cy:%d",cx,cy);
	//AfxMessageBox(info);
	//��ȡPicture Control�ؼ��Ĵ�С  
	GetDlgItem(IDC_SCSHOOT_STATIC)->GetWindowRect(&rect);
	//���ͻ���ѡ�е��ؼ���ʾ�ľ���������  
	ScreenToClient(&rect);
	//�����ƶ����ؼ���ʾ������  
	GetDlgItem(IDC_SCSHOOT_STATIC)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_SCSHOOT_STATIC);//��ȡ�ؼ����  
	pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С  

	CDC *pDc = NULL;
	pDc = pWnd->GetDC();//��ȡpicture��DC  

	image.Draw(pDc->m_hDC, rect);//��ͼƬ���Ƶ�picture��ʾ��������  

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


// ScreenshotDlg ��Ϣ�������


void ScreenshotDlg::OnBnClickedButton1()
{
	showImage();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void ScreenshotDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	showImage();
	Utils::_remotescreenSend = false;
}

extern bool remoteControl;
void ScreenshotDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	remoteControl = false;
	CDialogEx::OnClose();
}
