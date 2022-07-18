#include "MenuBar.h"

#include <QMenu>
#include "Portal.h"

MenuBar::MenuBar(QWidget* parent) : QMenuBar(parent)
{    
    AddPortalMenu();
}

void MenuBar::AddPortalMenu()
{
    QMenu* portalMenu = addMenu(tr("&Portal"));

    connectPortalAction = new QAction(tr("&Connect"), this);

    connectPortalAction->setDisabled(Portal::GetPortal()->connected);

    connect(connectPortalAction, &QAction::triggered, this, &MenuBar::ConnectPortal);

    portalMenu->addAction(connectPortalAction);

    disconnectPortalAction = new QAction(tr("&Disconnect"), this);

    disconnectPortalAction->setEnabled(Portal::GetPortal()->connected);

    connect(disconnectPortalAction, &QAction::triggered, this, &MenuBar::DisconnectPortal);

    portalMenu->addAction(disconnectPortalAction);

    connect(Portal::GetPortal(), &Portal::StateChanged, this, &MenuBar::PortalUpdated);

}

void MenuBar::ConnectPortal()
{
    Portal::GetPortal()->Connect();
}

void MenuBar::DisconnectPortal()
{
    Portal::GetPortal()->Disconnect();
}

void MenuBar::PortalUpdated()
{
    connectPortalAction->setDisabled(Portal::GetPortal()->connected);

    disconnectPortalAction->setEnabled(Portal::GetPortal()->connected);
}
