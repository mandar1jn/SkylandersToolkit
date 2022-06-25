#pragma once

#include <string>

class RWCommand
{
public:
	unsigned char buffer[0x21];

	RWCommand();
	~RWCommand();
};
