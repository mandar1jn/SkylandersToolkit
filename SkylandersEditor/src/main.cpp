#include <stdio.h>
#include <wchar.h>
#include <Windows.h>

#include "Portal.h"

int main()
{
	hid_init();

	Portal* portal = new Portal();

	portal->Ready();

	portal->Activate();

	portal->SetColorAlternative(0x02, 0x00, 0x00, 0x00, 0x00, 0xFF);

	//Sleep(10000);

	//portal->Deactivate();

	portal = NULL;
	delete portal;

	hid_exit();
}