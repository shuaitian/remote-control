#ifndef _FRIENDDAO_H_
#define _FRIENDDAO_H_
#include <mysql/mysql.h>
#include <vector>
#include <string>
using namespace std;

class FriendDao
{
public:
	FriendDao();
	int insert(const char* username1,const char* username2);
	bool isFriend(const char* username1,const char* username2);
	void query(vector<std::string> *v,const char* username,int pos);
private:
	MYSQL *_mysqlConn;
};

#endif