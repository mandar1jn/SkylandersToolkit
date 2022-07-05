#include "RWCommand.h"

RWCommand::RWCommand()
{
	memset(writeBuffer, 0, 0x21);
	memset(readBuffer, 0, 0x20);
}

RWCommand::~RWCommand()
{
}
