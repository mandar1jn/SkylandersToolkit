#ifndef PORTAL_H
#define PORTAL_H

#include <QObject>
#include <QPointer>
#include <hidapi.h>
#include <QMutex>

#include "RWCommand.h"
#include "PortalWorker.h"

enum PortalFeatures : int {
    SIDED_COLOR = (1 << 1),
    COLOR = (1 << 0),
    NONE = 0
};

typedef struct PortalStatus {
    bool success;
} PortalStatus;

class Portal : public QObject
{
    Q_OBJECT
public:
    explicit Portal(QObject *parent = nullptr);
    ~Portal();

    static Portal* GetPortal();

    void Connect();
    void Write(RWCommand* command);
    bool CheckResponse(RWCommand* command, char character);
    void Ready();
    void Activate();
    void Deactivate();
    void SetColor(int r, int g, int b);
    void SetColorAlternative(int side, int r, int g, int b, int u, int duration);
    PortalStatus* GetStatus();

    bool connected;
    int features;
    void Disconnect(bool allowWrite);

    unsigned char Id[2] = {0x00, 0x00};

    hid_device* handle;

    QMutex portalMutex;

signals:
    void StateChanged();

private:
    void SetFeatures();

    static QPointer<Portal> self;
    PortalWorker* worker;

};

#endif // PORTAL_H
