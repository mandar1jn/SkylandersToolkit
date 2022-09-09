#include "Portal.h"

#include <QMessageBox>

QPointer<Portal> Portal::self;

Portal::Portal(QObject *parent)
    : QObject{parent}
{
    self = this;

    handle = NULL;
    connected = false;
    features = NONE;

    worker = new PortalWorker;

    Connect();
}

Portal::~Portal()
{

    if(connected)
    {
        Disconnect(true);
    }

    hid_close(handle);

    delete worker;
    worker = NULL;
}

Portal* Portal::GetPortal()
{
    if(self.isNull())
    {
        self = new Portal();
    }

    return self;
}

bool Portal::CheckResponse(RWCommand* command, char character)
{
    if (!connected) return false;

    hid_read(handle, command->readBuffer, 0x20);

    return command->readBuffer[0] != character;
}

void Portal::Connect()
{
    hid_device_info* portals = hid_enumerate(0x1430, 0x0150);

    if (portals == NULL)
    {
        QMessageBox::warning(new QWidget, tr("Portal not found"), tr("No portal was found. Please make sure one is connected."));
        return;
    }

    if (portals->next != NULL)
    {
        QMessageBox::warning(new QWidget, tr("Multiple portals found"), tr("Multiple portals were found. Please make sure only one portal is connected."));
        return;
    }

    handle = hid_open_path(portals->path);

    hid_free_enumeration(portals);

    connected = true;

    worker->start();

    Ready();

    Activate();

    emit StateChanged();
}

void Portal::Ready()
{
    if (!connected) return;

    RWCommand readyCommand = RWCommand();

    readyCommand.writeBuffer[1] = 'R';

    if(!readyCommand.SendVerified('R', 15))
    {
        Disconnect(false);
        return;
    }

    Id[0] = readyCommand.readBuffer[1];
    Id[1] = readyCommand.readBuffer[2];

    SetFeatures();
}

void Portal::Activate()
{
    if (!connected) return;

    RWCommand activateCommand = RWCommand();

    activateCommand.writeBuffer[1] = 'A';
    activateCommand.writeBuffer[2] = 0x01;

    activateCommand.SendVerified('A', 15);
}

void Portal::Deactivate()
{
    if (!connected) return;

    RWCommand* deactivateCommand = new RWCommand();

    deactivateCommand->writeBuffer[1] = 'A';
    deactivateCommand->writeBuffer[2] = 0x00;

    worker->AddUnverifiedCommand(deactivateCommand);
}

void Portal::SetColor(int r, int g, int b)
{
    if (!connected) return;

    RWCommand* colorCommand = new RWCommand();

    colorCommand->writeBuffer[1] = 'C';
    colorCommand->writeBuffer[2] = r;
    colorCommand->writeBuffer[3] = g;
    colorCommand->writeBuffer[4] = b;

    worker->AddUnverifiedCommand(colorCommand);
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

    RWCommand* colorCommand = new RWCommand();

    colorCommand->writeBuffer[1] = 'J';
    colorCommand->writeBuffer[2] = side;
    colorCommand->writeBuffer[3] = r;
    colorCommand->writeBuffer[4] = g;
    colorCommand->writeBuffer[5] = b;
    colorCommand->writeBuffer[6] = u;
    colorCommand->writeBuffer[7] = duration;

    worker->AddUnverifiedCommand(colorCommand);

}

void Portal::SetFeatures()
{
    features = NONE;

    switch (Id[0])
    {
    case 0x01:
        switch (Id[1])
        {
        //runic portal (spyro's adventure, wireless)
        case 0x29:
            features = COLOR;
            break;
            // runic portal (giants, wired)
        case 0x3C:
        case 0x3D:
            features = COLOR;
            break;
            // runic portal (battlegrounds)
        case 0x40:
            features = COLOR;
        }
        break;
    case 0x02:
        switch (Id[1])
        {
        // traptanium portal
        case 0x1B:
        case 0x18:
            features = COLOR | SIDED_COLOR;
            break;
            // swap force portal
        case 0x00:
        case 0x03:
            features = COLOR;
        }
        break;
    }
}

PortalStatus* Portal::GetStatus()
{
    if(!connected)
    {
        PortalStatus* status = new PortalStatus();

        return status;
    }

    RWCommand statusCommand = RWCommand();
    statusCommand.writeBuffer[1] = 'S';

    PortalStatus* status = new PortalStatus();

    if(!statusCommand.SendVerified('S', 15))
    {
        status->success = false;
        return status;
    }

    status->success = true;

    return status;
}

void Portal::Disconnect(bool allowWrite)
{
    worker->requestInterruption();

    features = NONE;

    if(allowWrite)
    {
        Deactivate();
    }
    memset(Id, 0, 2);
    connected = false;

    emit StateChanged();
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
                          (unsigned char*)command->writeBuffer, 0x21,
                          (unsigned char*)command->writeBuffer, 0x21,
                          &bytes_returned, &ol);

    if (!res)
    {
        QMessageBox::warning(new QWidget, tr("Failed to write to portal."), tr("Failed to write to the portal. Disconnecting."));
        Disconnect(false);
    }
}

#else

void Portal::Write(RWCommand* command)
{
    if (!connected) return;

    int res = hid_write(handle, command->writeBuffer, 0x21);

    if (res == -1)
    {
        QMessageBox::warning(new QWidget, tr("Failed to write to portal."), tr("Failed to write to the portal. Disconnecting."));
        Disconnect(false);
    }
}

#endif
