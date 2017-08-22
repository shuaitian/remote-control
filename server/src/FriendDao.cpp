#include "FriendDao.h"
#include "Commons.h"

FriendDao::FriendDao(){
	_mysqlConn = mysql_init(NULL);
	_mysqlConn = mysql_real_connect(_mysqlConn,"localhost","root",
		"root","chat",0,NULL,0);
}

int FriendDao::insert(const char* username1,const char* username2){
	char sql[1024] = {0};
	sprintf(sql,"insert into friend(username1,username2) values('%s','%s')",username1,username2);
	int flag = 0;
	flag = mysql_real_query(_mysqlConn,sql,strlen(sql));
	if(flag) {
		printf("add data error:%d from %s\n",mysql_errno(_mysqlConn),mysql_error(_mysqlConn));
		return 0;
	}
	else return 1;
}

bool FriendDao::isFriend(const char* username1,const char* username2){
	int flag = -1;
	MYSQL_RES *res = NULL;
	char sql[1024] = {0};
	sprintf(sql,"select * from friend where (username1='%s' and username2='%s') or (username1='%s' and username2='%s')",
		username1,username2,username2,username1);
	flag = mysql_real_query(_mysqlConn,sql,strlen(sql));
	if(flag){
		printf("querry error:%d from %s\n",mysql_errno(_mysqlConn),mysql_error(_mysqlConn));
		return true;
	}
	res = mysql_store_result(_mysqlConn);
	my_ulonglong rows = mysql_num_rows(res);
	if(rows >= 1){
		return true;
	}else {
		return false;
	}
}

void FriendDao::query(vector<std::string> *v,const char* username,int pos){
	int flag = -1;
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = NULL;
	char sql[1024];
	if(pos == 1){
		sprintf(sql,"select username2 from friend where username1='%s'",username);
	}else if(pos == 2){
		sprintf(sql,"select username1 from friend where username2='%s'",username);
	}
	flag = mysql_real_query(_mysqlConn,sql,strlen(sql));
	if(flag){
		printf("querry error:%d from %s\n",mysql_errno(_mysqlConn),mysql_error(_mysqlConn));
		return ;
	}
	res = mysql_store_result(_mysqlConn);
	my_ulonglong rows = mysql_num_rows(res);
	if(rows == 0){
		return ;
	}
	for(int i=0;i<rows;i++){
		row = mysql_fetch_row(res);
		std::string str = row[0];
		v->push_back(str);
	}
	mysql_free_result(res);
}