#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "MenuBar.h"
#include "PortalWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MenuBar* menuBar;
    PortalWidget* portalMenu;
};
#endif // MAINWINDOW_H
