// ChatPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatPanel.h"
#include "afxdialogex.h"
#include "AddFriendDlg.h"
#include "boost/bind.hpp"
#include "SocketInstance.h"
#include "stdio.h"
#include "SysMessageDlg.h"
#include "Utils.h"
#include "UserDomain.h"
#include "ChatDlg.h"



// ChatPanel 对话框

IMPLEMENT_DYNAMIC(ChatPanel, CDialogEx)

ChatPanel::ChatPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	
}

ChatPanel::~ChatPanel()
{
}

BOOL ChatPanel::OnInitDialog() {
	CDialogEx::OnInitDialog();
	ModifyStyleEx(0, WS_EX_APPWINDOW);
	m_firendList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	LONG lStyle;
	lStyle = GetWindowLong(m_firendList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_firendList.m_hWnd, GWL_STYLE, lStyle);//设置style
	curIndex = 0;
	m_firendList.InsertColumn(0, "用户名", LVCFMT_LEFT, 60);
	m_firendList.InsertColumn(1, "状态", LVCFMT_LEFT, 50);
	m_firendList.InsertColumn(2, "未读消息", LVCFMT_LEFT, 50);
	m_firendList.InsertColumn(3, "主机IP", LVCFMT_LEFT, 100);
	m_firendList.InsertColumn(4, "权限", LVCFMT_LEFT, 45);
	//int nRow = m_firendList.InsertItem(0, "tom");//插入行
	//m_firendList.SetItemText(nRow, 1, "online");//设置数据

	SocketInstance::getInstance()->setAddFriendRequestCallback(boost::bind(&ChatPanel::onAddFriendRequest,this,_1));
	SocketInstance::getInstance()->setAddFriendRefuseCallback(boost::bind(&ChatPanel::onAddFriendRefuse,this,_1));
	_curuserEdit.SetWindowTextA(SocketInstance::curuser.c_str());
	SocketInstance::getInstance()->setAddFriendSuccessNotifyCallback(boost::bind(&ChatPanel::onAddFriendSuccessNotify,this,_1));
	SocketInstance::getInstance()->setUpdateFriendListCallback(boost::bind(&ChatPanel::onUpdateFriendList,this));
	SocketInstance::getInstance()->setFriendOnlineCallback(boost::bind(&ChatPanel::onFriendOnline,this,_1));
	SocketInstance::getInstance()->setFriendOffLineCallback(boost::bind(&ChatPanel::onFriendOffline, this, _1));
	SocketInstance::getInstance()->setNewMessageComingCallback(boost::bind(&ChatPanel::onNewMessgeComing,this,_1));
	Utils::sendMsg("13:req frined list");
	return TRUE;
}

void ChatPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_firendList);
	DDX_Control(pDX, IDC_BUTTON3, _msgBtn);
	DDX_Control(pDX, IDC_EDIT1, _curuserEdit);
}


BEGIN_MESSAGE_MAP(ChatPanel, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &ChatPanel::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &ChatPanel::OnBnClickedButton3)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &ChatPanel::OnNMDblclkList1)
END_MESSAGE_MAP()


// ChatPanel 消息处理程序


void ChatPanel::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//AfxMessageBox("close");
	exit(0);
	CDialogEx::OnClose();
}


void ChatPanel::OnBnClickedButton1()
{
	_addfriendDlg = new AddFriendDlg();
	_addfriendDlg->Create(IDD_DIALOG3,this);
	_addfriendDlg->ShowWindow(SW_SHOW);

}

void ChatPanel::onAddFriendRequest(const char *msg) {
	char buffer[128] = { 0 };
	sprintf_s(buffer, "%s 请求添加好友！", msg);
	addSysMessage(buffer);
}


void ChatPanel::OnBnClickedButton3()
{
	if (messageList.size() == 0) {
		return;
	}
	_msgDlg = new SysMessageDlg();
	_msgDlg->Create(IDD_DIALOG4, this);
	char buffer[128] = { 0 };
	sprintf_s(buffer, "%s", messageList[0].c_str());
	if (messageList[0].find("-") < messageList[0].size()) {
		_msgDlg->_accbtn.ShowWindow(SW_HIDE);
		_msgDlg->_refbtn.ShowWindow(SW_HIDE);
	}
	_msgDlg->_msg.SetWindowText(buffer);
	_msgDlg->_username = messageList[0].substr(0,messageList[0].find(" "));
	messageList.erase(messageList.begin());
	_msgDlg->ShowWindow(SW_SHOW);
	int size = messageList.size();
	if (size == 0) {
		_msgBtn.SetWindowTextA("系统消息");
	}
	else {
		char buffer[32] = { 0 };
		sprintf_s(buffer,"系统消息(%d)",size);
		_msgBtn.SetWindowTextA((buffer));
	}
}
void ChatPanel::onAddFriendRefuse(const char* username) {
	char buffer[128] = { 0 };
	sprintf_s(buffer, "%s-拒绝添加好友", username);
	addSysMessage(buffer);
}

void ChatPanel::addSysMessage(const char* msg) {
	char buffer[128] = { 0 };
	messageList.push_back(string(msg));
	int size = messageList.size();
	memset(buffer, 0, sizeof(buffer));
	sprintf_s(buffer, "系统消息(%d)", size);
	_msgBtn.SetWindowTextA(buffer);
}

void ChatPanel::onAddFriendSuccessNotify(const char* username) {
	char buffer[128] = { 0 };
	sprintf_s(buffer, "%s-同意添加好友", username);
	addSysMessage(buffer);
}

void ChatPanel::onUpdateFriendList() {
	m_firendList.DeleteAllItems();
	char buff[128] = { 0 };
	sprintf_s(buff, "%d", Utils::friendList->size());
	vector<UserDomain>::iterator iter;
	for (iter = Utils::friendList->begin(); iter != Utils::friendList->end(); ++iter) {
		int nRow = m_firendList.InsertItem(curIndex, (*iter).username.c_str());//插入行
		Utils::friendRow->insert(make_pair((*iter).username,nRow));
		Utils::rowFriend->insert(make_pair(nRow,(*iter).username));
		m_firendList.SetItemText(nRow, 1, (strcmp((*iter).online.c_str(),"1")==0)?"在线":"离线");
		if (strcmp((*iter).online.c_str(), "1") == 0) {
			m_firendList.SetItemText(nRow, 3,(*iter).ip.c_str());
		}
		m_firendList.SetItemText(nRow, 4, (*iter).level.c_str());
		m_firendList.SetItemText(nRow, 2, "0");
		curIndex++;
		//AfxMessageBox((*iter).username.c_str());
	}
}

void ChatPanel::onFriendOnline(const char* username) {
	char buffer[128] = { 0 };
	map<string, int>::iterator iter = Utils::friendRow->find(string(username));
	if (iter == Utils::friendRow->end()) {
		AfxMessageBox("not found");
		return;
	}
	int row = iter->second;
	m_firendList.SetItemText(row,1, "在线");
	vector<UserDomain>::iterator iterList;
	for (iterList = Utils::friendList->begin(); iterList != Utils::friendList->end(); ++iterList) {
		if ((*iterList).username == string(username)) {
			m_firendList.SetItemText(row, 3, (*iterList).ip.c_str());
		}
	}
}

void ChatPanel::onFriendOffline(const char* username) {
	char buffer[128] = { 0 };
	map<string, int>::iterator iter = Utils::friendRow->find(string(username));
	if (iter == Utils::friendRow->end()) {
		AfxMessageBox("not found");
		return;
	}
	int row = iter->second;
	m_firendList.SetItemText(row,1, "离线");
}


void ChatPanel::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox("double click");
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		int row = pNMListView->iItem;
		m_firendList.SetItemText(row, 2, "0");
		map<int, string>::iterator iter;
		iter = Utils::rowFriend->find(row);
		if (iter != Utils::rowFriend->end()) {
			map<string, UserDomain>::iterator iter2;
			iter2 = Utils::friendMap->find(iter->second);
			if (iter2 != Utils::friendMap->end()) {
				//UserDomain domain = (iter2->second);
				if ((iter2->second).dlg == NULL) {
					//AfxMessageBox("创建对话框");
					(iter2->second).dlg = new ChatDlg();
					(iter2->second).dlg->Create(IDD_DIALOG5,this);
					(iter2->second).dlg->username = (iter2->second).username;
					(iter2->second).dlg->updateName();
				}
				(iter2->second).dlg->ShowWindow(SW_SHOW);
				(iter2->second).dlg->updateMessage();
			}
		}
	}
	*pResult = 0;
}

void ChatPanel::onNewMessgeComing(const char* username) {
	map<string, UserDomain>::iterator friendMapIter;
	friendMapIter = Utils::friendMap->find(username);
	if (friendMapIter == Utils::friendMap->end()) {
		return;
	}
	if ((friendMapIter->second).dlg == NULL || !(friendMapIter->second).dlg->IsWindowVisible()) {
		//AfxMessageBox("add to box");
		map<string, int>::iterator friendRowIter;
		friendRowIter = Utils::friendRow->find(username);
		if (friendRowIter == Utils::friendRow->end()) {
			return;
		}
		int row = friendRowIter->second;
		map<string, vector<string>>::iterator unreadMsgIter;
		unreadMsgIter = Utils::unreadMsg->find(username);
		if (unreadMsgIter == Utils::unreadMsg->end()) {
			return;
		}
		int msgCount = (unreadMsgIter->second).size();
		char msgCountStr[16] = { 0 };
		sprintf_s(msgCountStr, "%d", msgCount);
		m_firendList.SetItemText(row, 2,msgCountStr);
	}
	else {
		(friendMapIter->second).dlg->updateMessage();
		//AfxMessageBox("add to dlg");
	}
}
