#include "stdafx.h"
#include "UserDomain.h"
#include "ChatDlg.h"


UserDomain::UserDomain()
{
	this->username = "";
	this->online = "";
	this->ip = "";
	this->level = "";
	dlg = NULL;
}


UserDomain::~UserDomain()
{
}
