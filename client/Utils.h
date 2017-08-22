#pragma once
#include <vector>
#include <string>
#include <map>
#include "UserDomain.h"
#include <stdint.h>
#include "RemoteScreenDlg.h"
using namespace std;

class Utils
{
public:
	static void sendMsg(const char* data);
	Utils();
	~Utils();
	static void splitString(std::vector<std::string> *res, const char* data, const char *pattern);
	static void CreateStretchImage(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth);
	static vector<UserDomain>* friendList;
	static map<string, int> *friendRow;
	static map<int, string> *rowFriend;
	static map<string, UserDomain> *friendMap;
	static map<string, vector<string>> *readedMsg;
	static map<string, vector<string>> *unreadMsg;
	static string curUser;
	static CString executeCmd(CString cmd);
	static TCHAR* cstringToTCHAR(CString str);
	static char* screenShot(uint32_t *size);
	static bool _remotescreenSend;
	static void ptrToImage(const char*pData,int size,CImage* image);
	static void BmpToJpg(CImage* bmp,CImage *jpg);
	static char* imageToPtr(CImage *image,int *sizeret);
	static const char* IP;
	static std::string curUsername;
	static RemoteScreenDlg *remoteScreendlg;
	static bool isSendFrame;
};

