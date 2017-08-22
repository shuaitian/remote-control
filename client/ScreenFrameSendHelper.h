#pragma once
#include <stdint.h>
class ScreenFrameSendHelper
{
public:
	ScreenFrameSendHelper();
	~ScreenFrameSendHelper();
	void senData(const char*, uint32_t size);
private:
	SOCKET sockClient;	
	bool init();
};

