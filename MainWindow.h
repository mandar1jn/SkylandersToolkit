#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

#include "MenuBar.h"
#include "PortalWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetupWindow();
    void SetupCentralWidget();
    void SetLayouts();

    MenuBar* menuBar;
    PortalWidget* portalMenu;
    QTabWidget* tabWidget;
    QWidget* centralWidget;
};
#endif // MAINWINDOW_H
