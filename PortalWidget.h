#ifndef PORTALWIDGET_H
#define PORTALWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QColorDialog>
#include <QComboBox>
#include <QPushButton>

class PortalWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PortalWidget(QWidget *parent = nullptr);
    ~PortalWidget();

private:
    QLabel* IDLabel;
    QColorDialog* colorPicker;
    QComboBox* sideSelector;
    QPushButton* colorButton;

public slots:
    void PortalUpdated();
    void ColorSelected(const QColor &color);
    void OpenColorSelector();

};

#endif // PORTALWIDGET_H
