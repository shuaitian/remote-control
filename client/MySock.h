#pragma once
#include "stdafx.h"
#include "afxsock.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <vector>
#include <string>
#include "MessageShowDlg.h"
using namespace std;
class ScreenshotDlg;
class RemoteScreenDlg;
class MySock : public CAsyncSocket
{
public:
	typedef boost::function<void()> LoginSuccessCallback;
	typedef boost::function<void()> LoginFailueCallback;
	typedef boost::function<void()> RegistSuccessCallback;
	typedef boost::function<void(int)> RegistFailueCallback;
	MySock();
	~MySock();
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	void setLoginSuccessCallback(const LoginSuccessCallback& login);
	void setLoginFailueCallback(const LoginFailueCallback loginfailue);
	void setRegistSuccessCallback(const RegistSuccessCallback& login);
	void setRegistFailueCallback(const RegistFailueCallback loginfailue);
	void setFriendNotOnlineCallback(const LoginFailueCallback friendnotonline);
	void setAddFriendReqSendlineCallback(const LoginFailueCallback addfriendreqsend);
	void setAddFriendRequestCallback(boost::function<void(const char*)> addfriendrequest);
	void setIsFriendCallback(LoginFailueCallback callback);
	void setAddFriendRefuseCallback(boost::function<void(const char*)> callback) {
		addFriendRefuseCallback = callback;
	}
	void setAddFriendSuccessNotifyCallback(boost::function<void(const char*)> callback) {
		addFriendSuccessNotifyCallback = callback;
	}
	void setUpdateFriendListCallback(LoginFailueCallback callback) {
		updateFriendListCallback = callback;
	}
	void setFriendOnlineCallback(boost::function<void(const char*)> callback) {
		friendOnlineCallback = callback;	
	}
	void setFriendOffLineCallback(boost::function<void(const char*)> callback) {
		friendOfflineCallback = callback;	
	}
	void setNewMessageComingCallback(boost::function<void(const char*)> callback) {
		newMessageComingCallback = callback;
	}
private:
	LoginSuccessCallback loginSuccessCallback;
	LoginFailueCallback loginFailueCallback;
	RegistSuccessCallback registSuccessCallback;
	RegistFailueCallback registFailueCallback;
	LoginFailueCallback addfriendreqsendCallback;
	LoginFailueCallback friendnotonlineCallback;	
	boost::function<void(const char*)> addFriendRequestCallback;
	LoginFailueCallback isFriendCallback;	
	boost::function<void(const char*)> addFriendRefuseCallback;
	boost::function<void(const char*)> addFriendSuccessNotifyCallback;
	void addFriendList(vector<string> *v);
	LoginFailueCallback updateFriendListCallback;
	boost::function<void(const char*)> friendOnlineCallback;
	boost::function<void(const char*)> friendOfflineCallback;
	void handleMsg(const char* msg);
	boost::function<void(const char*)> newMessageComingCallback;
	void do_executeCmd(const char * username,const char* cmdline);
	void do_executeCmdInfo(const char* data);
	MessageShowDlg *messageShowDlg;
	void do_imageReq(const char* username);
	bool isRecvImage;
	int imageSize;
	char* do_RecvImage(const char* size);
	void do_RecvImageDO();
	void do_screentShotReq(const char* username);
	ScreenshotDlg *_screenshotDlg;
	void do_screenShotTo(const char* data);

};

