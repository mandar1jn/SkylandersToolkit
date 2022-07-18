#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>
#include <QAction>


class MenuBar final : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget* parent = nullptr);

private:
    void AddPortalMenu();

    QAction* connectPortalAction;
    QAction* disconnectPortalAction;

private slots:
    void ConnectPortal();
    void DisconnectPortal();
    void PortalUpdated();
};

#endif // MENUBAR_H
