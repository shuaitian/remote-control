#pragma once
#include <stdint.h>

class Message {
public:
	int32_t size;
	char data[512];
};
