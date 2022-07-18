#include "PortalWidget.h"

#include <QLabel>
#include <cstring>
#include <QGridLayout>

#include "Portal.h"

PortalWidget::PortalWidget(QWidget *parent)
    : QWidget{parent}
{
    move(0, 15);

    resize(130, 90);

    Portal* portal = Portal::GetPortal();

    connect(portal, &Portal::StateChanged, this, &PortalWidget::PortalUpdated);

    colorPicker = new QColorDialog(this);

    colorPicker->setWindowTitle(tr("Select portal color"));

    connect(colorPicker, &QColorDialog::colorSelected, this, &PortalWidget::ColorSelected);

    QGridLayout* layout = new QGridLayout;

    IDLabel = new QLabel(this);

    layout->addWidget(IDLabel, 0, 0);

    sideSelector = new QComboBox(this);
    sideSelector->addItem(tr("All"));
    sideSelector->addItem(tr("Left"));
    sideSelector->addItem(tr("Right"));

    sideSelector->setToolTip(tr("Select the portal side to change the color"));

    layout->addWidget(sideSelector, 1, 0);

    colorButton = new QPushButton(tr("Select portal color"), this);

    connect(colorButton, &QPushButton::clicked, this, &PortalWidget::OpenColorSelector);

    layout->addWidget(colorButton, 2, 0);

    setLayout(layout);

    PortalUpdated();

}

PortalWidget::~PortalWidget()
{

}

void PortalWidget::PortalUpdated()
{
    char idText[21];

    Portal* portal = Portal::GetPortal();

    sprintf(idText, "Portal ID: 0x%X 0x%X", portal->Id[0], portal->Id[1]);
    IDLabel->setText(tr(idText));

    if(!Portal::GetPortal()->features.sidedColor)
    {
        sideSelector->setEditable(false);
        sideSelector->setEnabled(false);
        sideSelector->setCurrentIndex(0);
    }
    else
    {
        sideSelector->setEditable(true);
        sideSelector->setEnabled(true);
    }


}

void PortalWidget::ColorSelected(const QColor &color)
{
    switch(sideSelector->currentIndex())
    {
    // all
    case 0:
        Portal::GetPortal()->SetColor(color.red(), color.green(), color.blue());
        break;
    // left
    case 1:
        Portal::GetPortal()->SetColorAlternative(0x02, color.red(), color.green(), color.blue(), 0x00, 0x00);
        break;
    // right
    case 2:
        Portal::GetPortal()->SetColorAlternative(0x00, color.red(), color.green(), color.blue(), 0x00, 0x00);
        break;
    }
}

void PortalWidget::OpenColorSelector()
{
    colorPicker->show();
}
