#include "UserDao.h"
#include <string.h>
#include <stdio.h>	
#include <iostream>
using namespace std;

UserDao::UserDao(){
	_mysqlConn = mysql_init(NULL);
	_mysqlConn = mysql_real_connect(_mysqlConn,"localhost","root",
		"root","chat",0,NULL,0);
}

int UserDao::insert(UserDomain &user){
	char sql[1024] = {0};
	// insert into user(name,password) values('','')
	sprintf(sql,"insert into user(username,password,mail) values('%s','%s','%s')",user.getUsername(),user.getPassword(),user.getMail());
	int flag = 0;
	flag = mysql_real_query(_mysqlConn,sql,strlen(sql));
	if(flag) {
		printf("add data error:%d from %s\n",mysql_errno(_mysqlConn),mysql_error(_mysqlConn));
		return 0;
	}
	else return 1;
}

int UserDao::QueryByUsername(UserDomain *user,const char* username){
	int flag = -1;
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = NULL;
	char sql[1024];
	sprintf(sql,"select * from user where username='%s'",username);
	flag = mysql_real_query(_mysqlConn,sql,strlen(sql));
	if(flag){
		printf("querry error:%d from %s\n",mysql_errno(_mysqlConn),mysql_error(_mysqlConn));
		return -2;
	}
	res = mysql_store_result(_mysqlConn);
	my_ulonglong rows = mysql_num_rows(res);
	if(rows == 0){
		return 0;
	}
	row = mysql_fetch_row(res);
	user->setUsername(row[1]);
	user->setPassword(row[2]);
	user->setMail(row[3]);
	user->setLevel(row[4]);
	user->setOnline(row[5]);
	mysql_free_result(res);
	
	return -1;
}

int UserDao::setOnline(const char* username){
	int flag = -1;
	char sql[1024];
	sprintf(sql,"update user set online=1 where username='%s'",username);
	flag = mysql_real_query(_mysqlConn,sql,strlen(sql));
	if(flag){
		printf("update error:%d from %s\n",mysql_errno(_mysqlConn),mysql_error(_mysqlConn));
		return -1;
	}
	return 0;
}

int UserDao::setOffline(const char *username){
	int flag = -1;
	char sql[1024];
	sprintf(sql,"update user set online=0 where username='%s'",username);
	flag = mysql_real_query(_mysqlConn,sql,strlen(sql));
	if(flag){
		printf("update error:%d from %s\n",mysql_errno(_mysqlConn),mysql_error(_mysqlConn));
		return -1;
	}
	return 0;
}