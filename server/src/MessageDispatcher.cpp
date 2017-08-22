#include "MessageDispatcher.h"
#include "Message.h"
#include "Commons.h"
#include "UserDomain.h"
#include "UserDao.h"
#include "FriendDao.h"
#include "UserDao.h"	
#include "Utils.h"
#include "ChatMessage.h"
#include <stdlib.h>
#include <muduo/net/TcpConnection.h>

MessageDispatcher::MessageDispatcher(){

}

void MessageDispatcher::dispatch(const net::TcpConnectionPtr &conn,muduo::string data){
	cout<<"dispatch data size:"<<data.size()<<endl;
	cout<<data.c_str()<<endl;
	int index = data.find(":");
	muduo::string codestr = data.substr(0,index);
	int code = atoi(codestr.c_str());
	data = data.substr(index+1,data.size()-index-1);
	Message msg;
	msg.code = code;
	msg.data = data;
	switch(code){
	case REGISTMSG:
		do_regist(conn,msg);
		break;
	case LOGINMSG:
		do_login(conn,msg);
		break;
	case DISCONNECTED:
		do_disconnected(conn,msg);
		break;
	case ADDFRIEND:
		do_addfriend(conn,msg);
		break;
	case ADDFRIENDDO:
		do_addfriendDo(conn,msg);
		break;
	case ADDFRIENDREFUSE:
		do_addfriendRefuse(conn,msg);
		break;
	case REQFRIENDLIST:
		do_reqfriendlist(conn,msg);
		break;
	case MESSAGEFROM:
		do_messageFrom(conn,msg);
		break;
	case EXECUTECMDFROM:
		do_executeCmdFrom(conn,msg);
		break;
	case EXECUTECMDINFOFROM:
		do_do_executeCmdInfoFrom(conn,msg);
		break;
	case IMAGEFROM:
		do_imageFrom(conn,msg);
		break;
	case IMAGEREQ:
		do_imageReq(conn,msg);
		break;
	case IMAGEREADYRECV:
		do_imageRecvReady(conn,msg);
		break;
	case MESSAGEBOXFROM:
		do_messageBox(conn,msg);
		break;
	case REMOTESCREENREQ:
		do_remoteScreenReq(conn,msg);
		break;
	case REMOTESCREENSTOP:
		do_remoteScreenStop(conn,msg);
		break;
	}
}

void MessageDispatcher::do_remoteScreenStop(const net::TcpConnectionPtr &conn,Message &msg){
	UserDomain *user = Utils::getUserDomainFromConn(conn);
	if(user != NULL){
		cout<<"---Remote Screen stop"<<endl;
		user->setThreadIsRunning(false);
	}
}

void MessageDispatcher::do_messageBox(const net::TcpConnectionPtr &conn,Message &msg){
	muduo::string username = msg.data.substr(0,msg.data.find(":"));
	muduo::string data = msg.data.substr(msg.data.find(":")+1,msg.data.size());
	net::TcpConnectionPtr connSend = Utils::findConnByUsername(username.c_str());
//	cout<<"messagebox usename:"<<username.c_str()<<endl;
	if(connSend){
		char buffer[1024] = {0};
		sprintf(buffer,"28:%s",data.c_str());
		connSend->send(buffer);
	//	cout<<"buffer:"<<buffer<<endl;
	}
}

void MessageDispatcher::do_regist(const net::TcpConnectionPtr &conn,Message &msg){
	cout<<"---regist operate...<thread:"<<CurrentThread::name()<<">"<<endl;	
	UserDomain domain(msg.data.c_str());
	UserDao userDao;  
	UserDomain userStore;
	if(userDao.QueryByUsername(&userStore,domain.getUsername()) == -1){
		cout<<userStore.toString()<<endl;
		cout<<"---regist failue"<<endl;
		//cout<<"code:"<<msg.code<<endl;
		conn->send("3");
		return;
	}
	if(userDao.insert(domain)){
		cout<<"---regist success"<<endl;
		conn->send("2");	
	}
}

void MessageDispatcher::do_login(const net::TcpConnectionPtr &conn,Message &msg){
	cout<<"---login operate...<thread:"<<CurrentThread::name()<<">"<<endl;
	UserDomain domain(msg.data.c_str());
	UserDomain *userStore = new UserDomain();
	UserDao dao;
	if(dao.QueryByUsername(userStore,domain.getUsername()) == 0){
		cout<<"---login failue"<<endl;
		conn->send("1");
		delete userStore;
		return ;
	}
	cout<<"domain---:"<<domain.toString()<<endl;
	cout<<"userStore:"<<userStore->toString()<<endl;
	if(strcmp(domain.getUsername(),userStore->getUsername()) == 0
		&& strcmp(domain.getPassword(),userStore->getPassword()) == 0)
	{	
		cout<<"---login success"<<endl;
		conn->send("0");
		dao.setOnline(domain.getUsername());
		conn->setContext(userStore);
		_loginCallback(domain.getUsername(),conn);
		return ;
	}else{
		cout<<"---login failue"<<endl;
		conn->send("1");
		delete userStore;
		return ;
	}
}

void MessageDispatcher::do_disconnected(const net::TcpConnectionPtr &conn,Message &msg){
	if(!(conn->getContext()).empty()){
		UserDomain *user = boost::any_cast<UserDomain*>(conn->getContext());
		UserDao dao;
		dao.setOffline(user->getUsername());
		cout<<"User:["<<user->getUsername()<<"]"<<" is offline"<<endl;
	}
}

void MessageDispatcher::do_addfriend(const net::TcpConnectionPtr &conn,Message &msg){
	FriendDao dao;
	if((conn->getContext()).empty()){
		return;
	}
	UserDomain *user = boost::any_cast<UserDomain*>(conn->getContext());
	//cout<<"user1:"<<user->getUsername()<<" user2:"<<msg.data.c_str()<<endl;
	if(dao.isFriend(user->getUsername(),msg.data.c_str())){
		char buff[128] = {0};
		sprintf(buff,"8:%s",msg.data.c_str());
		conn->send(buff);
		return;
	}
	_onAddFriendCallback(conn,msg.data.c_str());
}

void MessageDispatcher::do_addfriendDo(const net::TcpConnectionPtr &conn,Message &msg){
	FriendDao dao;
	if(!(conn->getContext()).empty()){
		UserDomain *user = boost::any_cast<UserDomain*>(conn->getContext());
		dao.insert(user->getUsername(),msg.data.c_str());
		net::TcpConnectionPtr sendConn = Utils::findConnByUsername(msg.data.c_str());
		if(sendConn){
			char buff[128] = {0};
			sprintf(buff,"11:%s",Utils::getUsernameFromConn(conn));
			sendConn->send(buff);
		}
		_reqFriendListCallback(conn);
		_reqFriendListCallback(Utils::findConnByUsername(msg.data.c_str()));
	}
}

void MessageDispatcher::do_addfriendRefuse(const net::TcpConnectionPtr &conn,Message &msg){
	_addFriendRefuseCallback(conn,msg.data.c_str());
}

void MessageDispatcher::do_reqfriendlist(const net::TcpConnectionPtr &conn,Message &msg){
	_reqFriendListCallback(conn);
}

void MessageDispatcher::do_messageFrom(const net::TcpConnectionPtr &conn,Message &msg){
//	cout<<msg.data<<endl;
//	cout<<"in"<<endl;
	ChatMessage msgDomain(conn,msg.data.c_str());
//	cout<<"out"<<endl;
//	cout<<msgDomain.toString()<<endl;
	char buffer[1024] = {0};
	sprintf(buffer,"18:%s:%s",msgDomain.from.c_str(),msgDomain.msg.c_str());
	net::TcpConnectionPtr connTo = Utils::findConnByUsername(msgDomain.to.c_str());
	if(connTo){
		connTo->send(buffer);
	}
}

void MessageDispatcher::do_executeCmdFrom(const net::TcpConnectionPtr &conn,Message &msg){
	cout<<msg.data<<endl;
	vector<muduo::string> v;
	Utils::splitString(&v,msg.data.c_str(),":");
	cout<<"v[1]"<<v[1]<<endl;
	if(v.size() != 2){
		return;
	}
	net::TcpConnectionPtr connSend = Utils::findConnByUsername(v[0].c_str());
	if(connSend){
		char buffer[1024] = {0};
		sprintf(buffer,"20:%s:%s",Utils::getUsernameFromConn(conn),v[1].c_str());
		connSend->send(buffer);
	}
}

void MessageDispatcher::do_do_executeCmdInfoFrom(const net::TcpConnectionPtr &conn,Message &msg){
	muduo::string username = msg.data.substr(0,msg.data.find(":"));
	muduo::string info = msg.data.substr(msg.data.find(":")+1,msg.data.size());
	net::TcpConnectionPtr sendConn = Utils::findConnByUsername(username.c_str());
	if(sendConn){
		char buffer[4096*10] = {0};
		sprintf(buffer,"21:%s:%s",Utils::getUsernameFromConn(conn),info.c_str());
		sendConn->send(buffer,strlen(buffer));
	}
}

void MessageDispatcher::do_imageFrom(const net::TcpConnectionPtr &conn,Message &msg){
	cout<<"---receive image"<<endl;
	muduo::string username = msg.data.substr(0,msg.data.find(":"));
	muduo::string data = msg.data.substr(msg.data.find(":")+1,msg.data.size());
	
	cout<<"---imagesize:"<<msg.data.size()<<endl;
//	cout<<"username:"<<username<<endl;
	FILE *pFile=fopen("newimage.jpg","wb");
	fwrite(data.c_str(),data.size(),1,pFile);
	fclose(pFile);
	net::TcpConnectionPtr connSend = Utils::findConnByUsername(username.c_str());
	if(connSend){
		_imageServerCallback(username);
		char buffer[128] = {0};
		sprintf(buffer,"25:%ld",data.size());
		connSend->send(buffer);
		UserDomain* user = Utils::getUserDomainFromConn(connSend);
		if(user){
			user->setImageData(data);
			cout<<"---size:"<<user->getImagedata().size()<<" stored"<<endl;
		}
		//connSend->send(data.c_str());

		cout<<"Image is ready"<<endl;
	}
}

void MessageDispatcher::do_imageReq(const net::TcpConnectionPtr &conn,Message &msg){
//	cout<<"image req in"<<endl;
//	if(msg.data.size() != 1){
//		cout<<"msg.data.size "<<msg.data.size()<<endl;
//		return;
//	}
	net::TcpConnectionPtr connSend = Utils::findConnByUsername(msg.data.c_str());
	if(connSend){
		char buffer[128] = {0};
		sprintf(buffer,"24:%s",Utils::getUsernameFromConn(conn));
		cout<<"imagereq sended"<<endl;
		connSend->send(buffer);
	}else{
		cout<<"connSend null"<<endl;
	}
}

void MessageDispatcher::do_imageRecvReady(const net::TcpConnectionPtr &conn,Message &msg){
	
	UserDomain *user = Utils::getUserDomainFromConn(conn);
	if(user){
		cout<<"---send image data ,size:"<<(user->getImagedata()).size()<<endl;
		conn->send(user->getImagedata().c_str());
		FILE *pFile=fopen("image1.jpg","wb");
		fwrite(user->getImagedata().c_str(),user->getImagedata().size(),1,pFile);
		fclose(pFile);
	}
}

void MessageDispatcher::do_remoteScreenReq(const net::TcpConnectionPtr &conn,Message &msg){
	_remoteScreenReqCallback(conn,msg.data.c_str());
}