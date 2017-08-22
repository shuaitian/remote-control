// RemoteScreenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient.h"
#include "RemoteScreenDlg.h"
#include "afxdialogex.h"
#include "Utils.h"


// RemoteScreenDlg �Ի���

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


// RemoteScreenDlg ��Ϣ�������


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
	//����·������ͼƬ  
	//imageTemp.Load("C:\\image.jpg");

	//	DeleteFile("image.jpg");
	//��ȡͼƬ�Ŀ� �߶�  
	cx = image->GetWidth();
	cy = image->GetHeight();
	char info[128] = { 0 };
	//sprintf_s(info,"cx:%d,cy:%d",cx,cy);
	//AfxMessageBox(info);
	//��ȡPicture Control�ؼ��Ĵ�С  
	GetDlgItem(IDC_REMOTESC_STATIC)->GetWindowRect(&rect);
	//���ͻ���ѡ�е��ؼ���ʾ�ľ���������  
	ScreenToClient(&rect);
	//�����ƶ����ؼ���ʾ������  
	GetDlgItem(IDC_REMOTESC_STATIC)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_REMOTESC_STATIC);//��ȡ�ؼ����  
	pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С  

	CDC *pDc = NULL;
	pDc = pWnd->GetDC();//��ȡpicture��DC  

	image->Draw(pDc->m_hDC, rect);//��ͼƬ���Ƶ�picture��ʾ��������  

	ReleaseDC(pDc);
}

void RemoteScreenDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	showImage();
}


void RemoteScreenDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	char buffer[128] = { 0 };
	sprintf_s(buffer, "30:stop remotesceen");
	Utils::sendMsg(buffer);
	CDialogEx::OnClose();
}
