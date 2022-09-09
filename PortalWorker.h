#ifndef PORTALWORKER_H
#define PORTALWORKER_H

#include <QObject>
#include <QThread>

#include <stdio.h>

#include <vector>

#include "RWCommand.h"

class PortalWorker : public QThread
{
    Q_OBJECT
public:
    void run() override
    {
        unverifiedCommands = std::vector<RWCommand*>();

        while(!isInterruptionRequested())
        {
            Run();
        }
    }

    void Run();

    void AddUnverifiedCommand(RWCommand* command)
    {
        unverifiedCommands.push_back(command);
    }

private:

    std::vector<RWCommand*> unverifiedCommands;
};

#endif // PORTALWORKER_H
