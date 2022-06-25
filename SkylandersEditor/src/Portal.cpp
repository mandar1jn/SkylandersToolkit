#include "Portal.h"

Portal::Portal()
{
	Connect();
}

Portal::~Portal()
{
	hid_close(handle);
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
	BOOL res;

	OVERLAPPED ol;
	memset(&ol, 0, sizeof(ol));

	DWORD bytes_returned;

	res = DeviceIoControl(handle->device_handle,
		IOCTL_HID_SET_OUTPUT_REPORT,
		(unsigned char*)command->buffer, 0x21,
		(unsigned char*)command->buffer, 0x21,
		&bytes_returned, &ol);
}

#else

void Portal::Write(RWCommand* command)
{
	hid_write(handle, command->buffer, 0x21);
}

#endif

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

	printf("Portal ID: %X %X\n", readyCommand.buffer[1], readyCommand.buffer[2]);
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