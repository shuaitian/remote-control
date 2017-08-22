#pragma once
class ImageReceiveHelper
{
public:
	ImageReceiveHelper();
	~ImageReceiveHelper();
	char* receiveImage(int port,int size);
private:
	bool init();
	SOCKET sockClient;
};

