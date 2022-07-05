#pragma once

#include <string>

class RWCommand
{
public:
	unsigned char writeBuffer[0x21];
	unsigned char readBuffer[0x20];

	RWCommand();
	~RWCommand();
};
