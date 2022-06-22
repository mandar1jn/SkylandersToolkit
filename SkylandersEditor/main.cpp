#include <stdio.h>
#include <wchar.h>

#include "Portal.h"

int main()
{
	hid_init();

	Portal* portal = new Portal();

	while (true)
	{
		portal->PrintIncoming();
	}

	portal = NULL;
	delete portal;

	hid_exit();
}