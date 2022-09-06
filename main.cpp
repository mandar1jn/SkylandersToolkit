#include "MainWindow.h"

#include <QApplication>
#include <thread>
#include <iostream>

void IOFunc()
{
    std::cout << "test" << std::endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
