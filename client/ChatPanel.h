#pragma once
#include "afxcmn.h"
#include "AddFriendDlg.h"
#include <vector>
#include <string>
#include "afxwin.h"
#include "SysMessageDlg.h"
using namespace std;


// ChatPanel 对话框

class ChatPanel : public CDialogEx
{
	DECLARE_DYNAMIC(ChatPanel)

public:
	ChatPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ChatPanel();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	CListCtrl m_firendList;
	afx_msg void OnBnClickedButton1();
	AddFriendDlg *_addfriendDlg;	
	void onAddFriendRequest(const char *msg);
	vector<string> messageList;
	afx_msg void OnBnClickedButton3();
	CButton _msgBtn;
	SysMessageDlg *_msgDlg;
	std::string curuser;
	CEdit _curuserEdit;
	void onAddFriendRefuse(const char* username);
	void addSysMessage(const char* msg);
	void onAddFriendSuccessNotify(const char* username);
	void onUpdateFriendList();
	void onFriendOnline(const char* username);
	void onFriendOffline(const char* username);
	int curIndex;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	void onNewMessgeComing(const char* username);
};


