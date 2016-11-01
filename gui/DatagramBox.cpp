#include "DatagramBox.h"

#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>

namespace gui {

//------------------------------------------------------------------------------

DatagramBox::DatagramBox(QWidget *parent)
    : QGroupBox(tr("Intensity datagram"), parent)
    , _pixmap(new QPixmap(256, 256))
    , _pixPaint(_pixmap)
    , _datagramLbl(new QLabel())
{
    _datagramLbl->setPixmap(*_pixmap);
    //QGraphicsPixmapItem* item(scene->addPixmap(*pix))

    auto layout = new QHBoxLayout;
    layout->addStretch();
    layout->addWidget(_datagramLbl);
    layout->addStretch();
    setLayout(layout);
    _pixPaint.setPen(Qt::white);
}

//------------------------------------------------------------------------------

void DatagramBox::update(Datagram datagram)
{
    _pixmap->fill(Qt::black);

    const double max = *std::max_element(datagram, datagram+256);
    if (max != 0)
    {
        for (int i=0; i<256; ++i)
        {
            _pixPaint.drawLine(i,255,i,255*(1-datagram[i]/max));
        }
    }
    _datagramLbl->setPixmap(*_pixmap);
}

//------------------------------------------------------------------------------

}
