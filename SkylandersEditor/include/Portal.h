#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "hidapi.h"
#include "RWCommand.h"

class Portal
{
public:
	Portal();
	~Portal();
	void Connect();
	void Write(RWCommand* command);
	bool CheckResponse(RWCommand* command, char character);
	void Ready();
	void Activate();
	void Deactivate();
	void SetColor(int r, int g, int b);
	void SetColorAlternative(int side, int r, int g, int b, int u, int duration);

private:
	hid_device* handle;
};

