#ifndef _CHATSERVER_H_
#define _CHATSERVER_H_
#include "Commons.h"
#include <muduo/net/TcpConnection.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>
#include <boost/function.hpp>

class MessageDispatcher;
class ChatServer
{
public:
	ChatServer(net::EventLoop* loop,net::InetAddress &addr);
	~ChatServer();
	void start();
private:
	net::EventLoop *_loop;
	net::TcpServer *_server;
	void onConnection(const net::TcpConnectionPtr &conn);
	void onMessage(const net::TcpConnectionPtr &conn,
					net::Buffer *buffer,
					Timestamp time);
	MessageDispatcher *_dispatcher;
	bool _isReading;
	uint32_t _size;
	map<std::string,net::TcpConnectionPtr> _onlineList;
	map<muduo::string,net::TcpConnectionPtr> _remoteSCMap;
	void onLogin(const char* username,net::TcpConnectionPtr conn);
	void onAddFriend(net::TcpConnectionPtr conn,const char* username);
	void onAddFriendRefuse(net::TcpConnectionPtr conn,const char* username);
	void sendFriendList(net::TcpConnectionPtr conn);
	void onReqFriendList(net::TcpConnectionPtr conn);
	bool isOnline(const char* username);
	void getFriendByUsername(vector<std::string> *v,const char* username);
	void notifyFriendsOnlineStat(const char* username,bool stat);
	void onTimer();
	void keepOnline();
	void onImageServer(muduo::string username);
	void onImageConnection(const net::TcpConnectionPtr &conn);
	void onImageFromConnection(const net::TcpConnectionPtr &conn);
	void onImageFromMessage(const net::TcpConnectionPtr &conn,net::Buffer *buffer,
					Timestamp time);
	net::TcpServer* imageServer;
	net::TcpServer* imageFromServer;
	net::TcpServer* screenFrameServer;
	uint32_t imageFromSize;
	muduo::string imageRecvUsername;
	void onRemoteScreenReq(net::TcpConnectionPtr conn,const char* username);
	void onScreenFrameConnection(const net::TcpConnectionPtr &conn);
	void onScreenFrameMesesage(const net::TcpConnectionPtr &conn,net::Buffer *buffer,
					Timestamp time);
	uint32_t screenFrameSize;
	net::TcpServer* screenFrameToServer;
	void onScreenFrameToConnection(const net::TcpConnectionPtr &conn);
	void onScreenFrameToMesesage(const net::TcpConnectionPtr &conn,net::Buffer *buffer,
					Timestamp time);
public:
	boost::function<void(net::TcpConnectionPtr)> screenFrameToConnectionCallback;
};

#endif