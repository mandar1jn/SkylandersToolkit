#pragma once

#include <cstring>

class RWCommand
{
public:
	unsigned char writeBuffer[0x21];
	unsigned char readBuffer[0x20];

	RWCommand();
	~RWCommand();
};
