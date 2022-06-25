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

	Sleep(2000);

	portal->Deactivate();

	portal = NULL;
	delete portal;

	hid_exit();
}