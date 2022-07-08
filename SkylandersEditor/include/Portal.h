#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>

#include "hidapi.h"
#include "RWCommand.h"

typedef struct SupportedFeatures
{
	bool color;
	bool sidedColor;

	SupportedFeatures(bool supportColor = false, bool supportSidedColor = false)
	{
		color = supportColor;
		sidedColor = supportSidedColor;
	}
} SupportedFeatures;

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

	bool connected;
	std::string status;
	SupportedFeatures features;
	void Disconnect();

	unsigned char Id[2] = {0x00, 0x00};

private:
	hid_device* handle;
	void SetFeatures();
};

