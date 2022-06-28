#include "Portal.h"

Portal::Portal()
{
	handle = NULL;
	connected = false;
	status = "Portal not connected";
}

Portal::~Portal()
{
	hid_close(handle);
}

bool Portal::CheckResponse(RWCommand* command, char character)
{
	if (!connected) return true;

	hid_read(handle, command->buffer, 0x20);

	return !(command->buffer[0] == character);
}

void Portal::Connect()
{
	hid_device_info* portals = hid_enumerate(0x1430, 0x0150);

	if (portals == NULL)
	{
		status = "No portal was found.Please make sure one is connected.";
		return;
	}

	if (portals->next != NULL)
	{
		status = "Multiple portals found. Please make sure only one is connected.";
		return;
	}

	handle = hid_open_path(portals->path);

	status = "Portal connected";

	connected = true;

	hid_free_enumeration(portals);
}

void Portal::Ready()
{
	if (!connected) return;

	RWCommand readyCommand = RWCommand();

	readyCommand.buffer[1] = 'R';

	do
	{
		Write(&readyCommand);
	} while (CheckResponse(&readyCommand, 'R'));

	printf("Portal ID: %X %X\n", readyCommand.buffer[1], readyCommand.buffer[2]);
}

void Portal::Activate()
{
	if (!connected) return;

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
	if (!connected) return;

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
	if (!connected) return;

	RWCommand colorCommand = RWCommand();

	colorCommand.buffer[1] = 'C';
	colorCommand.buffer[2] = r;
	colorCommand.buffer[3] = g;
	colorCommand.buffer[4] = b;

	Write(&colorCommand);
}

/*
	Known codes:
	SetColorAlternative(0x00, 0x14, 0x28, 0x14, 0xF4, 0x01) (right grey)
	SetColorAlternative(0x00, 0x00, 0xFF, 0x00, 0xD0, 0x07) (right green)
	SetColorAlternative(0x00, 0x00, 0x00, 0x00, 0x00, 0x00) (disable right side)
	SetColorAlternative(0x02, 0x00, 0xFF, 0x00, 0xD0, 0x07) (left green)
	SetColorAlternative(0x00, 0x00, 0x00, 0xFF, 0xD0, 0x07) (right blue)
	SetColorAlternative(0x02, 0xFF, 0x00, 0x00, 0xD0, 0x07) (left red)
	SetColorAlternative(0x00, 0xFF, 0x00, 0x00, 0xD0, 0x07) (right red)
	SetColorAlternative(0x00, 0x64, 0x3C, 0x64, 0xF4, 0x01) (right pink)
*/

//observed values for u1 have been: 0xF4 and 0xD0
void Portal::SetColorAlternative(int side, int r, int g, int b, int u, int duration)
{
	if (!connected) return;

	RWCommand colorCommand = RWCommand();

	colorCommand.buffer[1] = 'J';
	colorCommand.buffer[2] = side;
	colorCommand.buffer[3] = r;
	colorCommand.buffer[4] = g;
	colorCommand.buffer[5] = b;
	colorCommand.buffer[6] = u;
	colorCommand.buffer[7] = duration;

	Write(&colorCommand);

}

#ifdef _WIN32

#include <Windows.h>

#define HID_CTL_CODE(id)    \
        CTL_CODE(FILE_DEVICE_KEYBOARD, (id), METHOD_NEITHER, FILE_ANY_ACCESS)
#define HID_IN_CTL_CODE(id)  \
        CTL_CODE(FILE_DEVICE_KEYBOARD, (id), METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_HID_SET_OUTPUT_REPORT             HID_IN_CTL_CODE(101)

struct hid_device_ {
	HANDLE device_handle;
	BOOL blocking;
	USHORT output_report_length;
	unsigned char* write_buf;
	size_t input_report_length;
	USHORT feature_report_length;
	unsigned char* feature_buf;
	wchar_t* last_error_str;
	BOOL read_pending;
	char* read_buf;
	OVERLAPPED ol;
	OVERLAPPED write_ol;
	struct hid_device_info* device_info;
};


void Portal::Write(RWCommand* command)
{
	if (!connected) return;

	BOOL res;

	OVERLAPPED ol;
	memset(&ol, 0, sizeof(ol));

	DWORD bytes_returned;

	res = DeviceIoControl(handle->device_handle,
		IOCTL_HID_SET_OUTPUT_REPORT,
		(unsigned char*)command->buffer, 0x21,
		(unsigned char*)command->buffer, 0x21,
		&bytes_returned, &ol);

	if (!res)
	{
		status = "failed to write. Disconnected.";
		connected = false;
	}
}

#else

void Portal::Write(RWCommand* command)
{
	if (!connected) return;

	int res = hid_write(handle, command->buffer, 0x21);

	if (res == -1)
	{
		status = "failed to write. Disconnected.";
		connected = false;
	}
}

#endif