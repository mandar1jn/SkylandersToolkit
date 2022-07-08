#include "Skylander.h"

Skylander::Skylander()
{
	memset(data, 0, SKYLANDER_SIZE);
}

unsigned char* Skylander::GetData()
{
	return data;
}