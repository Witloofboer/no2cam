#include "DatagramBox.h"

#include <QLabel>
#include <QHBoxLayout>

namespace gui {

//------------------------------------------------------------------------------

DatagramBox::DatagramBox(QWidget *parent)
    : QGroupBox(tr("Intensity datagram"), parent)
{
    auto datagram = QPixmap(200, 200);
    auto datagramLbl = new QLabel();
    datagramLbl->setPixmap(datagram);

    auto layout = new QHBoxLayout;
    layout->addStretch();
    layout->addWidget(datagramLbl);
    layout->addStretch();
    setLayout(layout);
}

//------------------------------------------------------------------------------

}
