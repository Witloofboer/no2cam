#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QLabel>
#include <QPainter>

class QPixmap;

namespace gui {

//------------------------------------------------------------------------------


class HistogramWidget : public QLabel
{
    Q_OBJECT

public:
    typedef quint16 Data[256];
    explicit HistogramWidget(QWidget *parent = 0);

    void update(const Data& intensities);
signals:

public slots:

private:
    QPixmap *_pixmap;
    QPainter _pixPaint;
};

//------------------------------------------------------------------------------

}
#endif // HISTOGRAMWIDGET_H
