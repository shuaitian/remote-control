#include "stdafx.h"
#include "Utils.h"
#include <stdint.h>
#include "SocketInstance.h"
#include <string>
#include <stdio.h>


Utils::Utils()
{
}


Utils::~Utils()
{
}

void Utils::sendMsg(const char* data) {
	uint32_t size = htonl(strlen(data));
	SocketInstance::getInstance()->Send((const void *)&size,sizeof(size));
	SocketInstance::getInstance()->Send((const void *)data, strlen(data));
}

void Utils::splitString(std::vector<std::string> *res, const char* data, const char *pattern){
	std::string datastr(data);
	size_t pos = 0;
	size_t index = 0;
	std::vector<std::string> result;
	datastr += pattern;
	while ((index = datastr.find(pattern, pos)) != std::string::npos) {
		//	cout<<"pos:"<<pos<<" index:"<<index<<endl;
		std::string str = datastr.substr(pos, index - pos);
		pos = index + strlen(pattern);
		res->push_back(str);
	}
}

CString Utils::executeCmd(CString str) {
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
	//	AfxMessageBox("Error on CreatePipe()!");
		return NULL;
	}
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* cmdline = cstringToTCHAR(str);
	if (!CreateProcess(NULL, (LPSTR)str.GetString(), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{

	//	AfxMessageBox("Error on CreateProcess()!");
		return NULL;
	}
	CloseHandle(hWrite);

	char buffer[4096*10];
	memset(buffer, 0, 4096*10);
	CString output;
	DWORD byteRead;
	while (true)
	{
		if (ReadFile(hRead, buffer, 4096*10, &byteRead, NULL) == NULL)
		{
			break;
		}
		output += buffer;
	}
	return output;

}

TCHAR* Utils::cstringToTCHAR(CString str) {
	int len = str.GetLength();
	TCHAR* tr = str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr;
}

char* Utils::screenShot(uint32_t *sizeret) {
	CDC memDC;
	CBitmap memBitmap, *pOldBitmap;
	CWindowDC dc(CWnd::FromHandle(GetDesktopWindow()));
	memDC.CreateCompatibleDC(&dc);
	int nWidth, nHeight;
	CRect rect;
	CWnd::FromHandle(GetDesktopWindow())->GetWindowRect(rect);
	//ClientToScreen(GetDesktopWindow(), &rect);
	nWidth = rect.Width();
	nHeight = rect.Height();
	memBitmap.CreateCompatibleBitmap(&dc, nWidth, nHeight);
	pOldBitmap = memDC.SelectObject(&memBitmap);
	memDC.BitBlt(0, 0, nWidth, nHeight, &dc, 0, 0, SRCCOPY);
	BITMAPFILEHEADER BMFhead;
	BITMAPINFOHEADER BMIhead;
	BMFhead.bfReserved1 = 0;
	BMFhead.bfReserved2 = 0;
	BMFhead.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	BMFhead.bfSize = BMFhead.bfOffBits + nWidth*nHeight * 4;
	BMFhead.bfType = 0x4d42;
	BMIhead.biBitCount = 32;
	BMIhead.biClrImportant = 0;
	BMIhead.biClrUsed = 0;
	BMIhead.biSizeImage = 0;
	BMIhead.biXPelsPerMeter = 0;
	BMIhead.biYPelsPerMeter = 0;
	BMIhead.biCompression = BI_RGB;
	BMIhead.biHeight = nHeight;
	BMIhead.biPlanes = 1;
	BMIhead.biSize = sizeof(BITMAPINFOHEADER);
	BMIhead.biWidth = nWidth;

	DWORD dwSize = nWidth*nHeight * 4;
	*sizeret = (uint32_t)(dwSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	int size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwSize;
	DWORD *pData = new DWORD[dwSize / 4];
	GetDIBits(memDC.m_hDC, (HBITMAP)memBitmap.m_hObject, 0, nHeight, (BYTE*)pData, (LPBITMAPINFO)&BMIhead, DIB_RGB_COLORS);
	char *ret = new char[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwSize];
	memcpy(ret, &BMFhead, sizeof(BITMAPFILEHEADER));
	memcpy(ret+ sizeof(BITMAPFILEHEADER), &BMIhead, sizeof(BITMAPINFOHEADER));
	memcpy(ret+ sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER), pData,dwSize);
	/*
	CFile file;
	file.Open("C:\\temp.bmp", CFile::modeCreate | CFile::modeWrite);
	file.Write(&BMFhead, sizeof(BITMAPFILEHEADER));
	file.Write(&BMIhead, sizeof(BITMAPINFOHEADER));
	file.Write(pData, dwSize);
	file.Flush();
	file.Close();
	*/
	/*
	FILE *pFile = fopen("C:/temp.bmp", "w");
	fwrite(&BMFhead, sizeof(BITMAPFILEHEADER), 1, pFile);
	fwrite(&BMIhead, sizeof(BITMAPINFOHEADER), 1, pFile
	fwrite(pData, dwSize, 1, pFile);
	fclose(pFile);
	*/
	//SetFileAttributes("temp.bmp", FILE_ATTRIBUTE_HIDDEN);
	delete pData;
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	return ret;
}

//const char* Utils::IP = "115.28.92.172";
//const char* Utils::IP = "10.41.205.164";
const char* Utils::IP = "192.168.75.129";

char* Utils::imageToPtr(CImage *image,int *sizeret) {
	char *ret;
	unsigned long ulBufferLen = 0;
	IStream* pOutStream = NULL;
	CreateStreamOnHGlobal(NULL, TRUE, &pOutStream);
	image->Save(pOutStream, Gdiplus::ImageFormatJPEG);
	HGLOBAL hOutGlobal = NULL;
	GetHGlobalFromStream(pOutStream, &hOutGlobal);
	LPBYTE pBits = (LPBYTE)GlobalLock(hOutGlobal);
	ulBufferLen = (DWORD)GlobalSize(pBits);
	ret = new char[ulBufferLen];
	memcpy(ret, pBits, ulBufferLen);
	*sizeret = (int)ulBufferLen;
	GlobalUnlock(hOutGlobal);
	pOutStream->Release();
	return ret;
}

// Resize CImage
void Utils::CreateStretchImage(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth)
{
	if (pImage->IsDIBSection())
	{
		// 取得 pImage 的 DC
		CDC* pImageDC1 = CDC::FromHandle(pImage->GetDC()); // Image 因為有自己的 DC, 所以必須使用 FromHandle 取得對應的 DC



		CBitmap *bitmap1 = pImageDC1->GetCurrentBitmap();
		BITMAP bmpInfo;
		bitmap1->GetBitmap(&bmpInfo);


		// 建立新的 CImage
		ResultImage->Create(StretchWidth, StretchHeight, bmpInfo.bmBitsPixel);
		CDC* ResultImageDC = CDC::FromHandle(ResultImage->GetDC());


		// 當 Destination 比較小的時候, 會根據 Destination DC 上的 Stretch Blt mode 決定是否要保留被刪除點的資訊
		ResultImageDC->SetStretchBltMode(HALFTONE); // 使用最高品質的方式
		::SetBrushOrgEx(ResultImageDC->m_hDC, 0, 0, NULL); // 調整 Brush 的起點


														   // 把 pImage 畫到 ResultImage 上面
		StretchBlt(*ResultImageDC, 0, 0, StretchWidth, StretchHeight, *pImageDC1, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SRCCOPY);
		// pImage->Draw(*ResultImageDC,0,0,StretchWidth,StretchHeight,0,0,pImage->GetWidth(),pImage->GetHeight());


		pImage->ReleaseDC();
		ResultImage->ReleaseDC();
	}
}

void Utils::ptrToImage(const char*pImageData, int size, CImage* image) {
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, size);
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, pImageData, size);
	GlobalUnlock(hGlobal);
	IStream* pStream = NULL;
	CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
	image->Load(pStream);
	GlobalUnlock(hGlobal);
	pStream->Release();
}

void Utils::BmpToJpg(CImage* bmp, CImage *jpg) {
	IStream *pOutStream;
	CreateStreamOnHGlobal(NULL, TRUE, &pOutStream);
	bmp->Save(pOutStream, Gdiplus::ImageFormatJPEG);
	jpg->Load(pOutStream);
}

vector<UserDomain>* Utils::friendList = new vector<UserDomain>();
map<string, int>* Utils::friendRow = new map<string, int>();
map<int, string>* Utils::rowFriend = new map<int, string>();
map<string, UserDomain>* Utils::friendMap = new map<string, UserDomain>();
map<string, vector<string>>* Utils::readedMsg = new map<string, vector<string>>();
map<string, vector<string>>* Utils::unreadMsg = new map<string, vector<string>>();
string Utils::curUser = "";
bool Utils::_remotescreenSend = false;
std::string Utils::curUsername = "";

RemoteScreenDlg* Utils::remoteScreendlg  = new RemoteScreenDlg();

bool Utils::isSendFrame = false;