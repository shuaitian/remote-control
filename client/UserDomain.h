#pragma once
#include <string>

class ChatDlg;
class UserDomain
{
public:
	UserDomain();
	~UserDomain();
	std::string username;
	std::string online;
	std::string ip;
	std::string level;
	ChatDlg *dlg;

};

