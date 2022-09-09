#include "PortalWorker.h"

#include "portal/Portal.h"

void PortalWorker::Run()
{
    Portal* portal = Portal::GetPortal();
    portal->portalMutex.lock();

    if(!portal->connected)
        goto end;

    if(!unverifiedCommands.empty())
    {
        RWCommand* command = unverifiedCommands.front();

        portal->Write(command);

        delete command;

        unverifiedCommands.erase(unverifiedCommands.begin());
    }

   end:
    portal->portalMutex.unlock();
}
