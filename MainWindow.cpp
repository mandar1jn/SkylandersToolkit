#include "MainWindow.h"

#include "portal/Portal.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    SetupWindow();
    SetupCentralWidget();
    tabWidget = new QTabWidget(centralWidget);
    QWidget* portalTab = new QWidget;
    SetLayouts();

    menuBar = new MenuBar(this);

    menuBar->setGeometry(QRect(0, 0, 800, 22));

    setMenuBar(menuBar);

    portalMenu = new PortalWidget(portalTab);

    tabWidget->addTab(portalTab, tr("Portal"));

}

void MainWindow::SetupWindow()
{

    setMinimumSize(300, 200);

    setWindowTitle(tr("Skylanders Editor"));
}

void MainWindow::SetupCentralWidget() {

    centralWidget = new QWidget;

    setCentralWidget(centralWidget);

}

void MainWindow::SetLayouts()
{
//    QVBoxLayout* windowLayout = new QVBoxLayout(this);

//    windowLayout->setContentsMargins(5, 5, 5, 5);

//    windowLayout->addWidget(centralWidget);

    QVBoxLayout* centralLayout = new QVBoxLayout(centralWidget);

    QWidget *topFiller = new QWidget;
    topFiller->setFixedHeight(10);
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    centralLayout->addWidget(topFiller);

    centralLayout->addWidget(tabWidget);
}

MainWindow::~MainWindow()
{
    Portal* portal = Portal::GetPortal();

    if(portal->connected)
    {
        portal->Disconnect(true);
    }
}
