#include "RWCommand.h"

#include <cstring>
#include <QMessageBox>
#include <QString>
#include <QObject>
#include <hidapi.h>

#include "portal/Portal.h"

RWCommand::RWCommand()
{
    memset(writeBuffer, 0, 0x21);
    memset(readBuffer, 0, 0x20);
}

RWCommand::~RWCommand()
{
}

bool RWCommand::SendUnverified()
{
    Portal* portal = Portal::GetPortal();

    if(!portal->connected)
    {
        return false;
    }

    portal->Write(this);

    return true;
}

bool RWCommand::SendVerified(char expectedChar, unsigned int attempts)
{
    Portal* portal = Portal::GetPortal();

    if(!portal->connected)
    {
        return false;
    }

    for(unsigned int i = 0; i < attempts; i++)
    {
        portal->Write(this);

        hid_read(portal->handle, this->readBuffer, 0x20);

        if(this->readBuffer[0] == expectedChar)
        {
            return true;
        }
    }

    QMessageBox::warning(new QWidget, QObject::tr("Didn't receive the correct response"), QObject::tr("The portal did not give the correct response."));

    return false;
}
