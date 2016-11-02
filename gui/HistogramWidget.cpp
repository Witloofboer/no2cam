#include "HistogramWidget.h"

#include <QPainter>

namespace gui {

//------------------------------------------------------------------------------

HistogramWidget::HistogramWidget(QWidget *parent)
    : QLabel(parent)
    , _pixmap(new QPixmap(256, 256))
    , _pixPaint(_pixmap)
{
    setPixmap(*_pixmap);
    _pixPaint.setPen(Qt::white);
}

//------------------------------------------------------------------------------

void HistogramWidget::update(const Data& intensities)
{
    _pixmap->fill(Qt::black);

    const double max = *std::max_element(intensities, intensities+256);
    if (max != 0)
    {
        for (int i=0; i<256; ++i)
        {
            _pixPaint.drawLine(i,255,i,255*(1-intensities[i]/max));
        }
    }
    setPixmap(*_pixmap);
}

//------------------------------------------------------------------------------

}
