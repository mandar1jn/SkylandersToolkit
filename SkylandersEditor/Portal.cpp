#include "Portal.h"

Portal::Portal()
{
	FindPortal();
}

Portal::~Portal()
{
	hid_close(handle);
}

void Portal::PrintIncoming()
{
	unsigned char inBuffer[32];

	hid_read(handle, inBuffer, 32);

	std::cout << "[";

	for (int i = 0; i < 32; i++)
	{
		std::cout << (int)inBuffer[i] << ", ";
	}
	std::cout << "]" << std::endl;
}

void Portal::FindPortal()
{
	hid_device_info* portals = hid_enumerate(0x1430, 0x0150);

	if (portals == NULL)
	{
		printf("No portals found. Please make sure one is connected.");
		exit(1);
	}

	if (portals->next != NULL)
	{
		printf("Multiple portals found. Please make sure only one is connected.");
		exit(1);
	}

	handle = hid_open_path(portals->path);

	printf("Portal found\n");

	hid_free_enumeration(portals);
}