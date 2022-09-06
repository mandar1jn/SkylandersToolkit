#include "MainWindow.h"

#include "portal/Portal.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    SetupWindow();
    SetupCentralWidget();
    CreateMenuBar();
    CreateTabWidget();
    CreatePortalMenu();
    SetLayouts();

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

void MainWindow::CreateMenuBar()
{
    menuBar = new MenuBar(this);

    menuBar->setGeometry(QRect(0, 0, 800, 22));

    setMenuBar(menuBar);
}

void MainWindow::CreateTabWidget()
{
    tabWidget = new QTabWidget(centralWidget);
}

void MainWindow::CreatePortalMenu()
{
    QWidget* portalTab = new QWidget;

    portalMenu = new PortalWidget(portalTab);

    tabWidget->addTab(portalTab, tr("Portal"));
}

void MainWindow::SetLayouts()
{
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
