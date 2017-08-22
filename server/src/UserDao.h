#ifndef _USERDAO_H_
#define _USERDAO_H_
#include <mysql/mysql.h>
#include "UserDomain.h"
class UserDao
{
public:
	UserDao();
	int insert(UserDomain &user);
	int QueryByUsername(UserDomain *user,const char* username);
	int setOnline(const char* username);
	int setOffline(const char* username);
private:
	MYSQL *_mysqlConn;
};
#endif