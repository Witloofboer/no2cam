#include "HistogramWidget.h"

#include <QPainter>

namespace gui {

static const unsigned barFactor = 1;
static const unsigned barWidth = 1 << barFactor;
static const unsigned nbrBars = 256 >> barFactor;

static const unsigned satLimit1 = 230 >> barFactor;
static const unsigned satLimit2 = 245 >> barFactor;

static const QColor mainColor(255, 255, 255);

static const QColor satColor1(255, 192, 0);
static const QColor satBgColor1(128, 96, 0);

static const QColor satColor2(255, 0, 0);
static const QColor satBgColor2(128, 0, 0);

static const QBrush mainBrush(mainColor, Qt::SolidPattern);
static const QBrush satBrush1(satColor1, Qt::SolidPattern);
static const QBrush satBgBrush1(satBgColor1, Qt::SolidPattern);
static const QBrush satBrush2(satColor2, Qt::SolidPattern);
static const QBrush satBgBrush2(satBgColor2, Qt::SolidPattern);


//------------------------------------------------------------------------------

HistogramWidget::HistogramWidget(QWidget *parent)
    : QLabel(parent)
    , _pixmap(new QPixmap(256+barWidth, 256))
    , _painter(_pixmap)
{
    _pixmap->fill(Qt::black);
    setPixmap(*_pixmap);
}

//------------------------------------------------------------------------------

void HistogramWidget::update(const Data& intensities)
{

    quint32 absoluteBarValues[nbrBars];
    quint32 const * cur = intensities;

    for (unsigned i=0; i<nbrBars; ++i)
    {
        absoluteBarValues[i]=0;
        for(unsigned j=0; j<barWidth; ++j)
            absoluteBarValues[i]+=*cur++;
    }

    const double maxAbsoluteBarValue = *std::max_element(absoluteBarValues,
                                       absoluteBarValues+nbrBars);

    quint8 barHeights[nbrBars];
    for (unsigned i=0; i<nbrBars; ++i)
    {
        barHeights[i]=255*(absoluteBarValues[i]/maxAbsoluteBarValue);
    }

    _pixmap->fill(Qt::black);

    if (maxAbsoluteBarValue != 0)
    {
        _painter.setPen(mainColor);
        _painter.setBrush(mainColor);

        for (unsigned i=0; i<satLimit1; ++i)
        {
            if (barHeights[i])
            {
                _painter.drawRect(i*barWidth, 255-barHeights[i],
                                  barWidth, barHeights[i]);
            }
        }

        _painter.setPen(satBgColor1);
        _painter.setBrush(satBgColor1);

        for (unsigned i=satLimit1; i<satLimit2; ++i)
        {
            if (barHeights[i])
            {
                _painter.drawRect(i*barWidth, 0, barWidth, 255-barHeights[i]);
            }
        }

        _painter.setPen(satColor1);
        _painter.setBrush(satColor1);

        for (unsigned i=satLimit1; i<satLimit2; ++i)
        {
            if (barHeights[i])
            {
                _painter.drawRect(i*barWidth, 255-barHeights[i],
                                  barWidth, barHeights[i]);
            }
        }

        _painter.setPen(satBgColor2);
        _painter.setBrush(satBgColor2);

        for (unsigned i=satLimit2; i<256; ++i)
        {
            if (barHeights[i])
            {
                _painter.drawRect(i*barWidth, 0, barWidth, 255-barHeights[i]);
            }
        }

        _painter.setPen(satColor2);
        _painter.setBrush(satColor2);

        for (unsigned i=satLimit2; i<256; ++i)
        {
            if (barHeights[i])
            {
                _painter.drawRect(i*barWidth, 255-barHeights[i],
                                  barWidth, barHeights[i]);
            }
        }
    }

    setPixmap(*_pixmap);
}

//------------------------------------------------------------------------------

}
