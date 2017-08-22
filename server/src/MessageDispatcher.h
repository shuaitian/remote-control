#ifndef _MESSAGEDISPATCHER_H_
#define _MESSAGEDISPATCHER_H_
#include "Message.h"
#include <muduo/net/TcpConnection.h>
#include <boost/function.hpp>
using namespace muduo;
using namespace std;

enum MessageType{
	REGISTMSG = 0,
	LOGINMSG = 1,
	DISCONNECTED = 2,
	ADDFRIEND = 3,
	FRIENDNOTONLINE = 4,
	ADDFRIENDREQSEND = 5,
	ADDFRIENDCONFIRM = 6,
	ADDFRIENDDO = 7,
	ISFRIEND = 8,
	ADDFRIENDREFUSE = 9,
	ADDFRIENDREFUSETOCLIENT = 10,
	ADDFRIENDSUCCESSNOTIFY = 11,
	FRIENDLIST = 12,
	REQFRIENDLIST = 13,
	FRIENDONLINE = 14,
	FRIENDOFFLINE = 15,
	KEEPONLINE = 16,
	MESSAGEFROM = 17,
	MESSAGETO = 18,
	EXECUTECMDFROM = 19,
	EXCUTECMDTO = 20,
	EXECUTECMDINFOFROM = 21,
	IMAGEFROM = 22,
	IMAGEREQ = 23,
	IMAGENOTIFY = 24,
	IMAGETO = 25,
	IMAGEREADYRECV = 26,
	MESSAGEBOXFROM = 27,
	MESSAGEBOXTO = 28,
	REMOTESCREENREQ = 29,
	REMOTESCREENSTOP = 30,
	REMOTESCREENFRAMEREQ = 31,
	REMOTESCREENFRAMEFROM = 32,
	REMOTESCREENFRAMETO = 33
};

class MessageDispatcher
{
public:
	typedef boost::function<void(const char*,net::TcpConnectionPtr conn)> OnLoginCallback;
	typedef boost::function<void(const net::TcpConnectionPtr &conn,const char*username)> OnAddFriendCallback;
	MessageDispatcher();
	void dispatch(const net::TcpConnectionPtr &conn,muduo::string msg);
	void setOnLoginCallback(OnLoginCallback callback){
		_loginCallback = callback;
	}
	void setOnAddFriendCallback(OnAddFriendCallback callback){
		_onAddFriendCallback = callback;
	}
	void setOnAddFriendRefuseCallback(boost::function<void(net::TcpConnectionPtr conn,const char *)> callback){
		_addFriendRefuseCallback = callback;
	}
	void setOnReqFriendListCallback(boost::function<void(net::TcpConnectionPtr conn)> callback){
		_reqFriendListCallback = callback;
	}
	void setOnImageServer(boost::function<void(muduo::string)> callback){
		_imageServerCallback = callback;
	}
	void setRemoteScreenReqCallback(boost::function<void(net::TcpConnectionPtr conn,const char* )> callback){
		_remoteScreenReqCallback = callback;
	}
public:
	void do_regist(const net::TcpConnectionPtr &conn,Message &msg);
	void do_login(const net::TcpConnectionPtr &conn,Message &msg);
	void do_disconnected(const net::TcpConnectionPtr &conn,Message &msg);
	void do_addfriend(const net::TcpConnectionPtr &conn,Message &msg);
	void do_addfriendDo(const net::TcpConnectionPtr &conn,Message &msg);
	void do_addfriendRefuse(const net::TcpConnectionPtr &conn,Message &msg);
	void do_reqfriendlist(const net::TcpConnectionPtr &conn,Message &msg);
	void do_messageFrom(const net::TcpConnectionPtr &conn,Message &msg);
	void do_executeCmdFrom(const net::TcpConnectionPtr &conn,Message &msg);
	void do_do_executeCmdInfoFrom(const net::TcpConnectionPtr &conn,Message &msg);
	void do_imageFrom(const net::TcpConnectionPtr &conn,Message &msg);
	void do_imageReq(const net::TcpConnectionPtr &conn,Message &msg);
	void do_imageRecvReady(const net::TcpConnectionPtr &conn,Message &msg);
	void do_messageBox(const net::TcpConnectionPtr &conn,Message &msg);
	void do_remoteScreenReq(const net::TcpConnectionPtr &conn,Message &msg);
	void do_remoteScreenStop(const net::TcpConnectionPtr &conn,Message &msg);
	OnLoginCallback _loginCallback;
	OnAddFriendCallback _onAddFriendCallback;
	boost::function<void(net::TcpConnectionPtr conn,const char *)> _addFriendRefuseCallback;
	boost::function<void(net::TcpConnectionPtr conn)> _reqFriendListCallback;
	boost::function<void(muduo::string)> _imageServerCallback;
	boost::function<void(net::TcpConnectionPtr conn,const char* )>_remoteScreenReqCallback;
};

#endif