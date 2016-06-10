#include "DatagramBox.h"

#include <QLabel>
#include <QHBoxLayout>

DatagramBox::DatagramBox(QWidget *parent)
    : QGroupBox(tr("Intensity datagram"), parent)
{
    auto datagram = QPixmap(180, 180);
    auto datagramLbl = new QLabel();
    datagramLbl->setPixmap(datagram);

    auto layout = new QHBoxLayout;
    layout->addWidget(datagramLbl);

    setLayout(layout);
}
