#include "Utils.h"
#include <string.h>
#include "UserDomain.h"
#include <boost/shared_ptr.hpp>

map<std::string,net::TcpConnectionPtr>* Utils::onlineList = NULL;

void Utils::splitString(vector<muduo::string> *res,const char* data,const char* pattern){
	muduo::string datastr(data);
	size_t pos = 0;
	size_t index = 0;
	vector<muduo::string> result;
	datastr+=pattern;
	while((index = datastr.find(pattern,pos)) != muduo::string::npos){
	//	cout<<"pos:"<<pos<<" index:"<<index<<endl;
		muduo::string str = datastr.substr(pos,index-pos);
		pos = index+strlen(pattern);
		res->push_back(str);
	}
}

const char* Utils::getUsernameFromConn(net::TcpConnectionPtr conn){
	if(!(conn->getContext()).empty()){
		UserDomain *user = boost::any_cast<UserDomain*>(conn->getContext());
		return user->getUsername();
	}
	return NULL;
}
UserDomain* Utils::getUserDomainFromConn(net::TcpConnectionPtr conn){
	if(!(conn->getContext()).empty()){
		UserDomain *user = boost::any_cast<UserDomain*>(conn->getContext());
		return user;
	}
	return NULL;
}

net::TcpConnectionPtr Utils::findConnByUsername(const char* username){
	map<std::string,net::TcpConnectionPtr>::iterator iter;
	iter = onlineList->find(username);
	if(iter!=onlineList->end()){
		return (*iter).second;
	}else{
		return boost::shared_ptr<net::TcpConnection>();
	}
}

map<muduo::string,queue<muduo::string> >* Utils::screenFrame = new map<muduo::string,queue<muduo::string> >();