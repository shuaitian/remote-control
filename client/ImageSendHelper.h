#pragma once
class ImageSendHelper
{
public:
	ImageSendHelper();
	~ImageSendHelper();
	bool init();
	SOCKET sockClient;
	void sendData(const char* data,int size);
};

