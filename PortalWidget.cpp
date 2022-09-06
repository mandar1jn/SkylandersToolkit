#include "PortalWidget.h"

#include <QLabel>
#include <cstring>
#include <QGridLayout>

#include "portal/Portal.h"

PortalWidget::PortalWidget(QWidget *parent)
    : QWidget{parent}
{

    Portal* portal = Portal::GetPortal();

    connect(portal, &Portal::StateChanged, this, &PortalWidget::PortalUpdated);

    colorPicker = new QColorDialog(this);

    colorPicker->setWindowTitle(tr("Select portal color"));

    colorPicker->setFixedSize(522, 402);

    connect(colorPicker, &QColorDialog::colorSelected, this, &PortalWidget::ColorSelected);

    QVBoxLayout* layout = new QVBoxLayout;

    IDLabel = new QLabel(this);

    layout->addWidget(IDLabel);

    sideSelector = new QComboBox(this);
    sideSelector->addItem(tr("All"));
    sideSelector->addItem(tr("Left"));
    sideSelector->addItem(tr("Right"));

    sideSelector->setToolTip(tr("Select the portal side to change the color"));

    layout->addWidget(sideSelector);

    colorButton = new QPushButton(tr("Select portal color"), this);

    connect(colorButton, &QPushButton::clicked, this, &PortalWidget::OpenColorSelector);

    layout->addWidget(colorButton);

    layout->setContentsMargins(5, 5, 5, 5);

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

    if(!(Portal::GetPortal()->features & SIDED_COLOR))
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
