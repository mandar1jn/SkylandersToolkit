#include "Portal.h"

Portal::Portal()
{
	Connect();
}

Portal::~Portal()
{
	hid_close(handle);
}

void Portal::Write(RWCommand* command)
{
	hid_write(handle, command->buffer, 0x21);
}

bool Portal::CheckResponse(RWCommand* command, char character)
{
	hid_read(handle, command->buffer, 0x20);

	return !(command->buffer[0] == character);
}

void Portal::Connect()
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

void Portal::Ready()
{
	RWCommand readyCommand = RWCommand();

	readyCommand.buffer[1] = 'R';

	do
	{
		Write(&readyCommand);
	} while (CheckResponse(&readyCommand, 'R'));
}

void Portal::Activate()
{
	RWCommand activateCommand = RWCommand();

	activateCommand.buffer[1] = 'A';
	activateCommand.buffer[2] = 0x01;

	do
	{
		Write(&activateCommand);
	} while (CheckResponse(&activateCommand, 'A'));
}

void Portal::Deactivate()
{
	RWCommand deactivateCommand = RWCommand();

	deactivateCommand.buffer[1] = 'A';
	deactivateCommand.buffer[2] = 0x00;

	do
	{
		Write(&deactivateCommand);
	} while (CheckResponse(&deactivateCommand, 'A'));
}

void Portal::SetColor(int r, int g, int b)
{
	RWCommand deactivateCommand = RWCommand();

	deactivateCommand.buffer[1] = 'C';
	deactivateCommand.buffer[2] = r;
	deactivateCommand.buffer[3] = g;
	deactivateCommand.buffer[4] = b;

	Write(&deactivateCommand);
}