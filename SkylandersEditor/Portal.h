#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "hidapi.h"

class Portal
{
public:
	Portal();
	~Portal();
	void PrintIncoming();

private:
	hid_device* handle;

	void FindPortal();
};

