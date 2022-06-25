#include "RWCommand.h"

RWCommand::RWCommand()
{
	memset(buffer, 0, 0x21);
}

RWCommand::~RWCommand()
{
}
