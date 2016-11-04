#include "HistogramWidget.h"

#include <QPainter>

namespace gui {

//------------------------------------------------------------------------------

HistogramWidget::HistogramWidget(QWidget *parent)
    : QLabel(parent)
    , _pixmap(new QPixmap(256+squeezeWidth, 256))
    , _painter(_pixmap)
    , _brush(Qt::white, Qt::SolidPattern)
    , _redBrush(Qt::red, Qt::SolidPattern)
{
    _pixmap->fill(Qt::black);
    setPixmap(*_pixmap);
}

//------------------------------------------------------------------------------

void HistogramWidget::update(const Data& intensities)
{

    quint32 squeezedValues[nbrSqueezedValues];
    quint32 const * cur = intensities;

    for (unsigned i=0; i<nbrSqueezedValues; ++i)
    {
        squeezedValues[i]=0;
        for(unsigned j=0; j<squeezeWidth; ++j)
            squeezedValues[i]+=*cur++;
    }

    const double max = *std::max_element(squeezedValues,
                                         squeezedValues+nbrSqueezedValues);

    squeezedValues[nbrSqueezedValues-1] -= intensities[255];

    quint8 heights[nbrSqueezedValues];
    for (unsigned i=0; i<nbrSqueezedValues; ++i)
    {
        heights[i]=255*(squeezedValues[i]/max);
    }

    _pixmap->fill(Qt::black);

    if (max != 0)
    {
        _painter.setPen(Qt::white);
        _painter.setBrush(_brush);
        for (unsigned i=0; i<nbrSqueezedValues; ++i)
        {
            if (heights[i])
            {
                _painter.drawRect(i*squeezeWidth, 255-heights[i],
                                  squeezeWidth, heights[i]);
            }
        }

        const quint8 h = 255*(intensities[255]/max);
        if (h)
        {
            _painter.setPen(Qt::red);
            _painter.setBrush(_redBrush);

            _painter.drawRect(nbrSqueezedValues*squeezeWidth, 255-h,
                              squeezeWidth, h);
        }

    }
    setPixmap(*_pixmap);
}

//------------------------------------------------------------------------------

}
