#include "ChatServer.h"
#include "MessageDispatcher.h"
#include "UserDomain.h"
#include "Utils.h"
#include "FriendDao.h"
#include "UserDao.h"
#include <muduo/base/Thread.h>
#include <stdio.h>
#include <pthread.h>

bool sendScreenFrameNotify = true;

ChatServer::ChatServer(net::EventLoop *loop,net::InetAddress &addr):
	_loop(loop),
	_server(new net::TcpServer(loop,addr,"ChatServer")),
	_dispatcher(new MessageDispatcher()),
	_isReading(false),
	_size(-1),
	_onlineList()
{
	_server->setConnectionCallback(boost::bind(&ChatServer::onConnection,this,_1));
	_server->setMessageCallback(boost::bind(&ChatServer::onMessage,this,_1,_2,_3));
	_server->setThreadNum(5);

	_dispatcher->setOnLoginCallback(boost::bind(&ChatServer::onLogin,this,_1,_2));
	_dispatcher->setOnAddFriendCallback(boost::bind(&ChatServer::onAddFriend,this,_1,_2));
	_dispatcher->setOnAddFriendRefuseCallback(boost::bind(&ChatServer::onAddFriendRefuse,this,_1,_2));
	_dispatcher->setOnReqFriendListCallback(boost::bind(&ChatServer::onReqFriendList,this,_1));
	_dispatcher->setOnImageServer(boost::bind(&ChatServer::onImageServer,this,_1));
	_dispatcher->setRemoteScreenReqCallback(boost::bind(&ChatServer::onRemoteScreenReq,this,_1,_2));
	Utils::onlineList = &_onlineList;
	//TimerId runEvery(double interval, const TimerCallback& cb);
	_loop->runEvery(10.0,boost::bind(&ChatServer::onTimer,this));
	imageServer = new net::TcpServer(_loop,net::InetAddress(9001),"ImageServer");
	imageServer->setConnectionCallback(boost::bind(&ChatServer::onImageConnection,this,_1));
	imageFromServer = new net::TcpServer(_loop,net::InetAddress(9002),"ImageFromServer");
	imageFromServer->setMessageCallback(boost::bind(&ChatServer::onImageFromMessage,this,_1,_2,_3));
	imageFromServer->setConnectionCallback(boost::bind(&ChatServer::onImageFromConnection,this,_1));
	imageFromSize = 0;

	screenFrameServer = new net::TcpServer(_loop,net::InetAddress(9003),"ScreenFrameServer");
	screenFrameServer->setConnectionCallback(boost::bind(&ChatServer::onScreenFrameConnection,this,_1));
	screenFrameServer->setMessageCallback(boost::bind(&ChatServer::onScreenFrameMesesage,this,_1,_2,_3));
	screenFrameSize = 0;

	screenFrameToServer = new net::TcpServer(_loop,net::InetAddress(9004),"ScreenFrameToServer");
	screenFrameToServer->setConnectionCallback(boost::bind(&ChatServer::onScreenFrameToConnection,this,_1));
	screenFrameToServer->setMessageCallback(boost::bind(&ChatServer::onScreenFrameToMesesage,this,_1,_2,_3));

}

void ChatServer::onScreenFrameToConnection(const net::TcpConnectionPtr &conn){
	if(conn->connected()){
		cout<<"**************ScreenFrameServer Connected"<<endl;
	//	screenFrameToConnectionCallback(conn);
		
	}
}
struct ThreadFuncAttr
{
	muduo::string* senderName;
	muduo::string* targetName;
	net::TcpConnectionPtr conn;
};

void* threadFunc(void* data){
	ThreadFuncAttr* attr = (ThreadFuncAttr*)data;
	map<muduo::string,queue<muduo::string> >::iterator iter;
	iter = Utils::screenFrame->find(*(attr->targetName));
	net::TcpConnectionPtr senderConn = Utils::findConnByUsername((attr->senderName)->c_str());
	if(!senderConn){
		cout<<"Sender is no online"<<endl;
		return NULL;
	}
	UserDomain *senderDomain = Utils::getUserDomainFromConn(senderConn);
	if(senderDomain == NULL){
		cout<<"Sender Domain is NULL"<<endl;
		return NULL;
	}
	cout<<"TargetName:"<<*(attr->targetName)<<endl;
	cout<<"SenderName:"<<*(attr->senderName)<<endl;
	if(iter == Utils::screenFrame->end()){
		cout<<"Queue == NULL"<<endl;
		return NULL;
	}
	queue<muduo::string> &q = iter->second;
	while(senderDomain->threadisRunning){
		while(!q.empty()){
			muduo::string data = q.front();
			q.pop();
			int size = data.size();
			attr->conn->send(&size,sizeof(size));
			attr->conn->send(data.c_str(),data.size());
			cout<<"Send to Data (Pop Queue) queue size:"<<q.size()<<endl;
		}	
	}
	
	cout<<"---ForceClose Sender Data Connection"<<endl;
	attr->conn->shutdown();
	return NULL;
}
void ChatServer::onScreenFrameToMesesage(const net::TcpConnectionPtr &conn,net::Buffer *buffer,
					Timestamp time)
{
	muduo::string data = buffer->retrieveAllAsString();
	ThreadFuncAttr *attr = new ThreadFuncAttr();
	attr->senderName = new muduo::string(data.substr(0,data.find(":")));
	attr->targetName = new muduo::string(data.substr(data.find(":")+1,data.size()));
	attr->conn = conn;
	
	pthread_t tid;
	pthread_create(&tid,NULL,&threadFunc,attr);
}

void ChatServer::onScreenFrameConnection(const net::TcpConnectionPtr &conn){
	if(conn->connected()){
	//	cout<<"---ScreenFrame connected"<<endl;
		//_remoteSCMap.insert();
	}
}
void ChatServer::onScreenFrameMesesage(const net::TcpConnectionPtr &conn,net::Buffer *buffer,
					Timestamp time)
{
	///cout<<"---ScreenFrame is Comming"<<endl;
	muduo::string data;
	if(screenFrameSize == 0){
		int size = buffer->readInt32();
	//	cout<<"---ScreenFrame comming size:"<<size<<" bytes"<<endl;
		if(screenFrameSize > 100*1024){
			buffer->retrieveAllAsString();
			return;
		}
		screenFrameSize = size;
		if(buffer->readableBytes() >= screenFrameSize){
			data = buffer->retrieveAsString(screenFrameSize);
			screenFrameSize = 0;
		}
	}else{
		if(buffer->readableBytes() >= screenFrameSize){
			data =  buffer->retrieveAsString(screenFrameSize);
			screenFrameSize = 0;
		}
	}
	if(screenFrameSize == 0){
		muduo::string username = data.substr(0,data.find(":"));
		muduo::string jpgData = data.substr(data.find(":")+1,data.size());
	//	FILE *pFile=fopen("small.jpg","wb");
	//	fwrite(jpgData.c_str(),jpgData.size(),1,pFile);
	//	fclose(pFile);
		//cout<<"---image save success"<<endl;
		//cout<<"username:"<<username<<endl;
		map<muduo::string,queue<muduo::string> >::iterator iter;
		iter = Utils::screenFrame->find(username);
		if(iter == Utils::screenFrame->end()){
			cout<<"not found ,insert:"<<username<<endl;
			Utils::screenFrame->insert(make_pair(username,queue<muduo::string>()));
		}
		iter = Utils::screenFrame->find(username);
		queue<muduo::string> &q = iter->second;
		if(q.size() > 30){
			return;
		}
		cout<<"*******Push Data Size:"<<data.size()<<" QueueSize:"<<q.size()<<"*******"<<endl;
		q.push(jpgData);
	}
}

void ChatServer::onRemoteScreenReq(net::TcpConnectionPtr conn,const char* username){
	cout<<"---Require RemoteScreen";
	cout<<" Target:"<<username<<endl;
	UserDomain *sender = Utils::getUserDomainFromConn(conn);
	if(sender == NULL){
		cout<<"RemoteScreen Sender == NULL"<<endl;
		return;
	}
	net::TcpConnectionPtr connTarget = Utils::findConnByUsername(username);
	if(!connTarget){
		cout<<"RemoteScreen Target Not Online"<<endl;
		return;
	}
	//if(sender->remoteScreenUsername != ""){
	//	return;
	//}
	if(sender->threadisRunning){
		return;
	}
	sender->remoteScreenUsername = muduo::string(username);
	//_loop->runEvery(0.5,boost::bind(&UserDomain::remoteScreenTimer,sender));
	sender->remoteScreenTimer();
}
void ChatServer::onImageFromConnection(const net::TcpConnectionPtr &conn){
	if(conn->connected()){
		cout<<"-----ImageFromServer connected"<<endl;
	}
	if(conn->disconnected()){
		imageFromSize = 0;
	}
}
void ChatServer::onImageFromMessage(const net::TcpConnectionPtr &conn,net::Buffer *buffer,
					Timestamp time){
	cout<<"******************image is comming**********************"<<endl;
	muduo::string data;
	if(imageFromSize == 0){
		imageFromSize = buffer->readInt32();
		if(imageFromSize > 500*1024){
			buffer->retrieveAllAsString();
			imageFromSize = 0;
			return;
		}
		cout<<imageFromSize<<" bytes"<<endl;
		if(buffer->readableBytes() >= imageFromSize){
			data = buffer->retrieveAsString(imageFromSize);
			imageFromSize = 0;
			cout<<"******************image has stored**********************"<<endl;
			int index = data.find(":");
			muduo::string codestr = data.substr(0,index);
			int code = atoi(codestr.c_str());
			data = data.substr(index+1,data.size()-index-1);
			Message msg;
			msg.code = code;
			msg.data = data;
			_dispatcher->do_imageFrom(conn,msg);
		}
	}else{
		if(buffer->readableBytes() >= imageFromSize){
			data = buffer->retrieveAsString(imageFromSize);
			imageFromSize = 0;	
			cout<<"******************image has stored**********************"<<endl;
			int index = data.find(":");
			muduo::string codestr = data.substr(0,index);
			int code = atoi(codestr.c_str());
			data = data.substr(index+1,data.size()-index-1);
			Message msg;
			msg.code = code;
			msg.data = data;
			_dispatcher->do_imageFrom(conn,msg);
		}
	}
}

ChatServer::~ChatServer(){
	delete _dispatcher;
	delete _server;
}

void ChatServer::start(){
	_server->start();
	imageFromServer->start();
	imageServer->start();
	screenFrameServer->start();
	screenFrameToServer->start();
}

void ChatServer::onConnection(const net::TcpConnectionPtr &conn){
	if(conn->connected()){
		cout<<"---"<<conn->peerAddress().toIpPort()<<" connected"<<endl;
	}
	if(conn->disconnected()){
		cout<<"---"<<conn->peerAddress().toIpPort()<<" disconnected"<<endl;
		if(!(conn->getContext()).empty()){
			UserDomain *user = boost::any_cast<UserDomain*>(conn->getContext());
			user->setThreadIsRunning(false);
			map<std::string,net::TcpConnectionPtr>::iterator iter;
			iter = _onlineList.find(std::string(user->getUsername()));
			if(iter != _onlineList.end()){
				_onlineList.erase(iter);
				cout<<"online number:"<<_onlineList.size()<<endl;
			}
		}
		_dispatcher->dispatch(conn,muduo::string("2:client has disconnected"));
		notifyFriendsOnlineStat(Utils::getUsernameFromConn(conn),false);
	}
}
void ChatServer::onMessage(const net::TcpConnectionPtr &conn,
							net::Buffer *buffer,
							Timestamp time)
{	
	muduo::string data;
	cout<<"---onMessage"<<endl;
	cout<<"isReading:"<<_isReading<<endl;
	cout<<"---need read bytes: "<<_size<<endl;
	int readableBytes = buffer->readableBytes();
	cout<<"****readable bytes : "<<readableBytes<<endl;
	if(!_isReading){
		if(buffer->readableBytes() < sizeof(uint32_t)){
			return;
		}
		uint32_t size = buffer->readInt32();
		if(size > 1024){
			buffer->retrieveAllAsString();
			return;
		}
		cout<<"---header:size:"<<size<<endl;
		cout<<"******read "<<size<<" bytes"<<endl;
		_size = size;
		if(buffer->readableBytes() >= size){		
			data = buffer->retrieveAsString(size);
			cout<<"---recieve size:"<<data.size()<<" data:"<<data.c_str()<<endl;
			cout<<"*****read "<<data.size()<<" bytes"<<endl;
			_dispatcher->dispatch(conn,data);
		}else{
			_isReading = true;
		}
	}else{
		if(buffer->readableBytes() >= _size){
			data = buffer->retrieveAsString(_size);
			cout<<"---recieve size:"<<data.size()<<" data:"<<data.c_str()<<endl;
			cout<<"*****read "<<data.size()<<" bytes"<<endl;
			_dispatcher->dispatch(conn,data);
			_isReading = false;
			_size = -1;
		}
	}
	
	
}

void ChatServer::onLogin(const char* username,net::TcpConnectionPtr conn){
	cout<<"User:["<<username<<"]"<<"is onLine"<<endl;
	_onlineList.insert(make_pair(std::string(username),conn));
	cout<<"online number:"<<_onlineList.size()<<endl;
	//sendFriendList(conn);
	//screenFrameToServer->setConnectionCallback(boost::bind(&UserDomain::onScreenFrameToConnection,Utils::getUserDomainFromConn(conn),_1));
	screenFrameToConnectionCallback = boost::bind(&UserDomain::onScreenFrameToConnection,Utils::getUserDomainFromConn(conn),_1);
	notifyFriendsOnlineStat(username,true);
}

void ChatServer::onAddFriend(net::TcpConnectionPtr conn,const char* username){
	if((conn->getContext()).empty()){
		return;
	}
	UserDomain *user = boost::any_cast<UserDomain*>(conn->getContext());
	if(strcmp(user->getUsername(),username) == 0){
		return;
	}
	map<std::string,net::TcpConnectionPtr>::iterator iter;
	iter = _onlineList.find(std::string(username));
	if(iter == _onlineList.end()){
		cout<<"---friend not online"<<endl;
		char buffer[1024] = {0};
		sprintf(buffer,"4:%s",username);
		conn->send(buffer);
	}
	else{
		
		cout<<"---friend online"<<endl;
		char buffer[1024] = {0};
		sprintf(buffer,"5:%s",username);
		conn->send(buffer);
		net::TcpConnectionPtr friendPtr = iter->second;
		memset(buffer,0,sizeof(buffer));
		sprintf(buffer,"6:%s",user->getUsername());
		friendPtr->send(buffer);	
	}
}

void ChatServer::onAddFriendRefuse(net::TcpConnectionPtr conn,const char* username){
	
	map<std::string,net::TcpConnectionPtr>::iterator iter;
	iter = _onlineList.find(std::string(username));
	if(iter!= _onlineList.end()){
		net::TcpConnectionPtr connsend = iter->second;
		char buffer[128] = {0};
		sprintf(buffer,"10:%s",Utils::getUsernameFromConn(conn));
		connsend->send(buffer);
		cout<<"refuse:"<<buffer<<endl;
	}
}

void ChatServer::sendFriendList(net::TcpConnectionPtr conn){
	vector<std::string> v;
	getFriendByUsername(&v,Utils::getUsernameFromConn(conn));
	UserDao userDao;
	if(v.size() == 0){
		return;
	}
	vector<std::string>::iterator iter;
	std::string result;
	for(iter = v.begin();iter!=v.end();++iter){
		UserDomain userDomain;
		if(userDao.QueryByUsername(&userDomain,(*iter).c_str()) == -1){
			result+=":";
			result+=userDomain.getUsername();
			result+=",";
			
			result+=userDomain.getLevel();
			result+=",";
			
			if(isOnline(userDomain.getUsername())){
				result+="1";
				net::TcpConnectionPtr conn = Utils::findConnByUsername(userDomain.getUsername());
				if(conn){
					result+=",";
					result+=std::string(conn->peerAddress().toIp().c_str());
				}
			}else{
				result+="0";
			}
		}
	}
	//cout<<result<<endl;
	result = result.substr(1,result.size());
	char buffer[1024] ={0};
	sprintf(buffer,"12:%s",result.c_str());
	cout<<buffer<<endl;
	conn->send(buffer);
}

void ChatServer::onReqFriendList(net::TcpConnectionPtr conn){
	sendFriendList(conn);
}

bool ChatServer::isOnline(const char* username){
	map<std::string,net::TcpConnectionPtr>::iterator iter;
	iter = _onlineList.find(std::string(username));
	if(iter!=_onlineList.end()){
		return true;
	}
	return false; 
}

void ChatServer::getFriendByUsername(vector<std::string> *v,const char* username){
	FriendDao dao;
	dao.query(v,username,1);
	dao.query(v,username,2);
}

void ChatServer::notifyFriendsOnlineStat(const char* username,bool stat){
	vector<std::string> v;
	getFriendByUsername(&v,username);
	if(v.size()==0){
		return;
	}
	//cout<<"has friend"<<endl;
	vector<std::string>::iterator iter;
	for(iter = v.begin();iter != v.end(); ++iter){
		net::TcpConnectionPtr conn = Utils::findConnByUsername((*iter).c_str());
		if(!conn){
			continue;
		}
		char buffer[128] = {0};
		if(stat){
			sprintf(buffer,"14:%s:%s",username,conn->peerAddress().toIp().c_str());
		}
		else{
			sprintf(buffer,"15:%s",username);
		}
		cout<<"->notify:"<<buffer<<endl;
		conn->send(buffer);
	}
}

void ChatServer::onTimer(){
	keepOnline();
}
void ChatServer::keepOnline(){
	map<std::string,net::TcpConnectionPtr>::iterator iter;
	for(iter = _onlineList.begin();iter != _onlineList.end();++iter){
		iter->second->send("16:keep online");
	}
}

void ChatServer::onImageServer(muduo::string username){
	//cout<<"---start image server"<<endl;
	imageRecvUsername = username;
//	imageServer->start();
}

void ChatServer::onImageConnection(const net::TcpConnectionPtr &conn){
	//cout<<"image username"<<imageRecvUsername<<endl;
	if(conn->connected()){
		UserDomain *userDomain = Utils::getUserDomainFromConn(Utils::findConnByUsername(imageRecvUsername.c_str()));
		cout<<"---image req connected"<<endl;
		cout<<(userDomain->getImagedata()).size()<<endl;
		conn->send(userDomain->getImagedata().c_str(),(userDomain->getImagedata()).size());
	}
	if(conn->disconnected()){
		cout<<"---image req disconnected"<<endl;
	}
}