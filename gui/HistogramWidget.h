#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QBrush>
#include <QLabel>
#include <QPainter>


class QPixmap;

namespace gui {

//------------------------------------------------------------------------------


class HistogramWidget : public QLabel
{
    Q_OBJECT

public:
    typedef quint32 Data[256];
    // In the case of an uniform image, there are 256x256 pixels with the same
    // value. This doesn't hold in a quint16. So a quint32 is needed.

    explicit HistogramWidget(QWidget *parent = 0);

    void update(const Data& intensities);
signals:

public slots:

private:
    QPixmap *_pixmap;
    QPainter _painter;
};

//------------------------------------------------------------------------------

}
#endif // HISTOGRAMWIDGET_H
