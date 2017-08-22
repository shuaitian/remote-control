#include "UserDomain.h"
#include "Utils.h"
#include <stdio.h>
#include <muduo/base/Thread.h>
#include <boost/bind.hpp>
#include <unistd.h>
#include <muduo/net/TcpConnection.h>
#include <queue>

extern bool sendScreenFrameNotify;

UserDomain::UserDomain(const char* data):
	_username(),
	_password(),
	_mail(),
	_imageData(),
	remoteScreenUsername()
{
	remoteScreenUsername = "";
	isSenderRecv = false;
	vector<muduo::string> v;
	Utils::splitString(&v,data,":");
	int index = 0;
	vector<muduo::string>::iterator iter;
	for(iter = v.begin();iter != v.end(); ++iter){
		switch(index){
		case 0:
			_username = *iter;
			break;
		case 1:
			_password = *iter;
			break;
		case 2:
			_mail = *iter;
			break;
		}
		index++;
	}
}

UserDomain::UserDomain():
	_username(),
	_password(),
	_mail()
{}

const char* UserDomain::getUsername(){
	return _username.c_str();
}
const char* UserDomain::getPassword(){
	return _password.c_str();
}
const char* UserDomain::getMail(){
	return _mail.c_str();
}
void UserDomain::setUsername(const char* username){
	_username = username;
}
void UserDomain::setPassword(const char* password){
	_password = password;
}
void UserDomain::setMail(const char* mail){
	_mail = mail;
}

muduo::string UserDomain::toString(){
	char buffer[512] = {0};
	sprintf(buffer,"username:%s password:%s mail:%s",_username.c_str(),_password.c_str(),_mail.c_str());
	muduo::string ret(buffer);
	return ret;
}

void UserDomain::setOnline(const char* online){
	_online = online;
}

const char* UserDomain::getOnline(){
	return _online.c_str();
}

void UserDomain::setLevel(const char* level){
	_level = level;
}

const char* UserDomain::getLevel(){
	return _level.c_str();
}
void UserDomain::threadFunc(){
	cout<<"thread is running"<<endl;
	net::TcpConnectionPtr conn = Utils::findConnByUsername(remoteScreenUsername.c_str());
	if(!conn){
		cout<<"remoteScreenUsername Conn == NULL"<<endl;
		return ;
	}
	map<muduo::string,queue<muduo::string> >::iterator iter = Utils::screenFrame->find(remoteScreenUsername);
	if(iter == Utils::screenFrame->end()){
		Utils::screenFrame->insert(make_pair(remoteScreenUsername,queue<muduo::string>()));
	}
	iter = Utils::screenFrame->find(remoteScreenUsername);
	queue<muduo::string> &frameQueue = iter->second; 
	while(!frameQueue.empty()){
		frameQueue.pop();
	}
	net::TcpConnectionPtr connSender = Utils::findConnByUsername(_username.c_str());
	if(conn){
		char buffer[128] = {0};
		cout<<"notify sender"<<endl;
		sprintf(buffer,"33:%s",remoteScreenUsername.c_str());
		connSender->send(buffer);
	}
	else{
		cout<<"sender == null"<<endl;
	}
	//while(threadisRunning){
		cout<<"Send Frame Req to Target:"<<remoteScreenUsername<<endl;
		//cout<<"$$$$$$$$$$$$$$$$$$$while thread Curusername "<<_username<<endl;
		if(!conn){
			return;
		}
		char buffer[128] = {0};
		sprintf(buffer,"31:%s",_username.c_str());
		if(!conn){
			return;
		}
		conn->send(buffer);
	//	usleep(0.1*1000*1000);
	//	cout<<"Request " <<remoteScreenUsername<<" screen frame"<<endl;
	//	while((frameQueue.size() == 0) || isSenderRecv){
			//continue;
	//	}
		//cout<<"retrive screenframe send to "<<_username.c_str()<<endl;
	//	net::TcpConnectionPtr connSend = Utils::findConnByUsername(_username.c_str());
	//	if(!connSend){
	//		return;
	//	}
	//	muduo::string data = frameQueue.front();
		//cout<<"queue size:"<<frameQueue.size()<<endl;
		//char header[128] = {0};
		//sprintf(header,"33:%d",(int)data.size());
		//connSend->send(header);
		//isSenderRecv = true;
	//}
	//remoteScreenUsername = "";
}
void UserDomain::remoteScreenTimer(){
	//sendScreenFrameNotify = true;
	cout<<"Timer start"<<endl;
	threadisRunning = true;
//	muduo::Thread t(boost::bind(&UserDomain::threadFunc,this));
//	t.start();
	threadFunc();
	
}

void UserDomain::onScreenFrameToConnection(const muduo::net::TcpConnectionPtr &conn){
	//cout<<"------------------------------onScreenFrameToConnection Curusername "<<_username<<endl; 
	cout<<"retrive image to : "<<remoteScreenUsername<<endl;
	if(strcmp(remoteScreenUsername.c_str(),"") == 0){
		cout<<"RemoteScreenUsername == NULL"<<endl;
		isSenderRecv = false;
		threadisRunning = false;
		return;
	}
	map<muduo::string,queue<muduo::string> >::iterator iter;
	iter = Utils::screenFrame->find(remoteScreenUsername);
	queue<muduo::string> &q = iter->second;
	muduo::string data = q.front();
	q.pop();
	cout<<"retrive image size:"<<data.size()<<endl;
	conn->send(data.c_str(),data.size());
	//sendScreenFrameNotify = true;
	isSenderRecv = false;
}

void* UserDomain::sendDataThreadFunc(void *){
	
}