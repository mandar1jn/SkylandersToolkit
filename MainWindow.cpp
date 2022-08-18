#include "MainWindow.h"

#include "Portal.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumWidth(300);
    this->setMinimumHeight(200);

    this->setWindowTitle(tr("Skylanders Editor"));

    menuBar = new MenuBar(this);

    this->setMenuBar(menuBar);

    portalMenu = new PortalWidget(this);

}

MainWindow::~MainWindow()
{
    Portal* portal = Portal::GetPortal();

    if(portal->connected)
    {
        portal->Disconnect(true);
    }
}
